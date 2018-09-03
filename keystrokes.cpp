#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "keystrokes.h"
#include "get_display.h"
#include "command_mode.h"
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
int arrow = 0;
int col;
int row;
int move_to_command = 0;
int fix;
string search_result[10];

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
	
 	
 	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

//    int rows , cols ;
    row = w.ws_row;
    bottom_pointer = w.ws_row;
    col = w.ws_col;
    fix = (69/col)+1;

 	int flag = 0 , check = 0;
 	get_permissions(pointer , now_path[path_counter++]);



 	while (1)
    	{

			char c ;

				c= getchar();

		          if (c == 'q' || c == 'Q')  // Ctr-D to exit 
		          {	

		          	printf("\033[2J");
		          	printf("\033[H");
			    	tcsetattr(0, TCSANOW, &oterm); //Set attribute after exit for safeside brother.
					exit(0) ;
		          }

		  	    else if(c == '\n') // If you press enter then you will be in this block 
		  	    	{
		  	    			if(move_to_command == 0)	// this is kind of a mode bit for normal and command mode 
		  	    				{	
		  	    					if(pointer[arrow].permissions[0]=='d' )
		  	    					{
		  	    					
			  	    					if((strcmp(pointer[arrow].name,"..")==0) && strcmp(now_path[path_counter-1],home)==0) // To check the condition so that you don't go back from where the application started
			  	    						{	
			  	    							//Go to home page if you press '..'
			  	    							count_of_entries = get_num_entries(home); 
				  	        					struct information ptr[count_of_entries];
					  	    					arrow=0;
					  	    					get_permissions(ptr , home);
		  	    							}
										else
											{
												//If you don't press '..' go here
												top_pointer = 0;
				  	    						bottom_pointer = w.ws_row;
				  	    						char temp[40]; 
				  	    						getcwd(temp , sizeof(temp));
					  	    					strcat(temp , "/") ;
					  	    					strcat(temp , pointer[arrow].name);


					  	    				int i = 0;
					  	    					if( strcmp(temp ,now_path[path_counter])== 0)
					  	    						check = 1;

					  	    				if(check != 1)
					  	    				{

					  	    					max_counter = path_counter +1;
					  	    					getcwd(now_path[path_counter] , sizeof(now_path[path_counter]));
					  	    					strcat(now_path[path_counter] , "/") ;
					  	    					strcat(now_path[path_counter] , pointer[arrow].name);

					  	    				}	
					  	    				
					  	    				//All of the above conditions is to make sure that left arrow key works properly.

											count_of_entries = get_num_entries(now_path[path_counter]);
				  	    					
				  	    					struct information ptr[count_of_entries];
				  	    					arrow=0;
											display( count_of_entries, ptr);

											}//else
									}
									else
									{
										string temp ; 
										strcat(&temp[0] , now_path[path_counter-1]);
										strcat(&temp[0] , "/");
										strcat(&temp[0] , pointer[arrow].name);
										int pid ; 
										pid = fork();
										if(pid == 0)
										{
											execl("/usr/bin/xdg-open" , "xdg-open" , &temp[0] , NULL );
											exit(1);
										}
									}
								}// If of mode bit = 0 ;
						}
						//Enter is till here. Go ahead for more functionalities mahn.

						else if(c == 127) // If you pressed Backspace by chance or just because you are smart
								{
									if(strcmp(now_path[path_counter-1],home)==0) // To check the condition so that you don't go back from where the application started
					  	    						{	
					  	    							//Go to home page if you press '..'
					  	    							count_of_entries = get_num_entries(home); 
						  	        					struct information ptr[count_of_entries];
							  	    					arrow=0;
							  	    					get_permissions(ptr , home);
				  	    							}

									if(move_to_command == 0)
										{
											if(path_counter >1 ) //If you are not at your home page .
												{	
													if(strcmp(now_path[path_counter-1],home)==0) // To check the condition so that you don't go back from where the application started
					  	    						{	
					  	    							//Go to home page if you press '..'
					  	    							count_of_entries = get_num_entries(home); 
						  	        					struct information ptr[count_of_entries];
							  	    					arrow=0;
							  	    					get_permissions(ptr , home);
				  	    							}

					  	    					getcwd(now_path[path_counter] , sizeof(now_path[path_counter]));
					  	    					strcat(now_path[path_counter] , "/") ;
					  	    					strcat(now_path[path_counter] , "..");
												chdir(now_path[path_counter]);
					  	    					
					  	    					count_of_entries = get_num_entries(now_path[path_counter]);
					  	    					
					  	    					struct information ptr[count_of_entries];
												arrow = 0;
												
												display( count_of_entries , ptr);
												}
										}
								}

		  	    else if(c=='H'||c=='h') //If you miss going home . For Home button
		  	    				{
		  	    				  	if(move_to_command == 0)
			  	    					{
			  	    						top_pointer = 0;
				  	    					bottom_pointer = w.ws_row;	//reset bottom and top again cz u are home again		
				  	    					path_counter =0; //and path too.
				  	    					max_counter = path_counter+1;
				  	    					strcpy(now_path[path_counter],home);
											count_of_entries = get_num_entries(now_path[path_counter]);
				  	    					arrow = 0;
				  	    					struct information ptr[count_of_entries];
											display( count_of_entries , ptr );
										}
								}

		  	    else if(c == 'D') // left
		  	    			{
		  	    				if(move_to_command == 0)
			  	    				{
			  	    					if( path_counter >1)
				  	    				{
				  	    					top_pointer = 0;
				  	    					bottom_pointer = w.ws_row;//reset top and bottom only this time .
				  	    					flag = 1; 
				  	    					path_counter -= 2;
				  	    					chdir(now_path[path_counter]);
				  	    					count_of_entries = get_num_entries (now_path[path_counter ]);
				  	    					arrow = 0;
				  	    					struct information ptr[count_of_entries];
				  	    				
				  	    					display(count_of_entries , ptr );
				  	    				}
				  	    			}
		  	    			}

		  	    else if(c== 'C') //right 
		  	    			{
		  	    				if(move_to_command == 0)
			  	    				{
			  	    					if(path_counter < max_counter )
					  	    				{
					  	    					top_pointer = 0;
					  	    					bottom_pointer = w.ws_row;			
					  	    					
						  	    				if(path_counter )
						  	    				count_of_entries = get_num_entries (now_path[path_counter]);
						  	    				arrow = 0;
						  	    				struct information ptr[count_of_entries];
			
						  	    				display(count_of_entries , ptr );
				  	    					}
			  	    				}
							}


			    else if(c== 'A' && arrow>0)  
			  		{
			  			if(move_to_command == 0)
				  			{
					  			printf("\033[%dA",fix); // for up arrow key
					  				arrow--;
					  			if(count_of_entries >= 0 && arrow < top_pointer && top_pointer > 0)
					  			{
					  				top_pointer--;
					  				bottom_pointer--;
					  				overflow_display(count_of_entries , pointer);
					  				printf("\033[H");
				  				}
				  			}
					}

		  	    else if(c== 'B' && arrow < count_of_entries-1 && arrow < bottom_pointer - 1 && move_to_command==0 )  
		  	  		{
		  	  			printf("\033[%dB",fix); // for down arrow key
		  	  			arrow+=fix;
			  			if(count_of_entries > bottom_pointer-1 && arrow == bottom_pointer-1 && arrow < count_of_entries)
			  			{
							top_pointer++;
			  				bottom_pointer++;
			  				overflow_display(count_of_entries , pointer);//////
			  				printf("\033[%dA",fix);
			  				printf("\033[%dD",68);
			  			}
			  		}
			  	else if(c == ':')
			  	{
			  		string command;
			  		move_to_command = 1;
		  			printf("\033[2J");
					printf("\033[H");	
				        		
			  		display_list(count_of_entries , pointer);
				    printf("\033[%dB", bottom_pointer );
			      	
				    while(1)
						{
							int check_flag = 0; 
						      	char char_cm = getchar();					      	
						      	
						      	if(char_cm  == '\033')
								    {
				    		 		    move_to_command = 0;
				    					printf("\033[2J");
						        		printf("\033[H");	
										top_pointer = 0;
						        		bottom_pointer = w.ws_row;
										arrow = 0;
					        			display_list(count_of_entries , pointer );
				  	    				break;
							        }
						        else if(char_cm  == '\n')
						        	{
						           		printf("\033[2J");
						        		printf("\033[H");	
					        			move_to_command = 0;
						        		go_for_command(command);
						        	}
						        else if(char_cm  == 127)
						        	{

						        		int len = strlen(&command[0]);
						        		if(len==0)
						        			{
						        				printf("\033[D");
								        		printf(" ");
								        		printf("\033[D");
								        		command = "\0";
						        				continue;
						        			}
						        		else if(command[len-1] != ' ')
						        			{
						        				printf("\033[D");
							        			printf(" ");
							        			printf("\033[D");
							        			command[len-1] = '\0';
							        		}
						        		else
						        		{

						      	   		printf("\033[D");
						        		printf(" ");
						        		printf("\033[D");
						        		command[len-1] = '\0';
						        		}
						        	 
						        }
						        else
						        {
						        	command += char_cm;
						        }
						        cout<<char_cm;
						}
			  	}
			  	else
			  	{}
			  	
		}
	
	printf("\033[2J");
	printf("\033[H");

}

