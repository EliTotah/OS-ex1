//Eli Totah 209186014

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>

// Function to print the prompt
void print_promot() {
    printf ("$ ");
    // Flush the standard output to ensure the prompt is printed immediately
    fflush(stdout);
}

// Function to parse user input into an array of arguments
void parse_input(char* input, char** arguments) {
    int argCount = 0;
    // Use strtok function to split the input string by space delimiter and store each argument in the arguments array
    char* token = strtok(input, " ");
    while(token!=NULL) {
        arguments[argCount] = token;
        argCount++;
        token = strtok(NULL, " ");
    }
    // Set the last argument as NULL to terminate the array
    arguments[argCount] = NULL;
}

// Function to update the environment PATH variable based on user input arguments
void envPath(int argc, char *argv[]) {
    char *path = getenv("PATH");
    int i;
    for (i = 1; i < argc; i++) {
        path = strcat(path, ":");
        path = strcat(path, argv[i]);
    }
    // Update the environment variable with the new path
    setenv("PATH", path, 1);
}

// Function to check if the command entered is a built-in command and execute it
int check_is_built_in(char* commands_history[101], int pid_history[101], char* args[101], int history_index) {
    // If the command is "exit", terminate the program
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    
    // If the command is "cd", change the current directory to the specified directory
    else if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) == -1) {
            perror("chdir failed");
        }
        // Return 1 to indicate a built-in command was executed
        return 1;
    }

    // If the command is "history", print the list of previous commands with their corresponding PIDs
    else if (strcmp(args[0], "history") == 0) {
        int i;
        for (i =0; i<=history_index; i++) {
            printf("%d %s\n", pid_history[i], commands_history[i]);
        }
        // Return 1 to indicate a built-in command was executed
        return 1;
    }
    // Return 0 to indicate a non-built-in command was entered
    return 0;
}

// Function to execute a command using fork and execvp system calls
int run_command(int pid_history[101], char* args[101], int history_index) {
    pid_t pid;
    // Fork a new process to execute the command
    pid = fork();

    // If the fork call fails, print an error message
    if (pid < 0) {
        perror("fork failed");
    }

    // If the current process is the child process
    else if (pid == 0) {
        // Execute the command using execvp function
        if (execvp(args[0], args) == -1) {
            // If the execvp call fails, print an error message
            perror ("execvp failed");
            // Return -1 to indicate an error occurred
            return -1;
        }
    } 

    // If the current process is the parent process
    else {
        // Store the child process PID in the pid_history array
        pid_history[history_index] = pid;
        // Wait for the child process to finish executing
        wait(NULL);
    }
    // Return 1 to indicate the command was executed successfully
    return 1;
}

// Main function to execute the shell program
int main(int argc, char *argv[]) {
    
    // Update PATH environment variable
    envPath(argc,argv);

    // Declare arrays to store commands history and their PIDs
    char *commands_history[101];
    int pid_history[101];
    int history_index;

    // Start infinite loop for accepting user input
    for (history_index = 0;;history_index++) {
        // Print prompt and get user input
        print_promot();
        char str[101] = "";
        scanf("%[^\n\r]", str);
        getchar();
        char str2[101] = "";
        strcpy(str2, str);

        // Parse user input into separate arguments
        char* args[101] = {};
        parse_input(str, args);

        // check if we got empty command
        if(args[0] == NULL) {
            history_index--;
            continue;
        }

        // Store the command and its PID in the history arrays
        commands_history[history_index] = strdup(str2);
        pid_history[history_index] = getpid();


        // Check if the command is a built-in command
        int res1 = check_is_built_in(commands_history, pid_history, args, history_index);
        // If it's a built-in command, continue the loop
        if (res1 == 1) {
            continue;
        } 

        // Run the command using execvp() in a child process
        int res2 = run_command(pid_history, args, history_index);
        // If execvp() fails, break the loop and exit the program
        if(res2 == -1) {
            break;;
        } 
    }
}


