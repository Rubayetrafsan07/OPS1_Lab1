#include <stdio.h>

int main(int argc , char **argv)
{
    FILE *f = stdin;    // either read from keyboard like (Hello world)
    if(argc>1)
    {
        f = fopen(argv[1],"r");  // or read from a text file (comments.txt)
        if(!f)
        {
            perror("foopen()");
            return 1;
        }
    }

    char buff[10]={0};

    while(!feof(f))
    {
       int ret = fscanf(f,"%9s",buff);
       printf("fscanf() returned %d, buf = \"%s\", feof(f) = %d\n", ret, buff,feof(f));
    }
    return 0;
}

// output :
// fscanf() returned 1, buf = "hello", feof(f) = 0
// fscanf() returned 1, buf = "world", feof(f) = 0
// fscanf() returned 1, buf = "this", feof(f) = 0
// fscanf() returned 1, buf = "is", feof(f) = 0
// fscanf() returned 1, buf = "Rafsan", feof(f) = 0
// fscanf() returned -1, buf = "Rafsan", feof(f) = 1   // end of file thats why 1


// we used high level lang:
//fopen()   -> 	Returns a FILE *
// Used with fscanf(), fgets(), fclose()
// part of c standard

// and other way is 
// open()  -> Returns an integer file descriptor (int)
// Used with read(), write(), close()
// POSIX function on Linux