#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))


void usage()
{
    fprintf(stderr, "USAGE: \n");
    exit(EXIT_FAILURE);
}

int main(int argc , char ** argv)
{
    int c;
    char * dir_name;
    int  creation  =0;

    while((c = getopt(argc, argv , "c:v:"))!=-1 )    // or we can do "cv:" then we do not need to put anyting in terminal after -c just put -v and the name
    {   
        switch(c)
        {
            case 'c':
              creation =atoi(optarg);
              break;
            
            case 'v':
              if(creation)
              {
                    dir_name = optarg;
                    if (mkdir(dir_name, 0755) != 0)
                    {
                        if (errno != EEXIST)
                            ERR("mkdir");
                    }

                    chdir(dir_name); // remeber to change directory. to go inside the directory created.
                    FILE *f;
                    if ((f = fopen("requirments", "w+")) == NULL)
                         ERR("fopen");
                     
                   creation =0; // reset creation to 0 so that we do not create the directory again if -v is used again.      
            
              }    
                 break;
                        
            case '?':
            default:
                usage();
               
        }

    }
    return 0;
}