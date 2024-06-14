#include "headers.h"

int found=0;
int seek_recursive(char *rel_path, char *target, int flag, int flag2)
{
    int found=0;
    struct dirent **file_list = NULL;
    int n = scandir(rel_path, &file_list, NULL, alphasort);
    int count = 0;
    // printf("%d\n", n);
    // break;
    // continue;
    if (n < 0)
    {
        // exit = 1;
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        char concatenated_path[513];
        sprintf(concatenated_path, "%s/%s", rel_path, file_list[i]->d_name);
        // printf("%s\n", concatenated_path);
        struct stat st_buf;
        stat(concatenated_path, &st_buf);

        if ((((flag == 0) || flag2 == 0) && S_ISDIR(st_buf.st_mode)) || ((flag == 2 || flag2 == 2) && S_ISREG(st_buf.st_mode)))
        {
            if (!strcmp(file_list[i]->d_name, target))
            {
                if ((flag == 1 || flag2 == 1))
                {
                    if ((flag == 0 || flag2 == 0) && S_ISDIR(st_buf.st_mode))
                    {
                        if (access(concatenated_path, X_OK) != 0)
                        {
                            printf("Missing permissions for task!\n");
                            return 0;
                        }
                        printf("%s\n", concatenated_path);
                        ++found;
                        chdir(concatenated_path);
                    }
                    else if ((flag == 2 || flag2 == 2) && S_ISREG(st_buf.st_mode))
                    {
                        if (access(concatenated_path, R_OK) != 0)
                        {
                            printf("Missing permissions for task!\n");
                            return 0;
                        }
                        printf("%s\n", concatenated_path);
                        ++found;
                    }
                }
                else
                {
                    printf("%s\n", concatenated_path);
                    ++found;
                }
            }
        }
        // if (exit)
        // {
        //     break;
        // }

        if (S_ISDIR(st_buf.st_mode) && strcmp(file_list[i]->d_name, ".") != 0 && strcmp(file_list[i]->d_name, "..") != 0)
        {
            found=seek_recursive(concatenated_path, target, flag, flag2);
            // break;
        }
        // if (exit)
        // {
        //     break;
        // }

        free(file_list[i]);
    }
    // if (exit)
    // {
    //     break;
    // }
    // printf("Here");
    // printf("%d ",found);
    free(file_list);
    return found;
}

int seek(char *command)
{
    
    // flag for d = 0, e = 1, f = 2
    char *tokens[100];
    int number_of_tokens = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }
    // printf("%s",command);
    int flag = -1;
    int flag2 = -1;
    char rel_path[256];
    char target[256];
    if (number_of_tokens == 2)
    {
        strcpy(target, tokens[1]);
        flag = 0;
        flag2 = 2;
        strcpy(rel_path,".");
    }
    if (number_of_tokens == 3)
    {
        // strcpy(target,tokens[2]);
        if (!strcmp(tokens[1], "-d"))
        {
            flag = 0;
        }
        if (!strcmp(tokens[1], "-e"))
        {
            flag = 1;
        }
        if (!strcmp(tokens[1], "-f"))
        {
            flag = 2;
        }
        if (flag == -1)
        {
            strcpy(target, tokens[1]);
            strcpy(rel_path, tokens[2]);
        }
        else
        {
            strcpy(target, tokens[2]);
            strcpy(rel_path,".");
        }
    }
    if (number_of_tokens == 4)
    {
        if (!strcmp(tokens[1], "-d"))
        {
            flag = 0;
        }
        if (!strcmp(tokens[1], "-e"))
        {
            flag = 1;
        }
        if (!strcmp(tokens[1], "-f"))
        {
            flag = 2;
        }
        if (!strcmp(tokens[2], "-d"))
        {
            flag2 = 0;
        }
        if (!strcmp(tokens[2], "-e"))
        {
            flag2 = 1;
        }
        if (!strcmp(tokens[2], "-f"))
        {
            flag2 = 2;
        }
        if (flag == 0 && flag2 == 2)
        {
            printf("Invalid flags!\n");
            return 0;
        }
        if (flag2 == 0 && flag == 2)
        {
            printf("Invalid flags!\n");
            return 0;
        }
        if (flag2 == -1)
        {
            strcpy(target, tokens[2]);
            strcpy(rel_path, tokens[3]);
        }
        else
        {
            strcpy(target, tokens[3]);
            strcpy(rel_path,".");
        }
    }
    if (number_of_tokens == 5)
    {
        if (!strcmp(tokens[1], "-d"))
        {
            flag = 0;
        }
        if (!strcmp(tokens[1], "-e"))
        {
            flag = 1;
        }
        if (!strcmp(tokens[1], "-f"))
        {
            flag = 2;
        }
        if (!strcmp(tokens[2], "-d"))
        {
            flag2 = 0;
        }
        if (!strcmp(tokens[2], "-e"))
        {
            flag2 = 1;
        }
        if (!strcmp(tokens[2], "-f"))
        {
            flag2 = 2;
        }

        if (flag == 0 && flag2 == 2)
        {
            printf("Invalid flags!\n");
            return 0;
        }
        if (flag2 == 0 && flag == 2)
        {
            printf("Invalid flags!\n");
            return 0;
        }

        strcpy(rel_path, tokens[4]);
        strcpy(target, tokens[3]);
    }
    found=seek_recursive(rel_path, target, flag, flag2);
    // printf("%d",found);
    // if(found==0)
    // {
    //     printf("No Matching Searches!\n");
    // }
    int found=0;
    return 1;
}