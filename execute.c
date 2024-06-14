#include "headers.h"

void execute(char *command, char **past_commands, int command_count, char *initial_address, char *current_address)
{
    // printf("exec->%s\n",command);
    char copy_command[256];
    strcpy(copy_command, command);
    // printf("%s\n",commands[c]);
    // return;
    char *tokens[100];
    int number_of_tokens = 0;

    char *token = strtok(copy_command, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }

    if (strcmp("pastevents", tokens[0]))
    {
        // printf("%s",command);
        if (command_count > 0)
        {
            if (strcmp(command, past_commands[command_count - 1]))
            {
                // printf("%s\n",command);
                // printf("%s\n",past_commands[command_count - 1]);
                if (command_count < 15)
                {
                    past_commands[command_count++] = strdup(command);
                }
                else
                {
                    for (int i = 1; i < 15; i++)
                    {
                        char temp[256];
                        strcpy(temp, past_commands[i]);
                        // printf("%s\n",temp);
                        free(past_commands[i - 1]);
                        past_commands[i - 1] = strdup(temp);
                    }
                    free(past_commands[14]);
                    past_commands[14] = strdup(command);
                }
            }
        }
        else
        {
            past_commands[command_count++] = strdup(command);
        }
    }

    // I/O Redirection [spec-9]

    int input_flag = 0;
    int output_flag = 0;
    int overwrite_flag = 0;

    for (int i = 0; i < number_of_tokens; i++)
    {
        if (!strcmp(tokens[i], "<"))
        {
            // printf("%s ",tokens[i]);
            input_flag = 1;
        }
        if (!strcmp(tokens[i], ">"))
        {
            // printf("%s ",tokens[i]);
            output_flag = 1;
        }
        if (!strcmp(tokens[i], ">>"))
        {
            // printf("%s ",tokens[i]);
            overwrite_flag = 1;
        }
    }
    // printf("%d %d %d\n",input_flag,output_flag,overwrite_flag);
    // return;
    if (input_flag || output_flag || overwrite_flag)
    {
        free(*tokens);
        char *new_command = io_redrn(command, input_flag, output_flag, overwrite_flag, past_commands, command_count, initial_address, current_address);
        return;
    }

    // pipes [spec-10]

    int pipe_flag = 0;

    for (int i = 0; i < number_of_tokens - 1; i++)
    {
        if (!strcmp(tokens[i], "|") && !strcmp(tokens[i + 1], "|"))
        {
            pipe_flag = 2;
        }
    }

    if (!strcmp(tokens[0], "|") || !strcmp(tokens[number_of_tokens - 1], "|"))
    {
        pipe_flag = 2;
    }

    if (pipe_flag == 2)
    {
        // printf("Invalid Pipe Usage!\n");
        printf("Invalid Usage of Pipe!\n");
        return;
    }

    pipe_flag = 0;

    for (int i = 0; i < number_of_tokens; i++)
    {
        if (!strcmp(tokens[i], "|"))
        {
            pipe_flag = 1;
        }
    }

    if (pipe_flag)
    {
        int pip = pipes(command, past_commands, command_count, initial_address, current_address);
        return;
    }

    // warp [spec-3]
    if (!strcmp(tokens[0], "warp"))
    {
        warp(command, initial_address, current_address);
        
        return;
    }

    // peek [spec-4]

    if (!strcmp(tokens[0], "peek"))
    {
        int p2 = peek(command);
        
        return;
    }

    // pastevents [spec-5]

    if (!strcmp(tokens[0], "pastevents"))
    {
        // printf("%d\n",command_count);
        // return;
        command_count = pastevents(command, past_commands, command_count, initial_address, current_address);
        
        return;
    }
    // proclore [spec-7]

    if (!strcmp(tokens[0], "proclore"))
    {
        int p = proclore(command);
        
        return;
    }
    // seek [spec-8]

    if (!strcmp(tokens[0], "seek"))
    {
        int x = seek(command);
        
        return;
    }

    // activities [spec-12]

    if (!strcmp(tokens[0], "activities"))
    {
        int act = activities(command);
        
        return;
    }

    // ping [spec-13]

    if (!strcmp(tokens[0], "ping"))
    {
        int pi = ping(command);
        
        return;
    }

    // fg and bg [spec-14]

    if (!strcmp(tokens[0], "fg"))
    {
        int fgp = atoi(tokens[1]);
        int fgr = fg(fgp);
        return;
    }

    if (!strcmp(tokens[0], "bg"))
    {
        int bgp = atoi(tokens[1]);
        int bgr = bg(bgp);
        return;
    }

    // neonate [spec-15]

    // iman [spec-16]

    if (!strcmp(tokens[0], "iMan"))
    {
        int im = iman(tokens[1]);
        return;
    }

    // system commands [spec-6]

    int sys = system_calls(command);
}