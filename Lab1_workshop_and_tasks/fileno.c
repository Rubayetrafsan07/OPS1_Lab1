#include <stdio.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  printf("Hello, world!\n");
  printf("fileno(stdin) = %d\n", fileno(stdin));
  printf("fileno(stdout) = %d\n", fileno(stdout));
  printf("fileno(stderr) = %d\n", fileno(stderr));
  return 0;
}


//output :

// Hello, world!
// fileno(stdin) = 0
// fileno(stdout) = 1
// fileno(stderr) = 2