#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[MAX_SIZE];
  while(1){

    printf("$ ");
  
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    char* builtin = strtok(input, " ");
    char* arg = strtok(NULL, "");

    if (builtin == NULL){
      continue;
    }

    
    if (strcmp(builtin, "exit") == 0){
      break;
    }
    else if (strcmp(builtin, "echo") == 0){
      printf("%s\n", input + 5);
    }
    else if (strcmp(builtin, "type") == 0){
      if(arg != NULL && strcmp(arg, "echo") == 0 || strcmp(arg, "exit") == 0
          || strcmp(arg, "type") == 0){
            printf("%s is a shell builtin\n", arg);
      }
      else{
        printf("%s: not found\n", arg);
      }
    }
    else{
      printf("%s: command not found\n", builtin);
    }
  }

  return 0;
}
