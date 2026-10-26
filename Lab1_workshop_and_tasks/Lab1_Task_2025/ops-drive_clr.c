#define _XOPEN_SOURCE 700
#include "utils.h"
#include "ctype.h"


void set(char* user, char* path, char* value) 
{
     //printf("%s SET %s to %s\n", user, path, value);

     char update[1034];

     snprintf(update , 1034 , "./drive/%s/%s",user,path);
     
     make_path(update);

     FILE * f = fopen(update,"w+");
     if(!f)
     {
        perror("fopen");
        return;
        
     }

     fprintf(f, "%s",value );

     fclose(f);

}

void get(char* user, char* path) 
{ 
     printf("%s GET %s\n", user, path);
    char update[1024];
    snprintf(update,1024 ,"./drive/%s/%s",user,path);
    
    struct stat filestat;
    if(checked_stat(update, &filestat, true, false) == -1)
    {
        fprintf(stderr,"GET: No file found at %s\n", path);
        return;
    }
    if (!S_ISDIR(filestat.st_mode))
    {
        fprintf(stderr,"GET: File is a directory at %s\n", path);
        return;
    }

    FILE *f = fopen(update, "r");
    if (f == NULL)
    {
        fprintf(stderr, "GET: Cannot read file: %s\n", path);
        return;
    }

    printf("%s %s: ", user, path);

    char buf[1024];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
    {
        fwrite(buf, 1, n, stdout);  // Because fread() does not add a terminating '\0' to the data. fwrite() prints exactly the n bytes we read, without needing a null-terminated string.
    }

    if (ferror(f))
    {
        fprintf(stderr, "GET: Cannot read file: %s\n", path);
    }
    else
    {
        printf("\n");
    }

    fclose(f);
}

void share(char* user, char* path) 
{ 
    //printf("%s SHARE %s\n", user, path); 
    checked_mkdir("./shared");
    
    char update[1024];
    snprintf(update,1024 ,"./drive/%s/%s",user,path);
    
    struct stat filestat;
    if(checked_stat(update, &filestat, true, false) == -1)
    {
        fprintf(stderr,"SHARE: No file found at %s\n", path);
        return;
    }
    // if (S_ISDIR(filestat.st_mode))
    // {
    //     fprintf(stderr,"SHARE: File is a directory at %s\n", path);
    //     return;
    // }

    char *name ;
    char * the_path = concat(2, "../", update);  // we need to go to parent directory and then concat the path to the file we want to share.
    name = get_link_name(user, path);
    
    if(chdir("./shared")==-1)  //first we change the directory where we want the symlink to be , then  we create the symlink .
    {
        perror("SHARE: chdir");
        free(name);
        free(the_path);
        return;
    }

    if(symlink(the_path, name )==-1) // the parameter is the destination of the symlink(to i.e the path)  , and the 2nd parameter is the name of the symlink file  
    {
        perror("SHARE: symlink");

        if (chdir("..") == -1)
            perror("SHARE: chdir");

        free(name);
        free(the_path);
        return;
    }

    chdir("..");
    free(name); // we have to free them 
    free(the_path); // same

}

void erase(char* user, char* path) 
{
     printf("%s ERASE %s\n", user, path); 
}

int main(int argc, char** argv)
{
    FILE * f ;
    if(argc>1)
    {
        f = fopen(argv[1],"r");
        if(!f)
        {
            ERR("fopen");
            return 0;
        } 
    }
    else
    {
        f = stdin;
    }

    char *buf;
    size_t length;
    size_t capacity= 0;

    while((length= getline(&buf,&capacity,f))!=-1)
    {
        int count;
        char ** args = split_string(buf,&count);

        buf[strcspn(buf, "\r\n")] = '\0';

        // if (count == 0)
        // {
        //     free_strings(args, count);
        //     continue;
        // }
        
 
        if(strcmp(args[0],"GET")==0 && count == 3)
        { 
           get(args[1] , args[2]);   
        }
        else if(strcmp(args[0],"SET")==0 && count == 4 )
        {
           set(args[1] , args[2], args[3]);
        }
        else if(strcmp(args[0],"SHARE")==0 && count == 3)
        {
           share(args[1] , args[2]);
        }
        else if(strcmp(args[0], "ERASE")==0 && count == 3)
        {   
           erase(args[1] , args[2]);
        }
        else{
        }

        free_strings(args, count);
    }

    free(buf);

    return 0; 
}
