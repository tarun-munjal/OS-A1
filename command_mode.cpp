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
#include "command_mode.h"
#include <unistd.h>
#include <vector>
#include <string>
#include <sys/ioctl.h>
#include <errno.h>

extern char now_path[100][100];
extern int path_counter;
extern char home[100];
string temp_home(home);

using namespace std;

   
int search_index=0;
extern string search_result[10];
int flag = 1;

void go_for_command(string command )
{
	string command_arg ;
	char *command_array[500];
	int  arg_index = 0; 

	char* token = strtok(&command[0] , " ");

	while( token != NULL)
	{
		command_array[arg_index++] = token ;
		token = strtok(NULL," ");
    }

		if( strcmp(command_array[0] , "copy") == 0 )
		{

			copy_recursive( arg_index , command_array );
		}
		else if( strcmp(command_array[0] , "move") == 0)
		{
			move_recursive(arg_index , command_array );
		}
		else if(strcmp(command_array[0] ,"rename") == 0)
		{
			rename_recursive(arg_index , command_array );
		}
		else if( strcmp(command_array[0] , "create_file") == 0)
		{	
			create_file( command_array[1] , command_array[2] );
		}
		else if( strcmp(command_array[0] ,"create_dir") == 0 )
		{
			create_dir( command_array[1] , command_array[2] );
		}
		else if( strcmp(command_array[0] , "delete_file" )== 0 )
		{
			delet_recursive( arg_index , command_array );
		}
		else if( strcmp(command_array[0] , "goto") == 0 )
		{
			goto_recursive( command_array );
		}
		else if( strcmp(command_array[0] , "search") == 0)
		{
			search_recursive( arg_index , command_array );
		}
		else if( strcmp(command_array[0] , "snapshot") == 0 )
		{
			snap_recursive( arg_index , command_array );
		}
		else
		{
			cout<<"Enter valid Command please";
		}
}




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



void snap(string src_dir_name ,string file)  
{
	
	ofstream dest_filename;

	string temp(home);
	temp = temp + "/" + file;

	dest_filename.open(temp , ios::out | ios::app);

	DIR* directory;

	struct dirent* dirent_ptr; //dirent pointer to get the name of the file . 

	directory = opendir(&src_dir_name[0]); 
	
	dest_filename<<src_dir_name<<" : \t";
	
	if(directory == NULL)
		{
			return ; 
		}
	while( (dirent_ptr = readdir( directory ) ) != NULL )
		{
			if(  ( strcmp(dirent_ptr->d_name,".")!=0 )  &&  ( strcmp(dirent_ptr->d_name,"..")!=0 )  )
				{
					dest_filename<<(string)dirent_ptr->d_name<<"   ";
				} 	
		}
		dest_filename.close();

	directory = opendir(&src_dir_name[0]);

	while( (dirent_ptr = readdir( directory ) ) != NULL )
		{
			string appended_src_file ;

			if(  ( strcmp(dirent_ptr->d_name,".")!=0 )  &&  ( strcmp(dirent_ptr->d_name,"..")!=0 )  )

				{

					appended_src_file = src_dir_name;

					appended_src_file = appended_src_file + "/" + (dirent_ptr->d_name); //append the filename to again check if it's directory or a normal file

					int check_dir = check_for_dir( appended_src_file );	//check if it's a directory or a file 

					if(check_dir)
						{
							snap(appended_src_file , file);	
						}
				}//if
				
		}//while
}//function




void snap_recursive(int arg_count , char *argv[])
{

	DIR* directory;

	struct dirent* dirent_ptr;

if(arg_count <3  )
{
	cout<<"Error : Too few arguments "<<endl;
	return ;
}

	string source_dir_name(home) ;
	source_dir_name = source_dir_name + argv[1];

	string file(argv[2]);
	directory = opendir(&source_dir_name[0]);


	if(directory ==NULL)
		{

			cout<<"Not opening the directory. Please check the Path Again > "<<endl;
		}
	else
	{
					snap(source_dir_name , file ); //If it's a directory then there's work to do . So call this guy.
	}			


	closedir(directory); // It's a good habit to close the directory .

}

//snapshot end 
//________________________________________________________________________________________________________________________________________________________________


void copy(char *src_filename , char *dest_filename)
{
	cout<<"Copying...... "<<endl;
	ifstream in (src_filename); 	// open original file
 	ofstream out(dest_filename); 	// open target file
 	out << in.rdbuf(); 		
 	flag = 1;	    // read original file into target
}




void to_copy_directory(string src_file_name , string dest_file_name) // If input is a directory and we have to copy it 
{
	
			int status; //status of new directory made or not !
			string dir_name = find_src_directory(dest_file_name);  //To find the directory before /
			chdir(&dir_name[0]); // change to that directory
			
			string cwd;
			
			string dir_only_name = find_src_file(src_file_name) ; // Find file name i.e. name at the end .
			
			char *temp = &dest_file_name[0];
			status = mkdir(temp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //function in c++ to create a directory 
			if(status == -1) //If new directory was not made succesful !
				{
						cout<<strerror(errno)<<endl;
						flag = 0;
						return ; 
				}
			
					DIR* directory;
					struct dirent* dirent_ptr; //dirent pointer to get the name of the file . 
					string source_dir_name = find_src_directory(&src_file_name[0]);  //to open directory find the path before the / of last filename
					directory = opendir(&src_file_name[0]); 
					
					chdir(&src_file_name[0]);
					
					if(directory == NULL)
						{
							cout<<"Not opening the directory. Please check the Path Again "<<endl;
							flag =0;
							return ; 
						}
					while( (dirent_ptr = readdir( directory ) ) != NULL )
						{

							string appended_src_file ;
							string appended_dest_file ;
							if(  ( strcmp(dirent_ptr->d_name,".")!=0 )  &&  ( strcmp(dirent_ptr->d_name,"..")!=0 )  )
		
								{
									appended_dest_file = dest_file_name;
									
									appended_src_file = src_file_name;
									appended_src_file = appended_src_file + "/" + (dirent_ptr->d_name); //append the filename to again check if it's directory or a normal file
									appended_dest_file = appended_dest_file + "/" + (dirent_ptr->d_name); // append the filename to give the input to the copy function.(Remember absolute path always) 
									
									int check_dir = check_for_dir( appended_src_file );	//check if it's a directory or a file 
									//And accordingly call the recursive function for directory or a file 
									if(check_dir)
										{
											to_copy_directory(appended_src_file , appended_dest_file);	
										}
									else
										{
											copy(&appended_src_file[0] , &appended_dest_file[0]);	
										}
								}//if
						}//while
	}//function




void copy_recursive(int arg_count , char *argv[])
{
	DIR* directory;
	struct dirent* dirent_ptr;
if(arg_count <3  )
{
	cout<<"Error : Too few arguments "<<endl;
	exit(0);
}

for(int i = 1 ; i  < arg_count - 1 ; i ++)
{
	string  home_path(argv[i]); //temporary because it's a char* i need strings as always

	string temp_source_file(home) ;
	temp_source_file = temp_source_file + home_path;

	string temp_dest_file(argv[arg_count - 1]);
	string dest_filename(home) ;
	dest_filename = dest_filename + temp_dest_file;

	string source_file_name; //file name only. name after last '/' in the path
	string source_dir_name; //directory name only. name before last '/' in the path 

	strcpy(&source_dir_name[0] , argv[1]);

	source_dir_name = find_src_directory(temp_source_file);
	source_file_name = find_src_file(temp_source_file);

	dest_filename = dest_filename + "/" + source_file_name ;

	directory = opendir(&source_dir_name[0]);
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
				copy(&temp_source_file[0] , &dest_filename[0]);	//If it'a normal file. Just copy it 
			}
			else 
			{
				to_copy_directory(temp_source_file , dest_filename); //If it's a directory then there's work to do . So call this guy.
			}
	}
	if(flag == 1)
	cout<<"Done"; //Done and dusted after this.
	
	closedir(directory); // It's a good habit to close the directory .
}

}


//copy recursive end here
//________________________________________________________________________________________________________________________________________________________________


//Path should be absolute from /home/rootuser..... and destination path should be till the destination directory
void move(char *src_filename , char *dest_filename)
{

	cout<<"Moving......"<<endl;
	if(rename(src_filename , dest_filename ) == -1 )
		{
			strerror( errno);
		}
}


void to_move_directory(string src_file_name , string dest_file_name) // If input is a directory and we have to copy it 
{
	
			int status; //status of new directory made or not !

			string dir_name = find_src_directory(dest_file_name);  //To find the directory before /

			chdir(&dir_name[0]); // change to that directory
			
			string cwd;
			
			string dir_only_name = find_src_file(src_file_name) ; // Find file name i.e. name at the end .
			
			char *temp = &dest_file_name[0];


			status = mkdir(temp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //function in c++ to create a directory 

			if(status == -1) //If new directory was not made succesful !
				{
						cout<<strerror(errno)<<endl;
						return ; 
				}
			

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
									appended_dest_file = dest_file_name;
									
									appended_src_file = src_file_name;


									appended_src_file = appended_src_file + "/" + (dirent_ptr->d_name); //append the filename to again check if it's directory or a normal file

									appended_dest_file = appended_dest_file + "/" + (dirent_ptr->d_name); // append the filename to give the input to the copy function.(Remember absolute path always) 

									int check_dir = check_for_dir( appended_src_file );	//check if it's a directory or a file 

									//And accordingly call the recursive function for directory or a file 

									if(check_dir)
										{
											to_move_directory(appended_src_file , appended_dest_file);	
										}
									else
										{
											move(&appended_src_file[0] , &appended_dest_file[0]);	
										}
								}//if
						}//while

	struct stat fix_permissions;
    stat(&src_file_name[0] , &fix_permissions);
    chown(&dest_file_name[0] , fix_permissions.st_uid , fix_permissions.st_gid);   //update owner and group to original file
    chmod(&dest_file_name[0] , fix_permissions.st_mode);   //update the permissions like that of original file

    if(rmdir(&src_file_name[0])!=0)
    {
    	cout<<"Error "<<endl;
    }
    
	}//function




void move_recursive(int arg_count , char *argv[])
{
	DIR* directory;

	struct dirent* dirent_ptr;

if(arg_count <3  )
{
	cout<<"Error : Too few arguments "<<endl;
	exit(0);
}


for(int i = 1 ; i < arg_count-1 ; i++)
{
	string path(argv[i]); //temporary because it's a char* i need strings as always
	string temp_source_file(home); 
	temp_source_file = temp_source_file + path ;
	string dest(argv[arg_count -1]);
	string dest_filename(home) ;
	dest_filename = dest_filename + dest;

	string source_file_name; //file name only. name after last '/' in the path
	string source_dir_name; //directory name only. name before last '/' in the path 

	strcpy(&source_dir_name[0] , argv[i]);
	
	source_dir_name = find_src_directory(temp_source_file);

	source_file_name = find_src_file(temp_source_file);

	dest_filename = dest_filename + "/" + source_file_name ;

	directory = opendir(&source_dir_name[0]);


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
				move(&temp_source_file[0] , &dest_filename[0]);	//If it'a normal file. Just copy it 
				cout<<endl<<"Done "<<endl;
			}
			else 
			{
				to_move_directory(temp_source_file , dest_filename); //If it's a directory then there's work to do . So call this guy.
				cout<<endl<<"Done "<<endl;
			}
	} //Done and dusted after this.
	
	closedir(directory); // It's a good habit to close the directory .
}

}



//move recursive ends here
//________________________________________________________________________________________________________________________________________________________________




void delete_file(string to_delete_path)
{
	if(remove(&to_delete_path[0])!=0)
	{
		strerror(errno);
	}
	else
		cout<<"Deletion Succesful "<<endl;
}


void to_empty(string src_file_name ) 
{
	
			string dir_name = find_src_directory(src_file_name);  //To find the directory before /

			chdir(&dir_name[0]); // change to that directory
			
			string cwd;
			
			string dir_only_name = find_src_file(src_file_name) ; // Find file name i.e. name at the end .

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
									appended_src_file = src_file_name;

									appended_src_file = appended_src_file + "/" + (dirent_ptr->d_name); //append the filename to again check if it's directory or a normal file

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

		    if(rmdir(&src_file_name[0])!=0)
		    {
		    	cout<<"Error "<<endl;
		    	flag = 0;
		    }
    
	}//function






void delet_recursive(int arg_count , char *argv[])
{
	DIR* directory;

	struct dirent* dirent_ptr;

if(arg_count <2  )
{
	cout<<"Error : Too few arguments "<<endl;
	exit(0);
}

	string temp(argv[1]); //temporary because it's a char* i need strings as always
	string temp_source_file(home) ;	temp_source_file = temp_source_file + temp;
	string source_file_name; //file name only. name after last '/' in the path
	string source_dir_name; //directory name only. name before last '/' in the path 

	strcpy(&source_dir_name[0] , argv[1]);
	
	source_dir_name = find_src_directory(temp_source_file);

	source_file_name = find_src_file(temp_source_file);


	directory = opendir(&source_dir_name[0]);

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

}

//Delete ends here
//________________________________________________________________________________________________________________________________________________________________



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
											flag = 0;
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

						closedir(directory);
}//function




void search_recursive(int arg_count , char *argv[])
{

	DIR* directory;


if(arg_count <2  )
{
	cout<<"Error : Too few arguments "<<endl;
	return;
}

	string search_filename(argv[1]);

	string source_dir_name(home);

	directory = opendir(&source_dir_name[0]);
	
	
	
	if(directory ==NULL)
		{
			cout<<"Not opening the directory. Please check the Path Again "<<endl;
		}
	else
		{
						search(source_dir_name , search_filename); //If it's a directory then there's work to do . So call this guy.
		}			


	if(flag == 1 )
		cout<<"File Not Found Anywhere"<<endl;
	closedir(directory); // It's a good habit to close the directory .

}


//search ends here
//________________________________________________________________________________________________________________________________________________________________


void re_name( char *source , char *destination )
{
	if(rename(source , destination ) == -1 )
		{
			strerror( errno);
		}
}


void rename_recursive(int argc , char *argv[])
{

	if(argc <3  )
	{
		cout<<"Error : Too few arguments "<<endl;
		exit(0);
}

	
	string source(argv[1]);
	string dest(argv[2]);

	string source_send(home);  
	string dest_send(home) ;
	source_send = source_send + source;
	dest_send = dest_send + dest;
	re_name(&source_send[0] , &dest_send[0]);


}

//rename ends here
//________________________________________________________________________________________________________________________________________________________________



void create_file( string file_name , string temp_dest_address )
{
	string dest_address(home) ;
	dest_address = dest_address + temp_dest_address ;
	dest_address = dest_address + "/" + file_name;
	ofstream  file(&dest_address[0]);
	cout<<"File Succesfully Made "<<endl;
	cout<<"Navigate it in Normal Mode "<<endl;
}



void create_dir (string temp_dest_address , string dir_name)
{
	int status ;
	string dest_address(home) ;
	dest_address = dest_address + temp_dest_address;
	dest_address = dest_address + "/" + dir_name;
	status = mkdir(&dest_address[0] , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if(status == -1)
	{
		cout<<strerror(errno)<<endl;
		return ;
	}
	cout<<"Directory Succesfully Made "<<endl;
	cout<<"Navigate it in Normal Mode "<<endl;
}

//________________________________________________________________________________________________________________________________________

void goto_recursive( char *temp_path[] )
{

	DIR *directory ; 
	string path(home);
	
	path = path + temp_path[1];

	strcat(now_path[path_counter] , &path[0]);

	directory = opendir(now_path[path_counter]);

	chdir(now_path[path_counter]);
	
	if(directory == NULL)
		cout<<"Directory cannot be opened ";

	int count_of_entries;	

	count_of_entries = get_num_entries(now_path[path_counter]);
				  	    					
	struct information ptr[count_of_entries];
	display( count_of_entries , ptr);

	closedir(directory);
}


