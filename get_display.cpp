#include <iostream>
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
#include <cstring>
#include "get_display.h"
#include "keystrokes.h"
#include <unistd.h>
#include <vector>
#include <string>
#include <sys/ioctl.h>

using namespace std;

extern int path_counter;
extern int max_counter;
extern int top_pointer ; 
extern int bottom_pointer ;
extern int arrow;
extern int col;
extern int row;
extern int move_to_command;
extern char now_path[100][100];

struct information * insert(struct information *node , struct information *head )
{
	if(head == NULL )
		head = node ;
	else
	{
		struct information *temp_node ; 
		temp_node = head ; 
		while(temp_node -> next != NULL)
			temp_node = temp_node -> next;
		temp_node ->next = node ;
	}
		return head ;  
}

double get_size(struct stat stat_structure)
{
	return stat_structure.st_size;
}




int get_num_entries(char *now_path)//char* now_path)
{
		
	DIR *directory ; 
	struct dirent *details_dirent;
	directory = opendir(now_path);

	
	if(directory == NULL)
	{
		printf("Error : Unable to open directory ");
		return 0;
	}	


	struct stat stat_structure;
		
			int num_entries = 0;

			while(	(details_dirent = readdir(directory)) != NULL	)
			{	
				//to append the name of file/directory encountered now with the previous path now_path
				string appended ; 
				appended = now_path; 
				appended = appended + "/" + (details_dirent->d_name);
				//temp_c = &temp[0];


				/*return data <sys/stat> by stat function*/
				if(	stat(&appended[0] , &stat_structure ) == 0 )
					num_entries++;

			}
	return num_entries;

}




void get_permissions( struct information store_info[] , char * now_path)//char *now_path)
{

	printf("\033[2J");
	printf("\033[H");




	//cout<<now_path;
	int count_items =0;
	cout.precision(1);
	cout.setf(ios::fixed);	
	DIR *directory ; 

	//Dirent structure to find the name of the file where stat doesn't contains the
	struct dirent *details_dirent;
	struct passwd *uid;
	struct group *gid;
	
	chdir(now_path);
									
				
	if(top_pointer!=0)
			{
				overflow_display(count_items , store_info);										
				return;
			}

	directory = opendir(now_path);
	if(directory == NULL)
	{
		printf("Error : Unable to open directory Here.\n");
		return ;
	}	
 	
 	//stat structure to find all the file attributes from sys/stat
		struct stat stat_structure;
			while(	(details_dirent = readdir(directory)) != NULL  && arrow < bottom_pointer)
			{	
				string appended ; 
				appended = now_path; 
				appended = appended + "/" + (details_dirent->d_name);
				
				char permissions_array[11];

				/*return data <sys/stat> by stat function*/
				if(	stat(&appended[0] , &stat_structure ) == 0 )
				{	
					
						//mode_t type of perm i.e. bit type . st_mode gives all the bits of permissions
						mode_t mode = stat_structure.st_mode;
					

						if((mode & S_IFMT) == S_IFDIR) 
							permissions_array[0] = 'd'; 
						else
							permissions_array[0] = '-';
					

						// mode bit from stat structure is and'ed with the bit of user, group, all to know the permissions 
						permissions_array[1] = (mode & S_IRUSR) ?'r':'-';
						permissions_array[2] = (mode & S_IWUSR) ?'w':'-';
						permissions_array[3] = (mode & S_IXUSR) ?'x':'-';
						permissions_array[4] = (mode & S_IRGRP) ?'r':'-';
						permissions_array[5] = (mode & S_IWGRP) ?'w':'-';
						permissions_array[6] = (mode & S_IXGRP) ?'x':'-';
						permissions_array[7] = (mode & S_IROTH) ?'r':'-';
						permissions_array[8] = (mode & S_IWOTH) ?'w':'-';
						permissions_array[9] = (mode & S_IXOTH) ?'x':'-';
						permissions_array[10] = '\0';
					//	double size ;
						//function to get size of the file 
					//	size = get_size(stat_structure);
						
						//User Name from pwd structure in pwd.h by uid 			
						uid = getpwuid(stat_structure.st_uid);
						
						//Group Name from grp structure in grp.h by gid			
						gid = getgrgid(stat_structure.st_gid);

						//Time from stat_structure	
						char *time ; 
						//To convert time into string
						time = ctime(&stat_structure.st_mtime);
						
						store_info[count_items].name = details_dirent->d_name;
						for(int i=0; i<10 ; i++)
						{
							//Storing information	
							store_info[count_items].permissions[i] = permissions_array[i];
						}
						store_info[count_items].size = get_size(stat_structure) ;
						store_info[count_items].uname = uid->pw_name; 
						store_info[count_items].gname = gid->gr_name;
						store_info[count_items].last_modified = time ;
						
					count_items++;
					}


					else
						{
							printf("Unable to print ");
						}
				}
						display_list( count_items, store_info );

}


void display_list(int count_items , struct information store_info[])
{
		if(move_to_command == 0)
			{	
				int i = top_pointer;
				for(  ; i < count_items && i < bottom_pointer -1 ; i++)
				{
							for(int j=0 ; j<10 ; j++)
									//cout<<"he";
									cout<<store_info[i].permissions[j];
									
							if(store_info[i].size>1024 && store_info[i].size<1048576 )
									cout<<setw(6)<<store_info[i].size/1024<<"K";
		
							else if(store_info[i].size>1048576)
									cout<<setw(6)<<store_info[i].size/1048576<<"M";
		
							else
									cout<<" "<<setw(6)<<store_info[i].size;
							
					
							//Printing UserName
							cout<<setw(10)<<store_info[i].uname;
							
					
							//Printing GroupName	
							cout<<setw(10)<<store_info[i].gname<<" ";
					
					
							//Last line of Char Time has \n so print like this and to avoid the long time 
							for(int k=4 ; k<=15 ; k++)
								printf("%c",store_info[i].last_modified[k]);
									
					

							//Use dirent here because name is not present in stat_structure
		//					if(strlen(store_info[i].name)<col-2)
							
							cout<<" ";
							int len = strlen(store_info[i].name);
							if(len <=20)
								cout<<setw(20)<<store_info[i].name<<endl;
							else
								{
									for(int p = 0 ; p<19 ; p ++)
									cout<<store_info[i].name[p];
									cout<<".."<<endl;
								}
				
					}
				
					printf("\033[%dB", bottom_pointer );//- count_items);
						cout <<setw(40)<<"\033[1;31mSTATUS BAR                                 PRESS : FOR COMMAND MODE\033[0m" ;//               : FOR NORMAL MODE "; 
				}	
				else
				{
					//to print Enter command at bottom 
					printf("\033[%dB", bottom_pointer );//- count_items);
					//to clear one line
					printf("\033[2K");

					//call a function for command mode 
				}


				printf("\033[%dA",count_items);
				printf("\033[H");
}



	
	
				//	count_items++;
			//if

		

		//while

		
		
//function get_permission

	void overflow_display(int count_items , struct  information store_info[])
	{
		if(move_to_command == 0)	
			{
				printf("\033[2J");
				printf("\033[H");
	//			printf("\033[%dA",bottom_pointer);
	//		int i = top_pointer;
	//		bottom_pointer = arrow ;

	//						cout<<bottom_pointer<<endl;
		//	printf("\33[%dD",col);
				int i=top_pointer;

				for(; i <count_items && i < bottom_pointer -1  ; i++)// && i < count_items)
						{
							for(int j=0 ; j<10 ; j++)
									//cout<<"he";
									cout<<store_info[i].permissions[j];
									
							if(store_info[i].size>1024 && store_info[i].size<1048576 )
									cout<<setw(6)<<store_info[i].size/1024<<"K";
		
							else if(store_info[i].size>1048576)
									cout<<setw(6)<<store_info[i].size/1048576<<"M";
		
							else
									cout<<" "<<setw(6)<<store_info[i].size;
							
					
							//Printing UserName
							cout<<setw(10)<<store_info[i].uname;
							
					
							//Printing GroupName	
							cout<<setw(10)<<store_info[i].gname<<" ";
					
					
							//Last line of Char Time has \n so print like this and to avoid the long time 
							for(int k=4 ; k<=15 ; k++)
								printf("%c",store_info[i].last_modified[k]);
									
					

							//Use dirent here because name is not present in stat_structure
							cout<<" ";
							int len = strlen(store_info[i].name);
							if(len <=20)
								cout<<setw(20)<<store_info[i].name<<endl;
							else
								{
									for(int p = 0 ; p<18 ; p ++)
									cout<<store_info[i].name[p];
									cout<<".."<<endl;
								}

						}
						if(move_to_command==0 )
							cout<<"\033[1;31mSTATUS BAR                                 PRESS : FOR COMMAND MODE\033[0m";
		
				}
		
	}


	//void command_mode_enter()