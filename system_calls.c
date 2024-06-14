#include "headers.h"

int system_calls(char *command)
{
    char *tokens[100];
    int number_of_tokens = 0;
    char exec_command[256];
    strcpy(exec_command,command);
    char *token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }
    char *arguments[100];
    int number_args = 1;
    arguments[0] = tokens[0];
    int process = 1;
    if (!strcmp(tokens[number_of_tokens - 1], "&"))
    {
        process = 0;
    }
    int upper;
    if (process)
    {
        upper = number_of_tokens;
    }
    else
    {
        upper = number_of_tokens - 1;
    }

    for (int i = 1; i < upper; i++)
    {
        arguments[number_args] = tokens[i];
        number_args = number_args + 1;
    }
    arguments[number_args] = NULL;
    // clock_t start_time = clock();
    if (!process)
    {

        pid_t pid = fork();

        add_to_activities(exec_command,pid);

        if (pid == 0)
        {
            execvp(tokens[0], arguments);
            perror("execvp");
        }
        else if (pid != 0)
        {
            printf("Process with PID [%d] started\n", pid);
        }
        else
        {
        }
    }
    else
    {
        pid_t pid = fork();

        // add_to_activities(command,pid);
        
        if (pid == 0)
        {
            execvp(tokens[0], arguments);
            perror("execvp");
            exit(0);
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
        else
        {
        }
    }
}