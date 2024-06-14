#include "headers.h"
char initial_address[256];
char terminal_name[256];

int foreground = -1;

void ctrl_c(int signo)
{
    if (foreground > 0)
    {
        kill(foreground, SIGINT);
    }
}

int main(int argc, char *argv[])
{
    // hostname
    char hostname[256];
    int result1 = gethostname(hostname, 256);
    // // printf("System Name (Hostname): %s\n", hostname);

    // // username
    char *username = getlogin();
    // // printf("Username: %s\n", username);

    // // current working directory [spec-1]
    // // char initial_address[256];
    char *result = getcwd(initial_address, 256);
    // // printf("%s",initial_address);
    // // char showing_address[256];
    char *past_commands[15];
    int command_count = 0;
    // Keep accepting commands
    char hist_file_path[272];
    snprintf(hist_file_path, sizeof(hist_file_path), "%s/%s", initial_address, "/hist_file.txt");
    // printf("%s",hist_file_path);
    FILE *file;
    if (access(hist_file_path, F_OK | R_OK) != -1)
    {
        file = fopen(hist_file_path, "r");
    }
    else
    {
        file = fopen(hist_file_path, "a");
    }
    // int command_count = 0;
    char line[256];
    size_t line_buf_size;
    while ((fgets(line, sizeof(line), file)) != NULL)
    {
        // printf("%s",line);
        past_commands[command_count] = strdup(line);
        command_count++;
    }
    signal(SIGINT, ctrl_c);
    while (1)
    {
        int initial = dup(1);
        // // current working directory
        char current_address[256];
        // char showing_address[256];
        char *result2 = getcwd(current_address, 256);
        prompt(initial_address);
        char input[4096];
        fgets(input, 4096, stdin);
        input[strlen(input) - 1] = '\0';
        char *commands[100];
        int number_of_commands = 0;
        char *cmd = strtok(input, ";");
        while (cmd != NULL)
        {
            commands[number_of_commands] = strdup(cmd);
            cmd = strtok(NULL, ";");
            number_of_commands += 1;
        }
        if (feof(stdin))
        {
            printf("\n");
            kill(0, SIGKILL);
            break;
        }
        // printf("%s",commands[number_of_commands-1]);
        // return;
        for (int c = 0; c < number_of_commands; c++)
        {
            char command[256];
            char copy_command[256];
            strcpy(command, commands[c]);
            strcpy(copy_command, commands[c]);
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
            // continue;
            if (input_flag || output_flag || overwrite_flag)
            {
                // free(*tokens);
                // printf("Here first");
                char *new_command = io_redrn(command, input_flag, output_flag, overwrite_flag, past_commands, command_count, initial_address, current_address);
                dup2(initial, STDOUT_FILENO);
                close(initial);
                continue;
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
                continue;
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
                printf("pipe first");
                int pip = pipes(command, past_commands, command_count, initial_address, current_address);
                continue;
            }

            // warp [spec-3]
            if (!strcmp(tokens[0], "warp"))
            {
                warp(command, initial_address, current_address);
                continue;
            }

            // peek [spec-4]

            if (!strcmp(tokens[0], "peek"))
            {
                int p2 = peek(command);
                continue;
            }

            // pastevents [spec-5]

            if (!strcmp(tokens[0], "pastevents"))
            {
                // printf("%d\n",command_count);
                // continue;
                command_count = pastevents(command, past_commands, command_count, initial_address, current_address);
                continue;
            }
            // proclore [spec-7]

            if (!strcmp(tokens[0], "proclore"))
            {
                int p = proclore(command);
                continue;
            }
            // seek [spec-8]

            if (!strcmp(tokens[0], "seek"))
            {
                int x = seek(command);
                continue;
            }

            // activities [spec-12]

            if (!strcmp(tokens[0], "activities"))
            {
                int act = activities(command);
                continue;
            }

            // ping [spec-13]

            if (!strcmp(tokens[0], "ping"))
            {
                int pi = ping(command);
                continue;
            }

            // fg and bg [spec-14]

            if (!strcmp(tokens[0], "fg"))
            {
                int fgp = atoi(tokens[1]);
                int fgr = fg(fgp);
                continue;
            }

            if (!strcmp(tokens[0], "bg"))
            {
                int bgp = atoi(tokens[1]);
                int bgr = bg(bgp);
                continue;
            }

            // neonate [spec-15]

            if (!strcmp(tokens[0], "neonate"))
            {
                int neo = neonate(command);
                continue;
            }

            // iman [spec-16]

            if (!strcmp(tokens[0], "iMan"))
            {
                int im = iman(tokens[1]);
                continue;
            }

            // system commands [spec-6]

            int sys = system_calls(command);
        }
    }
}
