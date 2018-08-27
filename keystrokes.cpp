#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "keystrokes.h"
#include "get_display.h"
#include <stack>
#include <vector>
#include <sys/ioctl.h>

using namespace std ;
static struct termios  term, oterm;

extern char home[100];
extern int path_counter;
extern char now_path[100][100];

extern int max_counter ;
using namespace std;

int top_pointer = 0  ;
int bottom_pointer ;

void function_to_store_in_termios_getchar()
{
   //termios type of structure to store the old standard form of the terminal 
  	
	//getting the old attributes of the terminal 

	tcgetattr(0, &oterm);

	//copy the terminal attributes in new termios type of structure term 

	memcpy(&term, &oterm, sizeof(term));

	//negating the values of ICANON and ECHO 
	//ICANON to enter into non canonical mode
	//Echo to not showing the value at that point of time  

	term.c_lflag &= ~(ICANON | ECHO);

	// VMin characters have been recieved with no more data available 	
        term.c_cc[VMIN] = 1;

	//Vtime tenths of a second elapses between bytes of character arrives 
	//in our case 0 tenths of second cz we have 1 character 

	term.c_cc[VTIME] = 0;

	//Set attribute to immediately by tcsanow in term 

        tcsetattr(0, TCSANOW, &term);

}




void display(int count_of_entries , struct information pointer[] )
{
	
 	int arrow = 0;
 	int flag = 0 , check = 0;
 	get_permissions(pointer , now_path[path_counter++]);


	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int rows , cols ;
    rows = w.ws_row;
    cols = w.ws_col;

 	while (1)
    	{

			char c ;

				c= getchar();

		          if (c == '\004')  // Ctr-D to exit 
		          {	

		          	printf("\033[2J");
		          	printf("\033[H");
			    	tcsetattr(0, TCSANOW, &oterm);
					exit(0) ;
		          }

		  	    else if(c == '\n')
		  	    	{
		  	    		//if(pointer[arrow].permissions[0] == 'd')
		  	    		//		{
		  	    						char temp[40];
		  	    					//	strcpy(temp , now_path[path_counter-]);
			  	    				
				  	    				getcwd(temp , sizeof(temp));
			  	    					
			  	    					strcat(temp , "/") ;
			  	    					
			  	    					strcat(temp , pointer[arrow].name);
			  	    				
			  	    				int i = 0;
			  	    				//while( i < max_counter )
			  	    					if( strcmp(temp ,now_path[path_counter-1])== 0)
			  	    					//{
			  	    						check = 1;
			  	    						//break; 
			  	    					//}
			  	    				if(check != 1)
			  	    				{

			  	    					max_counter = path_counter +1;
//			  	    					path_counter = path_counter - 1;
			  	    					getcwd(now_path[path_counter] , sizeof(now_path[path_counter]));
			  	    					strcat(now_path[path_counter] , "/") ;
			  	    					strcat(now_path[path_counter] , pointer[arrow].name);
			  	    				}	
			  	    				else 
			  	    					{

			  	    						path_counter = path_counter-1;
		  	    						}
		  	    					


			  	    				//}
		  	    				 	//printf("\033[2J");
									//printf("\033[H");
									
									
									count_of_entries = get_num_entries(now_path[path_counter]);
		  	    					
		  	    					struct information ptr[count_of_entries];
		  	    					arrow=0;
									
									display( count_of_entries, ptr);
						
						}

						else if(c == 127)
								{
									if(path_counter >1 )
									{	
		  	    					getcwd(now_path[path_counter] , sizeof(now_path[path_counter]));
		  	    					strcat(now_path[path_counter] , "/") ;
		  	    					strcat(now_path[path_counter] , "..");
									chdir(now_path[path_counter]);
		  	    					
		  	    					count_of_entries = get_num_entries(now_path[path_counter]);//p);//now_path);
		  	    					
		  	    					struct information ptr[count_of_entries];
									arrow = 0;
									
									display( count_of_entries , ptr);//p);//now_path);
									}
								}

		  	    else if(c=='H'||c=='h')
		  	    				{
		  	    					strcpy(now_path[path_counter],home);
									count_of_entries = get_num_entries(now_path[path_counter]);
		  	    					arrow = 0;
		  	    					struct information ptr[count_of_entries];
									display( count_of_entries , ptr );
		  	    				}

		  	    else if(c == 'D') // left
		  	    			{

		  	    				if( path_counter >1)
		  	    				{
		  	    					flag = 1; 
		  	    					path_counter -= 2;
		  	    					chdir(now_path[path_counter]);
		  	    					count_of_entries = get_num_entries (now_path[path_counter ]);
		  	    					arrow = 0;
		  	    					struct information ptr[count_of_entries];
		  	    				
		  	    					display(count_of_entries , ptr );
		  	    				}
		  	    				
		  	    			}
		  	    else if(c== 'C') //right 
		  	    			{
		  	    				if(path_counter < max_counter )
		  	    				{
//		  	    				path_counter += 1;
		  	    				if(path_counter )
		  	    				count_of_entries = get_num_entries (now_path[path_counter]);
		  	    				arrow = 0;
		  	    				struct information ptr[count_of_entries];

		  	    				display(count_of_entries , ptr );
		  	    				}
		  	    			}


			    else if(c== 'A' && arrow>0)  
			  		{

			  			printf("\033[1A"); // for up arrow key
			  			arrow--;

			  		} 
		  	    else if(c== 'B' && arrow < count_of_entries-1)  
		  	  		{
		  	  			printf("\033[1B"); // for down arrow key
		  	  			arrow++;
		  	  		} 
		}

	printf("\033[2J");
	printf("\033[H");

}
