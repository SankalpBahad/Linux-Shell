#include "headers.h"

void warp(char *c, char *initial_address, char *current_address)
{
    char *tokens[100];
    int number_of_tokens = 0;

    char *token = strtok(c, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }

    char showing_address[256];
    if (number_of_tokens == 1)
    {
        int result3 = chdir(initial_address);

        char current_address[1024];
        char *result4 = getcwd(current_address, 256);
        // strcpy(showing_address,get_showing_address(current_address));
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
        printf("%s\n", showing_address);
        return;
    }
    if (number_of_tokens >= 2)
    {
        for (int i = 1; i < number_of_tokens; i++)
        {
            // printf("'%s'", tokens[1]);
            if (!strcmp(tokens[i], "~") || !strcmp(tokens[i], " "))
            {
                int result3 = chdir(initial_address);
            }
            else if (!strcmp(tokens[i], "-"))
            {
                printf("%s\n", current_address);
                continue;
            }
            else
            {
                int result3 = chdir(tokens[i]);
            }
            char current_address[1024];
            char *result5 = getcwd(current_address, 256);
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
            printf("%s\n", current_address);
        }
        return;
    }
}