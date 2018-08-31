#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>

using namespace std; 

void create_file( string dest_address , string file_name )
{
	dest_address = dest_address + "/" + file_name;
	ofstream  out(&dest_address[0]);
	cout<<"File Succesfully Made "<<endl;
	cout<<"Confirm it by navigating to it in Normal Mode "<<endl;
}

void create_dir (string dest_address , string dir_name)
{
	int status ;
	dest_address = dest_address + "/" + dir_name;
	status = mkdir(&dest_address[0] , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if(status == -1)
	{
		cout<<strerror(errno)<<endl;
		return ;
	}
	cout<<"Directory Succesfully Made "<<endl;
	cout<<"Confirm it by navigating to it in Normal Mode "<<endl;
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


int main( int argc , char * argv[])
{
	if(argc < 3)
		cout<<"Too Few Arguements to Create File or a Directory";

	else
	{
//			create_dir( argv[1] , argv[2] );
			create_file( argv[1] , argv[2] );
	}
	return 0;
}