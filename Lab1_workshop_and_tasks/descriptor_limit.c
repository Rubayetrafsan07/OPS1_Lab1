#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char ** argv)
{
    printf("The PID : %d\n",getpid());

    int i =0;

    while(1)
    {
       // int fd = open("../descriptor_limit.txt", O_CREAT |O_WRONLY,0777); --> can be used to create file in parent directory
        int fd = open("/home/rafsan_07/OPS1_ practice(2k26)/descriptor_limit.txt", O_CREAT |O_WRONLY,0777); // same as above but with absolute path
     //   (/ -> this sign repersnt root directory of the file system) so u cannot do like /OPS1_ practice(2k26)/descriptor_limit.txt because it will look for OPS1_ practice(2k26) in root directory which is not there) 
        if(fd <0)
        {
            printf("i = %d\n",i);
            perror("open()");
            break;
        }
        i++;
 
    }
    getchar();
    return 0;

}