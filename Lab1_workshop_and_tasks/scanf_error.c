#include <stdio.h>
#include <unistd.h>

int main(int argc, const char **argv) 
{
  char buf[10];
  int age;
  int ret;

   printf("Your age? > ");
   ret = scanf("%d", &age);   // ret store = 1 as sucessful and 0 as failure
  printf("scanf() returned %d, age = %d\n", ret, age);

    printf("scanf() returned %d, age = %d\n", ret, age);
  printf("feof() = %d, ferror() = %d\n", feof(stdin), ferror(stdin));

printf("Your name? > ");
  ret = scanf("%9s", buf);  // if sucessful ret = 1, if failure ret = 0
  printf("scanf() returned %d, name = %s\n", ret, buf);
  printf("feof() = %d, ferror() = %d\n", feof(stdin), ferror(stdin));

  return 0;

}

// output :
// Your age? > 25
// scanf() returned 1, age = 25
// scanf() returned 1, age = 25
// feof() = 0, ferror() = 0
// Your name? > rafsan
// scanf() returned 1, name = rafsan
// feof() = 0, ferror() = 0    // feof(stdin) checks whether the end-of-file (EOF) indicator has been set on the input stream.
// ferror(stdin) checks whether an input error indicator has been set.

// when u press ctrl+d (EOF) after entering the age, the output will be: 
// Your age? > 25
// scanf() returned 1, age = 25
// scanf() returned 1, age = 25
// feof() = 1, ferror() = 0