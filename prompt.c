#include "headers.h"

#define YELLOW "\x1B[33m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"
#define BLUE "\x1B[34m"

char hostname[256];
char *past_commands[15];
// char current_address[256];
char showing_address[256];
// char initial_address[256];

void prompt(char* initial_address)
{
    // Do not hardcode the prmopt
    // printf("<Everything is a file> ");
    // hostname
    int result1 = gethostname(hostname, 256);
    // printf("System Name (Hostname): %s\n", hostname);

    // username
    char *username = getlogin();
    // printf("Username: %s\n", username);

    // current working directory [spec-1]
    // char initial_address[256];
    
    // printf("%s",initial_address);
    // char showing_address[256];

    int command_count = 0;

    // current working directory
    char current_address[256];
    char *result2 = getcwd(current_address, 256);
    // printf("%s\n",initial_address);
    strcpy(showing_address, current_address);
    char *occurrence = strstr(showing_address, initial_address);

    while (occurrence)
    {

        int position = occurrence - showing_address;

        showing_address[position] = '~';
        for (int i = 1; i < strlen(initial_address); i++)
        {
            showing_address[position + i] = '\0';
        }

        occurrence = strstr(occurrence + 1, initial_address);
    }

    printf(YELLOW "%s@%s:%s$ " RESET, username, hostname, showing_address);
}
