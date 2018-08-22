#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>


struct information
{
char *name ;
char *time ;
char permissions[10] ;
char *last_modified ;
char *uname ;
char *gname ;
double size;
};


//To know the name of file present in dirent structure of readdir();

int main(int argc , char *argv[])
{
	//Dirent type of structure cz dirent has the name and other attributes of the file 
	DIR *directory ; 
	struct dirent *details_dirent;
 	
	struct information info;

	//For username and groupname make this structure of passwd and group 
	struct passwd *uid;
	struct group *gid;


	directory = opendir(".");
	if(directory == NULL)
	{
		printf("Error : Unable to open directory.\n");
		exit(1);
	}	


	double total_size =0 ;

	while(	(details_dirent = readdir(directory)) != NULL	)
	{

		//Stat_structure to provide the size of the file 

		struct stat stat_structure;
		
		//total size 	
		if(	stat(details_dirent->d_name, &stat_structure) == 0)
		{

			total_size += stat_structure.st_size;	
	
		}//if

	}//while


	//For total size to be printed on top left corner

	if(total_size>1024 && total_size<1048576)
		printf("%.1fK\n",(total_size/1024)+1);
	else if(total_size>=1048576)
		printf("%.1fM\n",(total_size/1048576)+1);
	else
		printf("%.1f \n",total_size);



	//To open directory again from the starting because if will reach NULL by the first while loop above for total size
	
	directory = opendir(".");


	//To print ls command


	//details_dirent is getting the pointer of file from readdir
	while(	(details_dirent = readdir(directory)) != NULL	)
	{
		
		//to store permissions by stat 
		char permissions_array[10];
		
		//stat structure to find all the file attributes from sys/stat
		struct stat stat_structure;
		

	
		/*return data <sys/stat> by stat function*/
		if(	stat(details_dirent->d_name , &stat_structure ) == 0 )
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
			
			int i ;
			for(i=0 ; i< 10 ; i++)
		

			
			//Printing all the permissions 
			for( i=0; i<10 ; i++)
			{
				//Storing information	

				info.permissions[i] = permissions_array[i];
				printf("%c",permissions_array[i]);
			}


			//to store the size of the current file 
			double size;

			size = stat_structure.st_size;
			
			//Printing size of the current file 

			info.size = size;

			if(size>1024 && size<1048576 )
				printf(" %.1fK",(size/1024));
			else if(size>1048576)
				printf(" %.1fM",(size/1048576));
			else
				printf(" %.1f",size);

			//User Name from pwd structure in pwd.h by uid 			
			uid = getpwuid(stat_structure.st_uid);

			//User name to be stored in information 
			info.uname = uid->pw_name;

			//Group Name from grp structure in grp.h by gid			
			gid = getgrgid(stat_structure.st_gid);

			//Group Name to be stored in information 
			info.gname = gid->gr_name;
						
			//Printing UserName
			printf(" %s",uid->pw_name);
		
			//Printing GroupName	
			printf(" %s ",gid->gr_name);
			
	
			//Time from stat_structure	
			char *time ; 

			//To convert time into string
			time = ctime(&stat_structure.st_mtime);

			//To store time in information
			info.time = time ;

			//Last line of Char Time has \n so print like this and to avoid the long time 
			for(i=4;i<=15;i++)
				printf("%c",time[i]);
					

			//Use dirent here because name is not present in stat_structure
			printf(" %s\n",details_dirent->d_name);
				

			info.name = details_dirent->d_name ; 

			}//if
	

		
	}//Main while


	//To close the directory opened
	closedir(directory);
	

return 0 ;
} 
