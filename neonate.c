#include "headers.h"

// struct termios orig_termios;
void enableRawMode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


void disableRawMode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag |= (ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int kbhit()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int neonate(char *command)
{
    char *tokens[100];
    int number_of_tokens = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }

    int time_arg = atoi(tokens[2]);

    enableRawMode();
    pid_t pid;

    while (!kbhit())
    {
        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            // Child process
            printf("%d\n", getpid());
            sleep(time_arg);
            exit(0);
        }
        else
        {
            // Parent process
            wait(NULL); // Wait for the child process to complete
        }
    }

    disableRawMode();
    return 0;
}
