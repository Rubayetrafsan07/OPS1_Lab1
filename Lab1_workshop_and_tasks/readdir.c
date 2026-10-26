#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc ,const char **argv)
{
    if(argc!=2)
    {
        printf("usage :%s <pathname>\n",argv[0]);
        return 1;
    }

    DIR *ds = opendir(argv[1]);
    if(!ds)
    {
        perror("opendir()");
        return 1;
    }

    struct dirent* entry;

    while(entry= readdir(ds))
    {
        printf("%s -> %lu \n",entry->d_name,entry->d_ino);
    }

    closedir(ds);
    return 0;
}