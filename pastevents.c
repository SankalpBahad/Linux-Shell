#include "headers.h"

// void execute(char *command, char **past_commands, int command_count, char *initial_address, char* current_address)
// {
//     char copy_command[256];
//     strcpy(copy_command, command);
//     // printf("%s\n",commands[c]);
//     // return;
//     char *tokens[100];
//     int number_of_tokens = 0;

//     char *token = strtok(copy_command, " ");
//     while (token != NULL)
//     {
//         tokens[number_of_tokens++] = strdup(token);
//         token = strtok(NULL, " ");
//     }

//     if (strcmp("pastevents", tokens[0]))
//     {
//         // printf("%s",command);
//         if (command_count > 0)
//         {
//             if (strcmp(command, past_commands[command_count - 1]))
//             {
//                 // printf("%s\n",command);
//                 // printf("%s\n",past_commands[command_count - 1]);
//                 if (command_count < 15)
//                 {
//                     past_commands[command_count++] = strdup(command);
//                 }
//                 else
//                 {
//                     for (int i = 1; i < 15; i++)
//                     {
//                         char temp[256];
//                         strcpy(temp, past_commands[i]);
//                         // printf("%s\n",temp);
//                         free(past_commands[i - 1]);
//                         past_commands[i - 1] = strdup(temp);
//                     }
//                     free(past_commands[14]);
//                     past_commands[14] = strdup(command);
//                 }
//             }
//         }
//         else
//         {
//             past_commands[command_count++] = strdup(command);
//         }
//     }

//     // warp [spec-3]

//     if (!strcmp(tokens[0], "warp"))
//     {
//         warp(command, initial_address, current_address);
//         return;
//     }

//     // peek [spec-4]

//     if (!strcmp(tokens[0], "peek"))
//     {
//         int p2 = peek(command);
//         return;
//     }

//     // pastevents [spec-5]

//     if (!strcmp(tokens[0], "pastevents"))
//     {
//         // printf("%d\n",command_count);
//         // return;
//         command_count = pastevents(command, past_commands, command_count, initial_address, current_address);
//         return;
//     }

//     // proclore [spec-7]

//     if (!strcmp(tokens[0], "proclore"))
//     {
//         int p = proclore(command);
//         return;
//     }

//     // seek [spec-8]

//     if (!strcmp(tokens[0], "seek"))
//     {
//         int x = seek(command);
//         return;
//     }

//     // system commands [spec-6]

//     char *arguments[100];
//     int number_args = 1;
//     arguments[0] = tokens[0];
//     int process = 1;

//     if (!strcmp(tokens[number_of_tokens - 1], "&"))
//     {
//         process = 0;
//     }
//     int upper;
//     if (process)
//     {
//         upper = number_of_tokens;
//     }
//     else
//     {
//         upper = number_of_tokens - 1;
//     }

//     for (int i = 1; i < upper; i++)
//     {
//         arguments[number_args] = tokens[i];
//         number_args = number_args + 1;
//     }
//     arguments[number_args] = NULL;

//     if (!process)
//     {

//         pid_t pid = fork();

//         if (pid == 0)
//         {
//             execvp(tokens[0], arguments);
//             perror("execvp");
//         }
//         else if (pid != 0)
//         {
//             printf("Process with PID [%d] started\n", pid);
//         }
//         else
//         {
//         }
//     }
//     else
//     {
//         pid_t pid = fork();
//         if (pid == 0)
//         {
//             execvp(tokens[0], arguments);
//             perror("execvp");
//             exit(0);
//         }
//         else if (pid > 0)
//         {
//             wait(NULL);
//         }
//         else
//         {
//         }
//     }
// }

int pastevents(char *command, char **past_commands, int num_commands, char *initial_address, char *current_address)
{
    char *tokens[100];
    int number_of_tokens = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }

    if (number_of_tokens == 1)
    {
        // printf("%d",num_commands);
        // return num_commands;
        if (num_commands)
        {
            for (int i = 0; i < num_commands; i++)
            {
                printf("%s\n", past_commands[i]);
                // free(past_commands[i]);
            }
        }
        else
        {
            return 0;
        }
    }
    if (number_of_tokens > 1)
    {
        if (!strcmp(tokens[1], "purge"))
        {
            for (int i = num_commands - 1; i >= 0; i--)
            {
                free(past_commands[i]);
                past_commands[i] = NULL;
                // num_commands -= 1;
            }
            num_commands = 0;
            // free(*past_commands);
            // printf("%d");
        }
        if (!strcmp(tokens[1], "execute"))
        {
            int num = (int)(*tokens[2]) - 48;
            // printf("%d\n",num);
            if (num > num_commands)
            {
                num = 0;
            }
            strcpy(command, past_commands[num_commands - num]);
            printf("%s\n", command);
            execute(command, past_commands, num_commands, initial_address, current_address);
            return num_commands;
        }
    }
    // printf("\n");
    char hist_file_path[256];
    snprintf(hist_file_path, sizeof(hist_file_path), "%s/%s", initial_address, "/hist_file.txt");
    // printf("%s",hist_file_path);
    FILE *file = fopen(hist_file_path, "w");
    // if (!file)
    // {
    //     // perror("Unable to open the file");
    //     return 1;
    // }
    if (num_commands)
    {
        for (int i = 0; i < num_commands; i++)
        {
            fprintf(file, "%s\n", past_commands[i]);
            // free(past_commands[i]);
        }
    }
    fclose(file);
    return num_commands;
}