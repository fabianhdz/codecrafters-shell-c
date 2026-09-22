#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 4096

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // TODO: Uncomment the code below to pass the first stage
  printf("$ ");
  char input[MAX_SIZE];

  if(scanf("%s", input) != 1){
    printf("Invalid input");
  }

  printf("%s: command not found");

  return 0;
}
