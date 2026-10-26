#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char **argv)
{
    if(argc!=2){
        fprintf(stderr, "Usage:<seconds>\n");
        return 1;
    }

    const char *fn = argv[1];

    int fd = open(fn, O_RDWR | O_APPEND);
        if(fd <0)
        {
            perror("fopen");
            return 1;
        }

    char c = 0;
    ssize_t n =  0;
    n = read(fd, &c,1);
    printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, n, c);

    n = read(fd, &c,1);
     printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, n, c);


     c = '!';
     n = write(fd, &c,1);

    printf("write(%d, '%c', 1) returned %ld\n", fd, c, n);

    n = read(fd, &c, 1);
  printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, n, c);
  n = read(fd, &c, 1);
  printf("read(%d, _, 1) returned %ld, c = '%c'\n", fd, n, c);
  close(fd);

    return 0;
}
