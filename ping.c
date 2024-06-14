#include "headers.h"

int ping(char *command)
{
    char *tokens[100];
    int number_of_tokens = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }
    char proc_id[1024];
    char sig_id[1024];
    strcpy(proc_id,tokens[1]);
    strcpy(sig_id,tokens[2]);
    pid_t pid = atoi(proc_id);
    int signal_number = atoi(sig_id);
    signal_number=signal_number%32;

    if(!kill(pid,signal_number))
    {
        remove_from_activities(pid);
        printf("Sent signal %d to process with pid %d\n",signal_number,pid);
    }
    else
    {
        remove_from_activities(pid);
        // printf("No such process found with pid %d\n", pid);
    }
    return 0;
}