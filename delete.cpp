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
#include <cstdio>

using namespace std;

int flag = 1;

void delete_file(string to_delete_path)
{
	if(remove(&to_delete_path[0])!=0)
	{
		strerror(errno);
	}
	else
		flag = 0;
}



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



int check_for_file(string sent_src_file)  //returns true if input is a file not a directory
{
   struct stat stat_structure;
   int file = 1;
   if (stat(&sent_src_file[0] , &stat_structure) != -1) 
   	{
		if (S_ISDIR(stat_structure.st_mode)) 
		{
		   file = 0;
		}
	}
   return file;
}



string find_src_directory(string sent_src_directory) //Returns the directory i.e. the name before '/' in the sent source file 
{
	
	int index_of ;
		
	index_of = sent_src_directory.find_last_of('/');

	int i = sent_src_directory.size();

	while(sent_src_directory[i]!='/')
	{
		sent_src_directory[i]='\0';
		i--;
	}

	sent_src_directory[i]='\0';
	
	return sent_src_directory;
}


string find_src_file(string sent_src_directory) //Returns file name i.e. file name after '/'
{
	int len_sent_src_file ,index_of ;

	string src_file;
	
	len_sent_src_file = strlen(&sent_src_directory[0]);

	index_of = sent_src_directory.find_last_of('/');
	
	src_file = sent_src_directory.substr(index_of + 1,len_sent_src_file);
	
	return src_file;
}





void to_empty(string src_file_name ) // If input is a directory and we have to copy it 
{
	
			string dir_name = find_src_directory(src_file_name);  //To find the directory before /

			chdir(&dir_name[0]); // change to that directory
			
			string cwd;
			
			string dir_only_name = find_src_file(src_file_name) ; // Find file name i.e. name at the end .
			
//			char *temp = &dest_file_name[0];

					DIR* directory;

					struct dirent* dirent_ptr; //dirent pointer to get the name of the file . 

					string source_dir_name = find_src_directory(&src_file_name[0]);  //to open directory find the path before the / of last filename

					directory = opendir(&src_file_name[0]); 
					
					chdir(&src_file_name[0]);
					
					if(directory == NULL)
						{
							cout<<"Not opening the directory. Please check the Path Again "<<endl;
							return ; 
						}

					while( (dirent_ptr = readdir( directory ) ) != NULL )
						{

							string appended_src_file ;
							string appended_dest_file ;

							if(  ( strcmp(dirent_ptr->d_name,".")!=0 )  &&  ( strcmp(dirent_ptr->d_name,"..")!=0 )  )
		
								{
								//	cout<<dirent_ptr->d_name<<endl; 
																

									//appended_dest_file = dest_file_name;
									
									appended_src_file = src_file_name;


									appended_src_file = appended_src_file + "/" + (dirent_ptr->d_name); //append the filename to again check if it's directory or a normal file

									//appended_dest_file = appended_dest_file + "/" + (dirent_ptr->d_name); // append the filename to give the input to the copy function.(Remember absolute path always) 

									int check_dir = check_for_dir( appended_src_file );	//check if it's a directory or a file 

									//And accordingly call the recursive function for directory or a file 

									if(check_dir)
										{
											to_empty(appended_src_file );	
										}
									else
										{
											delete_file(&appended_src_file[0] );	
										}
								}//if
						}//while
/*
	struct stat fix_permissions;
    stat(&src_file_name[0] , &fix_permissions);
    chown(&dest_file_name[0] , fix_permissions.st_uid , fix_permissions.st_gid);   //update owner and group to original file
    chmod(&dest_file_name[0] , fix_permissions.st_mode);   //update the permissions like that of original file*/

		    if(rmdir(&src_file_name[0])!=0)
		    {
		    	cout<<"Error "<<endl;
		    	flag = 0;
		    }
    
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

	string temp_source_file(argv[1]); //temporary because it's a char* i need strings as always
//	string dest_filename(argv[1]);

	string source_file_name; //file name only. name after last '/' in the path
	string source_dir_name; //directory name only. name before last '/' in the path 

	strcpy(&source_dir_name[0] , argv[1]);
	
	source_dir_name = find_src_directory(temp_source_file);

	source_file_name = find_src_file(temp_source_file);

//	dest_filename = dest_filename + "/" + source_file_name ;


	directory = opendir(&source_dir_name[0]);

//	cout<<dest_filename<<endl;

	if(directory ==NULL)
		{
			cout<<"Not opening the directory. Please check the Path Again > "<<endl;
		}
	else
	{
			int check_dir = check_for_dir(temp_source_file); //Check for a directory 

			int check_file = check_for_file(temp_source_file); //Check for a file

			if(check_file)
			{
				delete_file(&temp_source_file[0] );	//If it'a normal file. Just copy it 
				if(flag == 1)
				cout<<endl<<"Deleted Succesfully "<<endl;
			}
			else 
			{
				to_empty(temp_source_file ); //If it's a directory then there's work to do . So call this guy.
				cout<<endl<<"Deleted Succesfully "<<endl;
			}
	} //Done and dusted after this.

	
	closedir(directory); // It's a good habit to close the directory .

	return 0 ; 
}
