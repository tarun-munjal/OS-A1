#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include "keystrokes.h"

struct information
{

char *name ;
char permissions[11] ;
char *last_modified ;
char *uname ;
char *gname ;
double size;
struct information *next ;
struct information *previous;
double total_size ; 
};


void get_permissions( struct information pointer[]);

void display(int , struct information ptr[]);

int get_num_entries();
