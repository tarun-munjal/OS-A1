#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "keystrokes.h"
#include "get_display.h"


using namespace std;
int function_to_store_in_termios_getchar()
{
   //termios type of structure to store the old standard form of the terminal 
  	static struct termios  term, oterm;

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

	//get character

	character= getchar();

	//set attribute immediately in oterm 

        tcsetattr(0, TCSANOW, &oterm);


	return character;

}

void keystrokes(int count , struct information pointer[])
{

	printf("\033[2J");
	printf("\033[H");
	display(count , pointer);
	printf("\033[1A");
	

	int arrow = count;
 	//till the user pess CTR-D run till that point
	while (1)
    	{
			int c ;
			 c = function_to_store_in_termios_getchar();
		   
		          if (c == '\004')  // Ctr-D to exit 
		          break;
		  	
			  if(c== 'A' && arrow>0)  
			  		{
			  			printf("\033[1A"); // for up arrow key
			  			arrow--;
			  		} 
		  	  else if(c== 'B' && arrow < count-1)  
		  	  		{
		  	  			printf("\033[1B"); // for down arrow key
		  	  			arrow++;
		  	  		} 
		}

	printf("\033[2J");
	printf("\033[H");

}
