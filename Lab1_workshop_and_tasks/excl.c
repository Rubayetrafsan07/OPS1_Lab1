#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc , const  char ** argv)
{
    if(argc!=2)
    {
        fprintf(stderr, "No file name provided!\n");
        return 1;
    }

    int fd = open(argv[1], O_CREAT | O_EXCL |O_WRONLY , 0666); // O_EXCL — when combined with O_CREAT, fail if the file already exists.
    if(fd<0)
    {
        perror("open()");
        return 1;
    }

    getchar();

    close(fd);
    unlink(argv[1]);   // removes the filename from the directory.
    return 1;

}