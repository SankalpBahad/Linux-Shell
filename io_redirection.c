#include "headers.h"

char input_file[256];
char output_file[256];
char overwrite_file[256];

char* io_redrn(char *command, int inp_flg, int out_flg, int over_flg, char **past_commands, int command_count, char *initial_address, char* current_address)
{

    char *tokens[100];
    int number_of_tokens = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }

    for (int i = 0; i < number_of_tokens; i++)
    {
        if (!strcmp(tokens[i], "<"))
        {
            if (i == 0)
            {
                printf("No Input File provided ! \n");
                return NULL;
            }
            // printf("%s ",tokens[i]);
            // input_flag = 1;
            strcpy(input_file, tokens[i + 1]);
        }
        if (!strcmp(tokens[i], ">"))
        {
            if (i == number_of_tokens - 1)
            {
                printf("No Output File provided ! \n");
                return NULL;
            }
            // printf("%s ",tokens[i]);
            // output_flag = 1;
            strcpy(output_file, tokens[i + 1]);
        }
        if (!strcmp(tokens[i], ">>"))
        {
            if (i == number_of_tokens - 1)
            {
                printf("No Output File provided ! \n");
                return NULL;
            }
            // printf("%s ",tokens[i]);
            // overwrite_flag = 1;
            strcpy(overwrite_file, tokens[i + 1]);
        }
    }
    
    if (inp_flg)
    {
        int inp_file = open(input_file, O_RDONLY);
        dup2(inp_file, STDIN_FILENO);
        close(inp_file);
    }

    if (out_flg)
    {
        int out_file = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(out_file, STDOUT_FILENO);
        close(out_file);
    }

    if (over_flg)
    {
        int over_file = open(overwrite_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(over_file, STDOUT_FILENO);
        close(over_file);
    }

    char *arguments[100];
    int number_args = 1;
    arguments[0] = tokens[0];

    for (int i = 1; i < number_of_tokens; i++)
    {
        if (!strcmp(tokens[i], "<"))
        {
            arguments[number_args]=" ";
        }
        else if (!strcmp(tokens[i], ">") || !strcmp(tokens[i],">>"))
        {
            // arguments[number_args]='\0';
            break;
        }
        else
        {
            arguments[number_args] = tokens[i];
        }
        number_args = number_args + 1;
    }
    arguments[number_args] = NULL;
    int total_length=0;
    for (int a = 0; a < number_args; a++)
    {
        // printf("%s ", arguments[a]);
        total_length+=strlen(arguments[a]);
    }
    // printf("\n");
    char* new_command = (char*)malloc(total_length + 1);
    // printf("%s",arguments);
    new_command[0] = '\0';
    char space[] = " ";
    // Iterate through the char** array and concatenate each word
    for (int i = 0; i < number_args; i++) {
        strcat(new_command, arguments[i]);
        strcat(new_command, space);
    }
    // printf("io->%s\n",new_command);

    execute(new_command, past_commands, command_count, initial_address, current_address);

    // pid_t pid = fork();

    // if (pid == 0)
    // {
    //     if (execvp(arguments[0], arguments) == -1)
    //     {
    //         // perror("Execvp failed");
    //         // break;
    //         // exit(1);
    //     }
    // }
    // else if (pid > 0)
    // {
    //     // int status;
    //     // waitpid(pid, &status, 0);
    //     wait(NULL);
    // }
    // else
    // {
    //     perror("Fork failed");
    // }

    return new_command;
}