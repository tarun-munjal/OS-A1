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
#include <stack>
#include <vector>
#include <sys/ioctl.h>

using namespace std;

//char char_path[100][100]; 

char now_path[100][100];

int path_counter = 0;

int max_counter = 1;

char home[100] ;

int main(int argc , char *argv[])
{
	//Dirent type of structure cz dirent has the name and other attributes of the file 
	DIR *directory ; 
	    
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

   // printf("%d\n", w.ws_row);
   // printf("%d\n",w.ws_col);

	getcwd(now_path[path_counter] , sizeof(now_path[path_counter]));
	
	strcpy(home , now_path[path_counter]);
	

	chdir(now_path[path_counter]);

	directory = opendir(".");
	if(directory == NULL)
	{
		printf("Error : Unable to open directory.\n");
		exit(1);
	}	

function_to_store_in_termios_getchar();
	
int num_entries = get_num_entries(now_path[path_counter]);

struct information ptr[num_entries];
	
//cout<<now_path[path_counter];
display(num_entries , ptr );// now_path[path_counter++]);

return 0;
}
