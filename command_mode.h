#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

void go_for_command(string command );

int check_for_dir(string sent_src_file );  //returns true if input is a directory

int check_for_file(string sent_src_file);  //returns true if input is a file not a directory

string find_src_directory(string sent_src_directory); //Returns the directory i.e. the name before '/' in the sent source file 

string find_src_file(string sent_src_directory); //Returns file name i.e. file name after '/'

//_________________________________________________________________________

void snap(string src_dir_name , string file) ;

void snap_recursive(int arg_count , char *argv[]);

//_________________________________________________________________________

void copy(char *src_filename , char *dest_filename);

void to_copy_directory(string src_file_name , string dest_file_name); // If input is a directory and we have to copy it 

void copy_recursive(int arg_count , char *argv[]);

//_________________________________________________________________________

void move(char *src_filename , char *dest_filename);

void to_move_directory(string src_file_name , string dest_file_name); // If input is a directory and we have to copy it 

void move_recursive(int arg_count , char *argv[]);

//_________________________________________________________________________

void delete_file(string to_delete_path);

void to_empty(string src_file_name ); // If input is a directory and we have to copy it 

void delet_recursive(int arg_count , char *argv[]);

//_________________________________________________________________________

void search(string src_dir_name , string search_name) ;

void search_recursive(int arg_count , char *argv[]);

//_________________________________________________________________________

void re_name( char *source , char *destination );

void rename_recursive(int argc , char *argv[]);

//_________________________________________________________________________

void create_recursive( int argc , char * argv[]);

void create_file( string dest_address , string file_name );

void create_dir (string dest_address , string dir_name);

//_________________________________________________________________________

void goto_recursive( char *path[] );

void replace_tilde(int arg_count , char *argv[]);

string find_start(string sent_src_directory);