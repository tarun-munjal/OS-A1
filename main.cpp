#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "get_display.h"
#include "keystrokes.h"

using namespace std;

int main(int argc , char *argv[])
{
	//Dirent type of structure cz dirent has the name and other attributes of the file 
	DIR *directory ; 

	directory = opendir(".");
	if(directory == NULL)
	{
		printf("Error : Unable to open directory.\n");
		exit(1);
	}	

int num_entries = get_num_entries();

struct information ptr[num_entries];

get_permissions(ptr);

keystrokes(num_entries , ptr);
			
return 0;
}