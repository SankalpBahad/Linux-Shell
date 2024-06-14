#include "headers.h"

int pipes(char *command, char **past_commands, int command_count, char *initial_address, char* current_address)
{
    char *commands[100];
    int number_of_commands = 0;
    char *cmd = strtok(command, "|");
    while (cmd != NULL)
    {
        commands[number_of_commands] = strdup(cmd);
        cmd = strtok(NULL, "|");
        number_of_commands += 1;
    }

    // for(int i=0;i<number_of_commands;i++)
    // {
    //     printf("pipes->%s\n",commands[i]);
    // }

    int file_st[2];
    int org=0;
    if(number_of_commands>1)
    {
        for(int i=0;i<number_of_commands;i++)
        {
            // printf("%s\n",commands[i]);
            pipe(file_st);
            pid_t pid = fork();
            if(!pid)
            {
                dup2(org,0);
                if(i!=number_of_commands-1)
                {
                    dup2(file_st[1],1);
                }
                close(file_st[0]);
                int inp=dup(0);
                int out=dup(1);
                
                // if(!strcmp(commands[i][0],"echo"))
                // printf("%s\n",commands[i]);
                execute(commands[i], past_commands, command_count, initial_address, current_address);

                dup2(inp,STDIN_FILENO);
                dup2(out,STDOUT_FILENO);
                exit(0);
            }
            wait(NULL);
            close(file_st[1]);
            org=file_st[0];
        }
    }

    return 0;
}