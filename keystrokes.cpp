#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "keystrokes.h"
#include "get_display.h"

using namespace std ;
static struct termios  term, oterm;

extern string home;
//char home[20] ;
//char now_path[100];

using namespace std;
void function_to_store_in_termios_getchar()
{
   //termios type of structure to store the old standard form of the terminal 
  	

	int character; // To input the keystroke 

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




void display(int count_of_entries , struct information pointer[] , char *now_path)
{

 	int arrow = 0;
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

		  	    if(c == '\n')
		  	    	{
		  	    		if(pointer[arrow].permissions[0] == 'd')
		  	    				{
		  	    					strcat(now_path , "/") ;
		  	    					strcat(now_path , pointer[arrow].name);

		  	    					count_of_entries = get_num_entries(now_path);
		  	    					struct information ptr[count_of_entries];
									arrow = 0;
									get_permissions( pointer, now_path);
		  	    				}

		  	    	}

		  	    if(c=='H'||c=='h')
		  	    				{

		  	    					count_of_entries = get_num_entries(&home[0]);
		  	    					arrow = 0;
		  	    					struct information ptr[count_of_entries];
									get_permissions( pointer , &home[0]);
		  	    				}

			    if(c== 'A' && arrow>0)  
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
