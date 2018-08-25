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
#include "include.h"
#include "keystrokes.h"
using namespace std;


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




int get_num_entries(char* now_path)
{
		
	DIR *directory ; 
	struct dirent *details_dirent;
	directory = opendir(now_path);

	
	if(directory == NULL)
	{
		printf("Error : Unable to open directory.\n");
		exit(1);
	}	


	struct stat stat_structure;
		
			char *temp_c;
			int num_entries = 0;
			while(	(details_dirent = readdir(directory)) != NULL	)
			{	
				string temp ; 
				temp = now_path; 
				temp = temp + "/" + (details_dirent->d_name);
				temp_c = &temp[0];


				/*return data <sys/stat> by stat function*/
				if(	stat(temp_c , &stat_structure ) == 0 )
					num_entries++;

			}
cout<<"Entries"<<num_entries<<endl;
	return num_entries;

}




void get_permissions( struct information store_info[] , char *now_path)
{

	printf("\033[2J");
	printf("\033[H");


	int count_items =0;
	cout.precision(3);	
	DIR *directory ; 

	//Dirent structure to find the name of the file where stat doesn't contains the
	struct dirent *details_dirent;
	struct passwd *uid;
	struct group *gid;
	
	
	directory = opendir(now_path);
	
	if(directory == NULL)
	{
		printf("Error : Unable to open directory.\n");
		exit(1);
	}	
 	
 	//stat structure to find all the file attributes from sys/stat
		struct stat stat_structure;
		
			char *temp_c;
			while(	(details_dirent = readdir(directory)) != NULL	)
			{	
				string temp ; 
				temp = now_path; 
				temp = temp + "/" + (details_dirent->d_name);
				temp_c = &temp[0];

				char permissions_array[11];

				/*return data <sys/stat> by stat function*/
				if(	stat(temp_c , &stat_structure ) == 0 )
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

			
					for(int j=0 ; j<10 ; j++)
						cout<<store_info[count_items].permissions[j];
			
					if(store_info[count_items].size>1024 && store_info[count_items].size<1048576 )
							cout<<setw(6)<<store_info[count_items].size/1024<<"K";

					else if(store_info[count_items].size>1048576)
							cout<<setw(6)<<store_info[count_items].size/1048576<<"M";

					else
							cout<<" "<<setw(6)<<store_info[count_items].size;
					
			
					//Printing UserName
					cout<<setw(10)<<store_info[count_items].uname;
					
			
					//Printing GroupName	
					cout<<setw(10)<<store_info[count_items].gname<<" ";
			
			
					//Last line of Char Time has \n so print like this and to avoid the long time 
					for(int i=4;i<=15;i++)
						printf("%c",store_info[count_items].last_modified[i]);
							
			
					//Use dirent here because name is not present in stat_structure
					printf(" %s\n",store_info[count_items].name);
			
					count_items++;
			}//if
						
		
		else
		{
			printf("Unable to print ");
		}

		}//while
		//cout<<now_path<<endl;
	
		printf("\033[%dA",count_items);

	} 
//function get_permission