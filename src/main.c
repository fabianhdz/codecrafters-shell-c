#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_SIZE 1024
#define MAX_ARGS 128

int find_path(const char* file_name, char* result, size_t result_size);


int main(int argc, char* argv[]) {
    /* Flush after every printf */
    setbuf(stdout, NULL);

    char input[MAX_SIZE];
    while (1) {

        printf("$ ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char* input_state;
        char* command = strtok_r(input, " ", &input_state);
        char* arg = strtok_r(NULL, "", &input_state);

        char path[PATH_MAX];

        if (command == NULL) {
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "echo") == 0) {
            printf("%s\n", arg);
        } else if (strcmp(command, "type") == 0) {
            /* Builtin types */
            if (arg != NULL &&
                strcmp(arg, "echo") == 0 ||
                strcmp(arg, "exit") == 0 ||
                strcmp(arg, "type") == 0 ||
                strcmp(arg, "pwd") == 0  ||
                strcmp(arg, "cd") == 0) {
                    printf("%s is a shell builtin\n", arg);
          } else { /* Executable types */
                if (arg != NULL && find_path(arg, path, sizeof(path)) == 0) {
                    printf("%s is %s\n", arg, path);
                } else {
                    printf("%s: not found\n", arg);
                }
            }
        } else if (strcmp(command, "pwd") == 0) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            }
        } else if (strcmp(command, "cd") == 0) {
            if (access(arg, F_OK) == 0) {
                chdir(arg);
            } else {
                printf("cd: %s: No such file or directory\n", arg);
            }
        } else if (find_path(command, path, sizeof(path)) == 0) { /* Execute command  if it exists*/
            char* args[MAX_ARGS];
            args[0] = command;
            int count = 1;
            char* arg_state;
            char* token = strtok_r(arg, " ", &arg_state);
            while (token != NULL && count < MAX_ARGS - 1) {
                args[count] = token;
                count++;

                token = strtok_r(NULL, " ", &arg_state);
            }
            args[count] = NULL;

            /* Fork the program */
            pid_t pid = fork();
            switch (pid) {
              case -1:
                    perror("fork");
                    continue;
              case 0:
                    execv(path, args);
                    perror("execv");
                    _exit(EXIT_FAILURE);
              default:
                    waitpid(pid, NULL, 0);
                    continue;
            }
        } else {
            printf("%s: command not found\n", command);
        }
    }

    return 0;
}

int find_path(const char* file_name, char* result, size_t result_size) {

    char* path_env = getenv("PATH");
    if (!path_env) {
        return -1;
    }
    /* Duplicate paths so we avoid modifying original path */
    char* path = strdup(path_env);
    if (!path) {
        return -1;
    }
    /* Get individual path */
    char* dir = strtok(path, ":");

    while (dir != NULL) {

        char candidate[PATH_MAX];
        /* Attach file name to the directory path */
        snprintf(candidate, sizeof(candidate), "%s/%s", dir, file_name);

        /* Check that the file exists and is executable */
        if (access(candidate, X_OK) == 0) {
            snprintf(result, result_size, "%s", candidate);
            free(path);
            return 0;
        }

        dir = strtok(NULL, ":");
    }

    free(path);
    return -1;

}
