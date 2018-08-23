#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>


int func()
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
	c= getchar();

	//set attribute immediately in oterm 
        tcsetattr(0, TCSANOW, &oterm);
	return c;

}

int main()
{

 	//till the user pess CTR-D run till that point
	while (1)
    	{
		
	int c ;
	 c = func();
   
          if (c == '\004')  // Ctr-D to exit 
          break;
  	
	  if(c== 'A')  printf("\033[1A"); // for up arrow key 
  	  if(c== 'B')  printf("\033[1B"); // for down arrow key 
	  if(c== 'C')  printf("\033[1C"); // for right arrow key 
	  if(c== 'D')  printf("\033[1D"); // for left arrow key 

   }

	
return 0 ;	
    }
