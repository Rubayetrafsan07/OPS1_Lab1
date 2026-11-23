// Write a simple program that copies files.
//It should accept two paths as arguments and copy the file from the first path to the second.

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#define FILE_BUF_LEN 256

void usage(const char* pname)
{
    fprintf(stderr,"usage: %s path_1 path_2 \n",pname);
    exit(EXIT_FAILURE);
}

int main (int argc, const char** argv)
{
    if(argc!=3)
    {
        usage(argv[0]);

    }

    const char * path_1 = argv[1];
    const char* path_2 = argv[2] ;

    int fd_1 = open(path_1 , O_RDONLY);
    if(fd_1 < 0)
    {
        ERR("open:");
    }
    int fd_2 = open(path_2, O_WRONLY | O_CREAT ,0777);
    
    if(fd_2 <0)
    {
        ERR("open");
    }

    char file_buf[FILE_BUF_LEN];

    for(;;)
    {
        int read_size = read(fd_1 , file_buf, FILE_BUF_LEN);
        if(read_size == -1)
        {
            ERR("read");

        }
        if(read_size == 0)
        {
            break;
        }

        int write_size = write(fd_2, file_buf, read_size);  // write accoding to the read_size not FILE_BUF_LEN,  
                                                            //   otherwise extra byte will be added as null byte in write.txt file after the last word .
        if(write_size ==-1)
        {
            ERR("write");
        }

    }

    if(close(fd_2) == -1)
    {
        ERR("close: ");
    }

    if(close(fd_1) == -1 )
    {
        ERR("close:");
    }

    return EXIT_SUCCESS;
}