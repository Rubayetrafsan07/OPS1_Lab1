#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

int  scan_dir(int size , FILE * fp)
{
    DIR* drip;
    struct dirent  * dp ;
    struct stat filestat;
    int sum = 0;
    int flag = 0;

    if ((drip = opendir(".")) == NULL)
        ERR("opendir");
    
    while((dp = readdir(drip))!=NULL)
    {
        if (lstat(dp->d_name, &filestat))
                ERR("lstat");
        //printf("file size : %ld\n",filestat.st_size);

         sum += filestat.st_size;
        
    }


    printf("The total  size of files: %d\n",sum); // stage 2
    fprintf(fp , "the toatl size of files:%d\n", sum); // satge 5
    
    if(sum > size)
     {
            flag = 1;  // stage 4 / we returned a flag so that we done have to pass the foler name to the fcuntion
                       // rather we print it in the main function
    }


    closedir(drip);
    return flag;
}

int main(int argc, char **argv)
{
    if(argc<2)
    {
        perror("less arrguments");
        return 1;
    }

    FILE *fp; // stage 5
    fp = fopen("output.txt", "w+"); // stage 5
    if(!fp)
    {
        ERR("open");
    }
    if( !access("output.txt" , W_OK))  // checking out the access
    {
        ERR("access");
    }

    char path[1024];
    if (getcwd(path, 1024) == NULL)
        ERR("getcwd");

    for (int i = 1 ; i <argc; i+=2)
    {
        if(chdir(argv[i]))
          ERR("chdir");
        int n = atoi(argv[i+1]);  //  do not use i++ because it will update the i value , insted use i+1 cz i will remian the same
        int R = scan_dir(n, fp);

        if(R == 1)
        {

            //printf("the name of the folder :%s\n",argv[i]); // stage 4

            fprintf(fp, " the name of the folder: %s\n", argv[i] );
        }

        if(chdir(path))
         ERR("failed to change it ");
    }
    
    return 0;
}