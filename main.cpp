#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "get_display.h"
#include "keystrokes.h"
#include <cstring>
#include <string.h>
using namespace std;

char now_path[100]; 
string home ;
int main(int argc , char *argv[])
{
	//Dirent type of structure cz dirent has the name and other attributes of the file 
	DIR *directory ; 

	getcwd(now_path , sizeof(now_path));
	home = now_path; 
	directory = opendir(".");
	if(directory == NULL)
	{
		printf("Error : Unable to open directory.\n");
		exit(1);
	}	

function_to_store_in_termios_getchar();
	
int num_entries = get_num_entries(now_path);

struct information ptr[num_entries];
	
get_permissions(ptr , now_path);
display(num_entries , ptr , now_path);

return 0;
}