This is a terminal based File Explorer programmed in C/C++ only. This is made without the use of any Shell Command's. "System" library function is also not used in prgramming of this terminal.

Various System libraries like ncurses , boost and many others are also not used.

NOTE :  a.) You will can go back to normal mode from command mode by pressing ESC , Up , Down , Left , Right arrow keys 
	b.) Search results displayed cannot be navigated . 
	c.) No relative paths will work. You will have to mention the absolute path 
	d.) Don't forget to mention "/" before you enter a path 
	e.) If you want to do work through commands in your home directory then just add "/" to the source/destination path where you want to work. 

This File Explorer is made using 2 modes. 
1.Normal Mode : Where you can explore all the files , folders that are present in the system. 
2.Command Mode : Where you can type the command and just like that your work is done which is preferable by the way .

NORMAL MODE : 
Functionalities of Normal Mode are  : 

1. Each line in the directory is shown (One entry per line) : 
	a.)Each line shows various details of that entry as follows :
		i.)   File Name
		ii.)  File size (Human readable)
		iii.) Ownership (User & Group)
		iv.)  Last modified
		v.)   Permissions
	b.) "." is Current Directory 
	c.) ".." is Parent Directory 
	d.) You can navigate all the files and folders through the up and down navigate keys .

2.When you press Enter then the corresponding file/folder will be opened.
	a.)If corresponding entry is a folder then all the previous details will be cleared and you will be displayed with all the files and folders 		   inside that folder in the format mentioned above in 1.
 	b.)If corresponding entry is file then that file will be opened with it's default application.

3. You can traverse through the file(s)/folder(s) listed through the navigation keys as follow :
	a.)If back arrow key is pressed , then you will be redirected to the previous folder where you went before visiting this folder. If you have   		   started the application then you wouldn't be able to navigate to the previous folder.
	b.)If right arrow key is pressed ,then if you have pressed any back arrow key only then you will be able to go to the folder where you were 		   before hitting the back arrow key.
	c.)If you press the backspace key , then you will go to parent directory of the current directory.If you come home by pressing the backspace 		   key then you will not be able to go to the parent directory.
	d.)If you hit the "h/H" key you will go to home directory and all the previous history will be deleted i.e. you wouldn't be able to traverse 		   through the navigation keys once you press the home key button .
	e.)If you want to exit the File Explorer then you can press "q/Q" key. you will exit the application and will enter the terminal of your 		   system. 

4. If the number of files in your system are more than the current number of rows of the terminal then you will be able to scroll down to visit rest of files in the present directory . Similarily , if you have scrolled down to visit the entries then you can scroll up to visit the other file(s)/folder(s).

5.Scrolling due to the overflow of the file(s) can be done by the up and down arrow key's.

To switch from Normal Mode to Command Mode you can press ":"

COMMAND MODE : 
Functionalities of Command Mode are :

1. All the commands that you will type will be displayed to you on the STATUS BAR at the bottom of the terminal.

2. The functions are available for you in command mode are as follows : 
	i.)    Copy 
	ii.)   Move
	iii.)  Rename
	iv.)   Create file
	v.)    Create Directory
	vi.)   Delete file
	vii.)  Goto 
	viii.) Search 
	ix.)   Snapshot

3. Details of all the above mentioned commands are as follows : 

a. COPY : To copy a single file or folder or multiple files or folders to a specific destination can be done using the copy command. 
	Syntax : copy foo.txt bar.txt baz.mp4 /foobar	
		Where foo.txt bar.txt baz.mp4 are the source files that are to be copied . 
		/foobar is the destination which means that the file/folders will be copied to the home directory .
		You can't mention "~". 

b. MOVE : To move a single file or folder or multiple files or folders to a specific destination can be done using move command. 
	Syntax : move foo.txt bar.txt baz.mp4 ~/foobar
		This will move the source files that are foo.txt bar.txt baz.mp4 to the specified destination folder/foobar.
		You will have to mention the absolute path from home to your destination. Relative path "~" is not allowed just like 			the copy command.

c. RENAME : You can rename your file through this command.	
	Syntax : rename oldfilename newfilename
		This will the rename you oldfilename to the newfilename that you have mentioned above. 

d. CREATE FILE : Create your file using this command .
	Syntax : create_file filename destination_path 


e. CREATE_DIRECTORY : Create your directory using this command .
	Syntax : create_dir directory_name destination_path 
		Just like the create file command this will create a new directory for you.

f. DELETE_FILE : It will simply delete a file.
	Syntax : delete_file path_of_file

g. GOTO : This as suggested by the name will go to the directory mentioned by you as an input 
	Syntax : goto path_name

h. SEARCH : It will search every file and folder from where you have started the application and it will display the search results.
	Syntax : search filename
	It will not go back to the previous folder if pressed back button instead it will go back to the home of the application.

i. SNAPSHOT : It will snapshot all the folder files everything you have after home directory and save it in a file in your home directory.
	Syntax : snapshot directory.


	
 































			


