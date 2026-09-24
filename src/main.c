#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>

#define MAX_SIZE 1024


int find_path(const char* file_name, char* result, size_t result_size){
  
  char* path_env = getenv("PATH");
  if(!path_env){
    return -1;
  }
  // Duplicate paths so we avoid modifying original path
  char* path = strdup(path_env);
  if (!path){
    return -1;
  }
  // Get individual path
  char* dir = strtok(path, ":");

  while (dir != NULL){
   
    char candidate[PATH_MAX];
    //Attach file name to the directory path
    snprintf(candidate, sizeof(candidate), "%s%s", dir, file_name);

    // Check that the file exists and is executable
    if (access(candidate, X_OK) == 0){
      snprintf(result, result_size, "%s", candidate);
      free(path);
      return 0;
    }

    dir = strtok(NULL, ":");
  }
  
  free(path);
  return -1;

}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[MAX_SIZE];
  while(1){

    printf("$ ");
    
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    char* command = strtok(input, " ");
    char* arg = strtok(NULL, "");

    if (command == NULL){
      continue;
    }

    
    if (strcmp(command, "exit") == 0){
      break;
    }
    else if (strcmp(command, "echo") == 0){
      printf("%s\n", input + 5);
    }
    else if (strcmp(command, "type") == 0){

      if(arg != NULL && strcmp(arg, "echo") == 0 
        || strcmp(arg, "exit") == 0 || strcmp(arg, "type") == 0){
            printf("%s is a shell builtin\n", arg);
      }
      else{
        char path[PATH_MAX];
        if(arg != NULL && find_path(arg, path, sizeof(path)) == 0){
          printf("%s is %s\n", arg, path);
        }
        else{
          printf("%s: not found\n", arg);
        }
      }
    }
    else{
      printf("%s: command not found\n", command);
    }
  }

  return 0;
}
