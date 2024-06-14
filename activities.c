#include "headers.h"

char *processes[100];
int pids[256];
int number_of_processes = 0;

char *get_activity(int pid)
// void get_activity(int pid)
{
    for (int i = 1; i < number_of_processes + 1; i++)
    {
        if (pids[i] == pid)
        {
            // found_flag=1;
            // printf("%s\n",processes[i]);
            return processes[i];
        }
        // printf("%d %s\n",pids[i],processes[i]);
    }
}

void add_to_activities(char *command, int pid)
{
    // int number_of_processes = strlen(processes);
    number_of_processes = number_of_processes + 1;
    processes[number_of_processes] = strdup(command);
    pids[number_of_processes] = pid;
    return;
}

void remove_from_activities(int pid)
{
    int found_flag = 0;

    if (number_of_processes == 1)
    {
        // free(pids[1]);
        pids[1] = 0;
        free(processes[1]);
        processes[1] = NULL;
        number_of_processes -= 1;
        return;
    }

    if (pids[number_of_processes] == pid)
    {
        // free(pids[number_of_processes]);
        pids[number_of_processes] = 0;
        free(processes[number_of_processes]);
        processes[number_of_processes] = NULL;
        number_of_processes -= 1;
        return;
    }

    for (int i = 1; i < number_of_processes; i++)
    {
        if (pids[i] == pid)
        {
            found_flag = 1;
        }
        if (found_flag)
        {
            // printf("%d %s \n", pids[i], processes[i]);
            pids[i] = pids[i + 1];
            free(processes[i]);
            processes[i] = strdup(processes[i + 1]);
        }
    }
    if (found_flag)
    {
        number_of_processes -= 1;
    }
}

int activities(char *command)
{
    for (int i = 1; i < number_of_processes + 1; i++)
    {
        char ch;
        int status;
        int result = waitpid(pids[i], &status, WNOHANG);

        if (result == 0)
        {
            ch = 'R';
        }
        else
        {
            ch = 'S';
        }
        char showing_comm[256];
        int l=strlen(processes[i]);
        strncpy(showing_comm, processes[i],l - 2);
        printf("%d %s %c\n", pids[i], showing_comm, ch);
    }
    return 0;
}
