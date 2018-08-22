#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <dirent.h>
//To know the name of file present in dirent structure of readdir();

int main(int argc , char *argv[])
{
	DIR *directory ; 
	struct dirent *details_dirent;
 
	directory = opendir(".");
	if(directory == NULL)
	{
		printf("Error : Unable to open directory.\n");
		exit(1);
	}	

	while(	(details_dirent = readdir(directory)) != NULL	)
	{
		printf(">>%s\t%d\n",details_dirent->d_name,details_dirent->d_reclen);
	}

	closedir(directory);
	

return 0 ;
} 
