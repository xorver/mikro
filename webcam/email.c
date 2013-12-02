#include "email.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void emailInit(){
	system("mkdir -p ~/.mutt/cache/headers");
	system("mkdir -p ~/.mutt/cache/bodies");
	system("touch -p ~/.mutt/certificates");
	system("sudo cp ../.muttrc ~/");
}
void sendVideo(char* path){
	char command[1024];
	command[0]=0;
	strcat(command,"echo \"Zaobserwowano ruch, nagranie w zalaczniku.\" | sudo mutt -s \"nagranie z monitoringu\" -a ");
	strcat(command,path);
	strcat(command," -- ");
	strcat(command,RECEIVER_ADDRESS);
	system(command);
}
