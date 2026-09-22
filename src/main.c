#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[MAX_SIZE];
  while(1)
  {
    printf("$ ");
  
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    int length = sizeof(input)/sizeof(input[0]);
    char command[1024];

    for(int i = 0; i < length; i++){
      if(isspace((unsigned char)input[i])){
        command[i] = '\0';
        break;
      }
      command[i] = input[i];
    }
    if (strcmp(input, "exit") == 0){
      break;
    }
    if (strcmp(command, "echo") == 0){
      printf("%s\n", input + 5);
      continue;
    }
    printf("%s: input not found\n", input);
  }

  return 0;
}
