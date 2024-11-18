#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define MAX_LINE 80 //command max length

//store the last command for '!!'
char last_command[MAX_LINE];

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[], char *second_command[], bool &run_in_background, char *&input_file, char *&output_file)
{
    int i = 0;
    char *token = strtok(command, " \n");
    bool is_pipe = false;
    run_in_background = false;
    input_file = NULL;
    output_file = NULL;

    while (token != NULL)
    {
        if (strcmp(token, "<") == 0)  //input redirection
        {
            token = strtok(NULL, " \n");
            input_file = token;  //store the input file name
            token = strtok(NULL, " \n");
            continue;
        }

        if (strcmp(token, ">") == 0)  //output redirection
        {
            token = strtok(NULL, " \n");
            output_file = token;  //output file name
            token = strtok(NULL, " \n");
            continue;
        }

        if (strcmp(token, "|") == 0)
        {
            is_pipe = true;  //found a pipe
            token = strtok(NULL, " \n");
            i = 0;  //reset i for the second command
            continue;
        }

        if (strcmp(token, "&") == 0)
        {
            run_in_background = true;  //command should run in background
            token = strtok(NULL, " \n");  //skip '&' and continue to the next token (if any)
            continue;  //move on to the next iteration without adding '&' to the arguments list
        }

        if (is_pipe)
        {
            second_command[i++] = token;  //command after pipe
        }
        else
        {
            args[i++] = token;  //first command
        }
        token = strtok(NULL, " \n");
    }

    args[i] = NULL;            //terminate argument list for first command
    second_command[i] = NULL;  //terminate argument list for second command
    return is_pipe ? 1 : 0;    //return 1 if there was a pipe, 0 otherwise
}


/**
 * @brief The main function of a simple UNIX Shell.
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];        //command that was entered
    char *args[MAX_LINE / 2 + 1];  //parsed out command line arguments
    char *second_command[MAX_LINE / 2 + 1]; //second command for pipes
    int should_run = 1;            //flag for when to exit program

    bool run_in_background = false; //track background execution
    char *input_file = NULL;        //input redirection
    char *output_file = NULL;       //output redirection

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        //read input command
        fgets(command, MAX_LINE, stdin);

        //'!!' for command history
        if (strcmp(command, "!!\n") == 0)
        {
            if (strlen(last_command) == 0)
            {
                printf("No command history found\n");
                continue;
            }
            strcpy(command, last_command);
            printf("%s\n", command);  //print last command
        }
        else
        {
            strcpy(last_command, command);  //Store current command
        }

        if (strcmp(command, "exit\n") == 0)  //exit command to terminate shell
        {
            should_run = 0;
            continue;
        }

        //Parse the input command, check for a pipe and redirection
        int is_pipe = parse_command(command, args, second_command, run_in_background, input_file, output_file);

        if (is_pipe)
        {
            //Handle piping between two commands
            int pipefd[2];
            if (pipe(pipefd) == -1)
            {
                perror("Pipe failed");
                return 1;
            }

            //Fork the first child process to execute the first command
            pid_t pid1 = fork();
            if (pid1 < 0)
            {
                perror("Fork failed");
                return 1;
            }

            if (pid1 == 0)
            {
                //first child: write to the pipe
                close(pipefd[0]);              //close unused read end
                dup2(pipefd[1], STDOUT_FILENO); //redirect stdout to pipe write
                close(pipefd[1]);              //close write end after dup

                //handle input redirection for first command, if any
                if (input_file != NULL)
                {
                    int fd_in = open(input_file, O_RDONLY);
                    if (fd_in < 0)
                    {
                        perror("Input file not found");
                        exit(1);
                    }
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                }

                execvp(args[0], args);  //execute the first command
                perror("Command not found");
                exit(1);
            }

            //fork second child process to execut second command
            pid_t pid2 = fork();
            if (pid2 < 0)
            {
                perror("Fork failed");
                return 1;
            }

            if (pid2 == 0)
            {
                close(pipefd[1]);             //close unused write end
                dup2(pipefd[0], STDIN_FILENO); //redirect stdin to pipe read
                close(pipefd[0]);             //close read end after dup

                //handle output redirection for second command, if any
                if (output_file != NULL)
                {
                    int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd_out < 0)
                    {
                        perror("Output file error");
                        exit(1);
                    }
                    dup2(fd_out, STDOUT_FILENO);
                    close(fd_out);
                }

                execvp(second_command[0], second_command);  //execute second command
                perror("Command not found");
                exit(1);
            }

            close(pipefd[0]);  //close both ends of pipe in parent process
            close(pipefd[1]);

            //parent process waits for both child processes unless in background
            if (!run_in_background)
            {
                wait(NULL);  //wait for first child
                wait(NULL);  //wait for second child
            }else{
                printf("Process running in background [%d]\n", pid1);  //print background process PID
            }
        }
        else
        {
            //no pipe, regular fork-exec for single command
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("Fork failed");
                return 1;
            }
            if (pid == 0)
            {
                //handle input redirection for single command
                if (input_file != NULL)
                {
                    int fd_in = open(input_file, O_RDONLY);
                    if (fd_in < 0)
                    {
                        perror("Input file not found");
                        exit(1);
                    }
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                }

                //handle output redirection for single command
                if (output_file != NULL)
                {
                    int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd_out < 0)
                    {
                        perror("Output file error");
                        exit(1);
                    }
                    dup2(fd_out, STDOUT_FILENO);
                    close(fd_out);
                }

                execvp(args[0], args);  //execute the command
                perror("Command not found");
                exit(1);
            }
            else
            {
                //parent process: wait for the child to finish unless in background
                if (!run_in_background)
                {
                    wait(NULL);
                }
            }
        }
    }
    return 0;
}
