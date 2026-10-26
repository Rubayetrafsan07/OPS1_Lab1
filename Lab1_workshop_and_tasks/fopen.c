#include <stdio.h>
#include <unistd.h>

int main(int argc, const char ** argv)
{
    printf("pid = %d\n", getpid());

    FILE * f = fopen("fopen.txt","w");
    if(!f)
    {
        perror("fopen()");
        return 1;
    }

    printf("fileno(f) = %d\n",fileno(f));
    printf("fileno (stdout) = %d\n",fileno(stdout));

    getchar();

    fprintf(f," Hello form the process %d\n",getpid());
    fprintf(stdout,"hello form process %d\n", getpid());
    getchar();

    fclose(f);
    fclose(stdout);   // we already close the stdout here  . 
                     // stdout , stdin ,stderr are like file stream like FILE*f
                     // are are open automatically when u run a program. 

    getchar();
    
    printf("this is should fail\n"); // for that this will not work ,because  stdout is closed
    
    fprintf(stderr,"this should shown and error msg\n");  // here stderr is not close that why it is printed .


    return 0;

}


//output:

// pid = 3874
// fileno(f) = 3
// fileno (stdout) = 1

// hello form process 3874


// this should shown and error msg