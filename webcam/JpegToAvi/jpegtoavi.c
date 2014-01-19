#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE

#include "jpegtoavi.h"
#include "byteswap.h"
#include <stdio.h>
#include "avifmt.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "llist.h"

typedef struct _Jpeg_Data Jpeg_Data;

struct _Jpeg_Data
{
  DWORD size;
  DWORD offset;
  char name[0]; /* i.e. variable length structure */
};

#define JPEG_DATA_SZ (sizeof(DWORD) * 2)

off_t file_sz(char *fn)
{
  struct stat s;
  if (stat(fn, &s) == -1)
    return -1;
  return s.st_size;
}

off_t get_file_sz(List *l)
{
  off_t tmp, ret = 0;

  for(; (l); l = l->next) {

    if((tmp = file_sz(((Jpeg_Data *)l->data)->name)) == -1)
      return -1;
    ((Jpeg_Data *)l->data)->size = (DWORD) tmp;
    tmp += ((4-(tmp%4)) % 4);
    ret += tmp;
  }

  return ret;
}

List *get_file_list_stdin(void)
{
  char fn[PATH_MAX];
  Jpeg_Data *tmp;
  List *ret = (List *) malloc(sizeof(List)),
       *l = ret;

  ret->data = 0;
  ret->prev = 0;
  ret->next = 0;

  while (scanf("%s", fn) == 1) {

    tmp = (Jpeg_Data *) malloc(strlen(fn) + 1 + JPEG_DATA_SZ);
    tmp->offset = 0;
    tmp->size = 0;
    strcpy(tmp->name, fn);
    if (l->data == 0)
      l->data = tmp;
    else
      l = list_push_back(l, tmp);
  }

  return ret;
}


List *get_file_list_argv(int argSize, char **arguments)
{
  List *ret = (List *) malloc(sizeof(List)),
       *l = ret;
  Jpeg_Data *tmp;
  int i;

  ret->data = 0;
  ret->prev = 0;
  ret->next = 0;

  for (i = 0; i < argSize; ++i) {
    tmp = (Jpeg_Data *) malloc(strlen(arguments[i]) + 1 + JPEG_DATA_SZ);
    strcpy(tmp->name, arguments[i]);
    if (l->data == 0)
      l->data = tmp;
    else
      l = list_push_back(l, tmp);
  }

  return ret;
}



void print_quartet(unsigned int i, FILE* f)
{
    fputc(i % 0x100, f); i /= 0x100;
    fputc(i % 0x100, f); i /= 0x100;
    fputc(i % 0x100, f); i /= 0x100;
    fputc(i % 0x100, f);
}


int makeMovieInside(int argSize, char **arguments,char* path)
{
  FILE *film = fopen(path, "w");
 
  DWORD per_usec = 1;
  DWORD width;
  DWORD height;
  DWORD frames = 1;
  unsigned int fps;
  unsigned short img0;
  off64_t jpg_sz_64, riff_sz_64;
  long jpg_sz = 1;
  const off64_t MAX_RIFF_SZ = 2147483648LL; /* 2 GB limit */
  DWORD riff_sz;

  int fd;
  long nbr, nbw, tnbw = 0;
  char buff[512];
  off_t mfsz, remnant;
  List *frlst = 0,
       *f = 0;

  struct AVI_list_hdrl hdrl = {
    /* header */
    {
      {'L', 'I', 'S', 'T'},
      LILEND4(sizeof(struct AVI_list_hdrl) - 8),
      {'h', 'd', 'r', 'l'}
    },

    /* chunk avih */
    {'a', 'v', 'i', 'h'},
    LILEND4(sizeof(struct AVI_avih)),
    {
      LILEND4(per_usec),
      LILEND4(1000000 * (jpg_sz/frames) / per_usec),
      LILEND4(0),
      LILEND4(AVIF_HASINDEX),
      LILEND4(frames),
      LILEND4(0),
      LILEND4(1),
      LILEND4(0),
      LILEND4(width),
      LILEND4(height),
      {LILEND4(0), LILEND4(0), LILEND4(0), LILEND4(0)}
    },

    /* list strl */
    {
      {
    {'L', 'I', 'S', 'T'},
    LILEND4(sizeof(struct AVI_list_strl) - 8),
    {'s', 't', 'r', 'l'}
      },

      /* chunk strh */
      {'s', 't', 'r', 'h'},
      LILEND4(sizeof(struct AVI_strh)),
      {
    {'v', 'i', 'd', 's'},
    {'M', 'J', 'P', 'G'},
    LILEND4(0),
    LILEND4(0),
    LILEND4(0),
    LILEND4(per_usec),
    LILEND4(1000000),
    LILEND4(0),
    LILEND4(frames),
    LILEND4(0),
    LILEND4(0),
    LILEND4(0)
      },
      
      /* chunk strf */
      {'s', 't', 'r', 'f'},
      sizeof(struct AVI_strf),
      {      
    LILEND4(sizeof(struct AVI_strf)),
    LILEND4(width),
    LILEND4(height),
    LILEND4(1 + 24*256*256),
    {'M', 'J', 'P', 'G'},
    LILEND4(width * height * 3),
    LILEND4(0),
    LILEND4(0),
    LILEND4(0),
    LILEND4(0)
      },

      /* list odml */
      {
    {
      {'L', 'I', 'S', 'T'},
      LILEND4(16),
      {'o', 'd', 'm', 'l'}
    },
    {'d', 'm', 'l', 'h'},
    LILEND4(4),
    LILEND4(frames)
      }
    }
  };
  
  sscanf(arguments[1], "%u", &fps);
  per_usec = 1000000 / fps;
  img0 = 4;
      
  if (img0 >= argSize)
    frlst = get_file_list_stdin();
  else
    frlst = get_file_list_argv(argSize - img0 , &arguments[img0]);

  frames = list_size(frlst);

  /* getting image, and hence, riff sizes */
  jpg_sz_64 = get_file_sz(frlst);

  if (jpg_sz_64 == -1) {
    fprintf(stderr, "couldn't determine size of images\n");
    return -2;
  }

  riff_sz_64 = sizeof(struct AVI_list_hdrl) + 4 + 4 + jpg_sz_64
    + 8*frames + 8 + 8 + 16*frames;

  if (riff_sz_64 >= MAX_RIFF_SZ) {
    fprintf(stderr,"RIFF would exceed 2 Gb limit\n");
    return -3;
  }

  jpg_sz = (long) jpg_sz_64;
  riff_sz = (DWORD) riff_sz_64;

  /* printing predicted file size and quitting */
  if (img0 == 2) {
    fprintf(film,"%lu\n", (unsigned long) riff_sz + 8UL);
    return 0;
  }

  /* printing AVI riff hdr */
  fprintf(film,"RIFF");
  print_quartet(riff_sz, film);
  fprintf(film,"AVI ");

  /* list hdrl */
  hdrl.avih.us_per_frame = LILEND4(per_usec);
  hdrl.avih.max_bytes_per_sec = LILEND4(1000000 * (jpg_sz/frames)
                      / per_usec);
  hdrl.avih.tot_frames = LILEND4(frames);
  hdrl.avih.width = LILEND4(width);
  hdrl.avih.height = LILEND4(height);
  hdrl.strl.strh.scale = LILEND4(per_usec);
  hdrl.strl.strh.rate = LILEND4(1000000);
  hdrl.strl.strh.length = LILEND4(frames);
  hdrl.strl.strf.width = LILEND4(width);
  hdrl.strl.strf.height = LILEND4(height);
  hdrl.strl.strf.image_sz = LILEND4(width * height * 3);
  hdrl.strl.list_odml.frames = LILEND4(frames); /*  */
  fwrite(&hdrl, sizeof(hdrl), 1, film);

  /* list movi */
  fprintf(film, "LIST");
  print_quartet(jpg_sz + 8*frames + 4, film);
  fprintf(film,"movi");

  /* list movi .. frames */
  for (f = frlst; (f); f = f->next) {
    fprintf(film,"00db");
    mfsz = ((Jpeg_Data *) f->data)->size;
    remnant = (4-(mfsz%4)) % 4;
    print_quartet(mfsz + remnant, film);
    ((Jpeg_Data *) f->data)->size += remnant;

    if (f == frlst) {
      ((Jpeg_Data *) f->data)->offset = 4;

    } else {
      ((Jpeg_Data *) f->data)->offset = 
    ((Jpeg_Data *) f->prev->data)->offset +
    ((Jpeg_Data *) f->prev->data)->size + 8;
    }

    if ((fd = open(((Jpeg_Data *) f->data)->name, O_RDONLY)) < 0) {
      fprintf(stderr, "couldn't open file!\n");
      list_rerased(frlst);
      return -6;
    }
    nbw = 0;

    if ((nbr = read(fd, buff, 6)) != 6) {
      fprintf(stderr, "error\n");
      list_rerased(frlst);
      return -7;
    }
    fwrite(buff, nbr, 1, film);
    read(fd, buff, 4);
    fwrite("AVI1", 4, 1, film);
    nbw = 10;

    while ((nbr = read(fd, buff, 512)) > 0) {
      fwrite(buff, nbr, 1, film);
      nbw += nbr;
    }
    if (remnant > 0) {
      fwrite(buff, remnant, 1, film);
      nbw += remnant;
    }
    tnbw += nbw;
    close(fd);
  }

  if (tnbw != jpg_sz) {
    fprintf(stderr, "error writing images (wrote %ld bytes, expected %ld bytes)\n",
        tnbw, jpg_sz);
    list_rerased(frlst);
    return -8;
  }
  
  /* indices */
  fprintf(film, "idx1");
  print_quartet(16 * frames, film);
  for (f = frlst; (f); f = f->next) {
    fprintf(film, "00db");
    print_quartet(18, film);
    print_quartet(((Jpeg_Data *) f->data)->offset, film);
    print_quartet(((Jpeg_Data *) f->data)->size, film);
  }

  close(fd);
  list_rerased(frlst);
  fflush(film);
  fclose(film);
}

void makeMovie(int amount, char* picturesPath, char* videoPath, int fps){
    char** photos = malloc(sizeof(char*)*(amount+4));
    int i;
    for(i=0; i<amount+4; i++){
        photos[i] = malloc(50); 
    }
    sprintf(photos[0], "moviemaker");
    sprintf(photos[1], "%d",fps);
    sprintf(photos[2], "%d", W);
    sprintf(photos[3], "%d", H);

    char tmp[50];
    for(i=4; i<amount+4;i++){
        sprintf(tmp, "%d", (i-4));  
        strcpy(photos[i], picturesPath);
        strcat(photos[i], tmp);
    }
    makeMovieInside(amount+4, photos, videoPath);
    
    for(i=0; i<amount+4; i++){
        free(photos[i]);
    }
    free(photos);
    
}