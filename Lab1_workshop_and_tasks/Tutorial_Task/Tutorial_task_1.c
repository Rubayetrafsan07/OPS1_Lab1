#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void scan_dir()
{
    DIR* drip;
    struct dirent * dp;
    struct stat filesate;
    int dirs = 0 , files=0, links=0, others=0;

    if((drip = opendir("."))== NULL)
    {
        ERR("opendir");
    }

    do
    {
        errno = 0;

        if((dp= readdir(drip))!=NULL)
        {
            if(lstat(dp->d_name,&filesate))
                ERR("lstat");

            if(S_ISDIR(filesate.st_mode))
            {
                dirs++;
            }
            else if(S_ISREG(filesate.st_mode))
            {
                files++;
            }
            else if(S_ISLNK(filesate.st_mode))
            {
                links++;
            }
            else
               others++;
        }

    } while(dp != NULL);

    if( errno!=0)
    {
        ERR("readdir");
    }
    if(closedir(drip))
    {
        ERR("closedir()");
    }
     printf("Files: %d, Dirs: %d, Links: %d, Other: %d\n", files, dirs, links, others);

    
}

int main(int argc , char **argv)
{
    scan_dir();
    return EXIT_SUCCESS;
}

// output :
// Files: 2, Dirs: 2, Links: 0, Other: 0

// why 2 dirs :

// Tutorial_Task/
// ├── .      // current dirs
// ├── ..     // parent dirs    
// ├── task1 
// └── Tutorial_task_1

//Remember: .. is an entry pointing to the parent directory; your program doesn't scan the parent's contents.