/**
 * \brief Parent-child communication via FIFO example.
 *
 * Parent creates FIFO file and communicates through it with child process.
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FIFO_NAME "my.fifo"

int main() {

    unlink(FIFO_NAME);

    if (mkfifo(FIFO_NAME, 0666) < 0) {
        perror("mkfifo(): ");
        return 1;
    }

    int fd;

    pid_t pid = fork();
    switch (pid) 
    {
        case -1:
            perror("fork(): ");
            return 1;

        case 0: 
        {
            // child
             //sleep(3); // The child waits 3 seconds before opening the FIFO.  Meanwhile, the parent will likely reach  -> fd = open(FIFO_NAME, O_WRONLY); 
                                                                               //  and block until the child opens it.
                     
            printf("Trying to open FIFO '%s' for reading\n", FIFO_NAME);

            // TODO: Add O_NONBLOCK and observe behavior
            if ((fd = open(FIFO_NAME, O_RDONLY )) < 0) {
                perror("open(): ");
                return 1;
            }
            char buf[16];
            ssize_t ret = read(fd, buf, sizeof(buf));
            if (ret < 0) {
                perror("read()");
                return 1;
            }
            printf("read() returned %ld bytes\n", ret);
            close(fd);
            break;
        }

        default: 
        {
            // parent
             //sleep(3); //The parent waits 3 seconds before opening the FIFO. Meanwhile, the child will likely reach  -> fd = open(FIFO_NAME, O_RDONLY); 
                                                                              //and block until the parents opens it.

            printf("Trying to open FIFO '%s' for writing\n", FIFO_NAME);
            
            // TODO: Add O_NONBLOCK and observe behavior
            if ((fd = open(FIFO_NAME, O_WRONLY)) < 0) 
            {
                perror("open(): ");
                return 1;
            }

            if (write(fd, "text", 5) < 0) 
            {
                perror("write()");
                return 1;
            }

            close(fd);

            while (wait(NULL) > 0);  // wait(NULL) waits for a child process to terminate and collects its exit status. The parent uses this to ensure it has collected the child before exiting.
            break;
        }
    }

    return 0;
}