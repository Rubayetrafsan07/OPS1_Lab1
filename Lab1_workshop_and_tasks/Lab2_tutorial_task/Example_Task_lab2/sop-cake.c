#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <poll.h>  
#include <sys/mman.h>


#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))




int main(int argc, char **argv)
{
    //if(argc !=3)  part 1
    if(argc <3 || argc > 7)  // part 3
    {
        fprintf(stderr, "Usage: %s recipe tower1 ... tower5\n", argv[0]);
        return 1;
    }

    char* path = argv[1] ;

    int fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0666);
    
    if(fd == -1)  // if file already exists, open() will return -1 and set errno to EEXIST.
    {
        if(errno == EEXIST)      // Since O_EXCL makes open() fail when the file exists, fd is still -1.
        {
            fd = open(path, O_RDWR ,0666); // Open the existing file again before calling lseek()
            if(fd == -1)
                ERR("open");

            off_t size = lseek(fd, 0, SEEK_END);
            if(size == -1)
                ERR("lseek");

            if(size != 2048)
            {
                fprintf(stderr, "this Book not a recipe!\n");
                close(fd);
                return EXIT_FAILURE;
            }
        }
        else
        {
            ERR("open:");
        }
    }
    else
    {
        char buf[64];
        memset(buf, ' ', 63);
        buf[63] = '\n';
    
        for(int i = 0 ; i < 32; i++)
        {
            int ret = write(fd ,buf, sizeof(buf));
            if(ret <0)
            {
                close(fd);
                ERR("write:");
            }
        }
    }
    
    //char * tower1 = argv[2]; part 1
 
     int n = argc - 2; // part 3 .  open all fifos  
    
    //  part 1
    // if (stat(tower1, &s) == -1 || !S_ISFIFO(s.st_mode))
    // {
    //     fprintf(stderr, "There is no granny in %s\n", tower1);
    //     close(fd);
    //     return EXIT_FAILURE;
    // }


    // int fd2 = open(tower1, O_RDONLY);
    // if(fd2 == -1) // if does not exists 
    // {
    //     fprintf(stderr, "this is not a granny %s\n", tower1);
    //     close(fd2);
    //     ERR("open");
    // }

    struct pollfd fds[5];   // part 3
    char messages[5][128] = {0}; // part 3
    int pos[5] = {0};   // part 3

    //char buf[64];
    //char message[128];  // part 2
    //int pos = 0;   // part 2

 
    for (int i = 0; i < n; i++)  // part 3
    {
        struct stat s;

        if (stat(argv[i + 2], &s) == -1 || !S_ISFIFO(s.st_mode))
        {
            fprintf(stderr, "There is no granny in %s\n", argv[i + 2]);
            close(fd);
            return 1;
        }

        fds[i].fd = open(argv[i + 2], O_RDONLY | O_NONBLOCK);
        if (fds[i].fd == -1)
            ERR("open FIFO");

        fds[i].events = POLLIN;
        fds[i].revents = 0;
    }


     int active = n;

    // for(;;)   // part 1
    // {
    //     int ret = read(fd2, buf, sizeof(buf));
    //     if(ret < 0)
    //     {
    //         close(fd2);
    //         ERR("read");
    //     }
    //     if(ret == 0)
    //     {
    //         break;
    //     }

        // fprintf(stdout, "%s", buf);  // read() doesn't add a null terminator ('\0'). Therefore, %s may print extra garbage or cause undefined behavior.
        //part 1
       // write(STDOUT_FILENO, buf, ret);  // write to stdout , Used write() with the exact number of bytes read.

    while (active > 0)  // part 3
    {
        int ready = poll(fds, n, 3000);

        if (ready == -1)
        {
            if (errno == EINTR)
                continue;
            ERR("poll");
        }

        if (ready == 0)
        {
            printf("Grannies are pouring more tea...\n");
            continue;
        }

        for (int i = 0; i < n; i++)
        {
            if (fds[i].fd == -1)
                continue;

            if (fds[i].revents & (POLLIN | POLLHUP))
            {
                char buf[64];
                ssize_t ret = read(fds[i].fd, buf, sizeof(buf));

                if (ret == -1)
                {
                    if (errno == EAGAIN || errno == EINTR)
                        continue;
                    ERR("read");
                }

                if (ret == 0)
                {
                    close(fds[i].fd);
                    fds[i].fd = -1;  // inactivate the fifo. poll() ignores entries whose descriptor is negative.
                    active--;

                    printf("Granny from %s finished dictating\n", argv[i + 2]);
                    continue;
                }
                //part 2  later updated to part 3
                for (int j = 0; j < ret; j++)
                {
                    if (buf[j] == '\n')
                    {
                        messages[i][pos[i]] = '\0';

                        int step;
                        char content[128];
                    
                         // sscanf () reads from a string already stored in memory. and stores the exracted values into the provided variables.

                        if (sscanf(messages[i], "%d:%127[^\n]", &step, content) == 2 && step >= 0 && step <= 31)  // %127[^\n]	Read up to 127 characters, stopping at newline ,we did not use %s because %s stop at whitesapce
                                    // we check If both conditions are true, sscanf() should return 2 and step should be [0-31].  we save the step.
                        {
                            char line[64];   // line is 64 cz Our recipe file requires every line to contain exactly 64 bytes.
                            memset(line, ' ', 63);
                            line[63] = '\n';

                            int len = strlen(content);
                            if (len > 63)
                            len = 63;

                            memcpy(line, content, len);

                            if(lseek(fd, step * 64, SEEK_SET) ==-1)  // each recipe line is 64 byets , so if we want to put the step in it postion we multiply the step by 64 to get the correct offset in the file.
                                      // ie. for step 07 -> offset = 7 * 64 = 448 bytes. so we start to write  from 448 bytes.
                            {
                                ERR("lseek");
                            }                            
                            if(write(fd, line, 64)!=64)
                            {
                            ERR("write");
                            }

                            printf("Step %02d saved\n", step); 
                        }
                        else
                        {
                        fprintf(stderr, "Granny got carried away: %s\n", messages[i]);
                        }

                        pos[i] = 0;
                    }
                    else if (pos[i] < 127)
                    {       
                        messages[i][pos[i]++] = buf[j];
                    }   
                }
                    
            }
        }
    }

    /// stage 4 :
    char *recipe = mmap(NULL, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (recipe == MAP_FAILED)
        ERR("mmap");

    // Replace raw~berries with raspberries
    int count = 0;

    for (int i = 0; i <= 2048 - 11; i++) //If we start too close to the end of the file, we might try to read beyond its 2048 bytes. Therefore, the last possible starting position is: 2048-11= 2037
    {
        if (memcmp(recipe + i, "raw~berries", 11) == 0)
        {
            memcpy(recipe + i, "raspberries", 11);
            count++;
        }
    }
    
     // Print all non-empty recipe steps
    int missing = 0;

    for (int i = 0; i < 32; i++)
    {
        char *line = recipe + i * 64;
        int len = 63;

        // Ignore padding spaces
        while (len > 0 && line[len - 1] == ' ')
            len--;

        if (len == 0)
        {
            missing++;
        }
        else
        {
            printf("%02d: %.*s\n", i, len, line);
        }
    }

    if (missing > 0)
        printf("Missing %d steps, the cake will collapse!\n", missing);
    else
        printf("Cake ready!\n");

    munmap(recipe, 2048);


    close(fd);
    //close(fd2); part 1
    return 0 ;
}