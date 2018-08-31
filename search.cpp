#include <iostream>
#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include "get_display.h"
#include "keystrokes.h"
#include <unistd.h>
#include <vector>
#include <string>
#include <sys/ioctl.h>
#include <errno.h>

using namespace std;


int flag = 0;

//Path should be absolute from /home/rootuser..... And file name should be mentioned


int check_for_dir(string sent_src_file )  //returns true if input is a directory
{
   struct stat stat_structure;
   int dir = 0;
   if (stat(&sent_src_file[0] , &stat_structure) != -1) 
   	{
		if (S_ISDIR(stat_structure.st_mode)) 
		{
		   dir = 1;
		}
	}
   return dir;
}



void search(string src_dir_name , string search_name)  
{
	

					DIR* directory;

					struct dirent* dirent_ptr; //dirent pointer to get the name of the file . 

					directory = opendir(&src_dir_name[0]); 
					
					chdir(&src_dir_name[0]);
					
					if(directory == NULL)
						{
							return ; 
						}

					while( (dirent_ptr = readdir( directory ) ) != NULL )
						{
							string appended_src_file ;

							if(  ( strcmp(dirent_ptr->d_name,".")!=0 )  &&  ( strcmp(dirent_ptr->d_name,"..")!=0 )  )
		
								{
									appended_src_file = src_dir_name;

									appended_src_file = appended_src_file + "/" + (dirent_ptr->d_name); //append the filename to again check if it's directory or a normal file

									int check_dir = check_for_dir( appended_src_file );	//check if it's a directory or a file 

									//And accordingly call the recursive function for directory or a file 

									if(strcmp(dirent_ptr->d_name , &search_name[0] ) == 0)
										{
											cout<<src_dir_name<<"/"<<search_name<<endl; 
											flag = 1;
										}

									else if(check_dir)
										{
											search(appended_src_file , search_name);	
										}
									else
										{										
										}
								}//if
						}//while
}//function




int main(int arg_count , char *argv[])
{

	DIR* directory;

	struct dirent* dirent_ptr;

if(arg_count <2  )
{
	cout<<"Error : Too few arguments "<<endl;
	exit(0);
}

	string search_filename(argv[1]);

	string source_dir_name = "/home/rootuser";

	directory = opendir(&source_dir_name[0]);


	if(directory ==NULL)
		{

			cout<<"Not opening the directory. Please check the Path Again > "<<endl;
		}
	else
	{
					search(source_dir_name , search_filename); //If it's a directory then there's work to do . So call this guy.
	}			


	if(flag == 0 )
		cout<<"File Not Found Anywhere"<<endl;
	closedir(directory); // It's a good habit to close the directory .

	return 0 ; 
}
