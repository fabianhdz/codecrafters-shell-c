#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // TODO: Uncomment the code below to pass the first stage
  printf("$ ");
  char command[MAX_SIZE];
  fgets(command, sizeof(command), stdin);
  
  printf("%s: command not found", command);

  return 0;
}
