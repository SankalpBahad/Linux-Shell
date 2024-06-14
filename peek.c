#include "headers.h"

char *get_premissions(mode_t mode)
{
    static char file_permissions[11];
    strcpy(file_permissions, "----------");

    if (S_ISDIR(mode))
        file_permissions[0] = 'd';
    if (mode & S_IRUSR)
        file_permissions[1] = 'r';
    if (mode & S_IWUSR)
        file_permissions[2] = 'w';
    if (mode & S_IXUSR)
        file_permissions[3] = 'x';
    if (mode & S_IRGRP)
        file_permissions[4] = 'r';
    if (mode & S_IWGRP)
        file_permissions[5] = 'w';
    if (mode & S_IXGRP)
        file_permissions[6] = 'x';
    if (mode & S_IROTH)
        file_permissions[7] = 'r';
    if (mode & S_IWOTH)
        file_permissions[8] = 'w';
    if (mode & S_IXOTH)
        file_permissions[9] = 'x';

    return file_permissions;
}

int peek(char *command)
{
    char *tokens[100];
    int numTokens = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[numTokens++] = strdup(token);
        token = strtok(NULL, " ");
    }
    // printf("%d",numTokens);
    // return;
    char directory_path[256];
    if (numTokens == 1)
    {
        // printf("Here");
        // return;
        strcpy(directory_path, ".");
        // const char* directory_path=".";
    }
    else
    {
        // const char* directory_path = tokens[numTokens - 1];
        strcpy(directory_path, tokens[numTokens - 1]);
    }
    // printf("%s",directory_path);
    // return;
    DIR *dir = opendir(directory_path);
    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }
    // return;
    char *files[256];
    struct dirent *file;
    // file=0 exec=1 dir=2
    int index = 0;
    char filename[256];
    int file_type[256];
    while ((file = readdir(dir)) != NULL)
    {
        char concatenated_path[2305];
        snprintf(concatenated_path, sizeof(concatenated_path), "%s/%s", directory_path, file->d_name);

        struct stat file_stats;
        if (stat(concatenated_path, &file_stats) == 0)
        {
            if (S_ISREG(file_stats.st_mode))
            {
                // printf("%s is a regular file.\n", concatenated_path);
                file_type[index] = 0;

                // Check if the file is executable
                if (file_stats.st_mode & S_IXUSR)
                {
                    // printf("%s is also executable.\n", concatenated_path);
                    file_type[index] = 1;
                }
            }
            else if (S_ISDIR(file_stats.st_mode))
            {
                // printf("%s is a directory.\n", concatenated_path);
                file_type[index] = 2;
            }
            strcpy(filename, file->d_name);
            files[index] = strdup(filename);
            index += 1;
        }
        else
        {
            perror("stat");
        }
        // count += 1;
        // printf("%s  ", file->d_name);
    }
    if (numTokens == 3)
    {
        if (!strcmp(tokens[1], "-a"))
        {

            // int count = 0;
            DIR *dir = opendir(directory_path);
            if (dir == NULL)
            {
                perror("opendir");
                return 1;
            }
            // int file_type[256];
            int index = 0;
            struct dirent *file;
            // char filename[256];
            // int file_type[256];
            while ((file = readdir(dir)) != NULL)
            {
                char concatenated_path[2305];
                snprintf(concatenated_path, sizeof(concatenated_path), "%s/%s", directory_path, file->d_name);

                struct stat file_stats;
                if (stat(concatenated_path, &file_stats) == 0)
                {
                    if (S_ISREG(file_stats.st_mode))
                    {
                        // printf("%s is a regular file.\n", concatenated_path);
                        file_type[index] = 0;

                        // Check if the file is executable
                        if (file_stats.st_mode & S_IXUSR)
                        {
                            // printf("%s is also executable.\n", concatenated_path);
                            file_type[index] = 1;
                        }
                    }
                    else if (S_ISDIR(file_stats.st_mode))
                    {
                        // printf("%s is a directory.\n", concatenated_path);
                        file_type[index] = 2;
                    }
                }
                else
                {
                    perror("stat");
                }
                index += 1;
                // printf("%s  ", file->d_name);
                strcpy(filename, file->d_name);
                // files[index] = strdup(filename);
                // index += 1;
            }
            // printf("\n");
            // char *sorted_files[256];
            // for (int i = 0; i < count; i++)
            // {
            //     strcpy(sorted_files[i], files[i]);
            // }
            // printf("%s,%s",sorted_files[0],files[0]);
            // qsort(sorted_files, count, sizeof(char *), compare_files);

            for (int i = 0; i < index; i++)
            {
                // int i = 0;
                // for (int j = 0; j < count; j++)
                // {
                //     if (!strcmp(files[j], sorted_files[i]))
                //     {
                //         i = j;
                //     }
                // }
                if (file_type[i] == 0)
                {
                    printf("%s  ", files[i]);
                }
                if (file_type[i] == 1)
                {
                    printf(GREEN "%s  " RESET, files[i]);
                }
                if (file_type[i] == 2)
                {
                    printf(BLUE "%s  " RESET, files[i]);
                }
                // printf("%s  ", files[i]);
                // if(i%5==0)
                // {
                //     printf("\n");
                // }
            }
            printf("\n");
        }
        if (!strcmp(tokens[1], "-l"))
        {
            DIR *dir = opendir(directory_path);
            if (dir == NULL)
            {
                perror("opendir");
                return 1;
            }
            // int file_type[256];
            int index = 0;
            struct dirent *file;

            while ((file = readdir(dir)) != NULL)
            {
                if (strcmp(file->d_name, ".") && strcmp(file->d_name, "..") && file->d_name[0] != '.')
                {
                    char filePath[2305];
                    sprintf(filePath, "%s/%s", directory_path, file->d_name);
                    // printf("%c\n",(file->d_name)[0]);

                    struct stat file_stats;
                    if (stat(filePath, &file_stats) == 0)
                    {
                        char *file_permissions = get_premissions(file_stats.st_mode);

                        struct passwd *pw = getpwuid(file_stats.st_uid);
                        struct group *gr = getgrgid(file_stats.st_gid);

                        char time_buffer[80];
                        strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", localtime(&file_stats.st_mtime));

                        printf("%s  ", file_permissions);
                        printf("%d  ", (int)file_stats.st_nlink);
                        printf("%s  ", pw->pw_name);
                        printf("%s  ", gr->gr_name);
                        printf("%lld", (long long)file_stats.st_size);
                        long long size = (long long)file_stats.st_size;
                        int num_digits = 0;
                        while (size != 0)
                        {
                            size = size / 10;
                            num_digits += 1;
                        }
                        // printf("\n%d\n",num_digits);
                        for (int i = 0; i < (10 - num_digits); i++)
                        {
                            printf(" ");
                        }
                        printf("%s  ", time_buffer);

                        // printf("%s  ", file->d_name);
                        if (file_type[index] == 0)
                        {
                            printf("%s  ", files[index]);
                        }
                        if (file_type[index] == 1)
                        {
                            printf(GREEN "%s  " RESET, files[index]);
                        }
                        if (file_type[index] == 2)
                        {
                            printf(BLUE "%s  " RESET, files[index]);
                        }
                        printf("\n");
                    }
                    else
                    {
                        perror("stat");
                    }
                }
                index += 1;
            }
        }
        if (!strcmp(tokens[1], "-la") || !strcmp(tokens[1], "-al"))
        {
            DIR *dir = opendir(directory_path);
            if (dir == NULL)
            {
                perror("opendir");
                return 1;
            }
            int index = 0;
            struct dirent *file;
            while ((file = readdir(dir)) != NULL)
            {
                char filePath[2305];
                sprintf(filePath, "%s/%s", directory_path, file->d_name);
                // printf("%c\n",(file->d_name)[0]);

                struct stat file_stats;
                if (stat(filePath, &file_stats) == 0)
                {
                    char *file_permissions = get_premissions(file_stats.st_mode);

                    struct passwd *pw = getpwuid(file_stats.st_uid);
                    struct group *gr = getgrgid(file_stats.st_gid);

                    char time_buffer[80];
                    strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", localtime(&file_stats.st_mtime));

                    printf("%s  ", file_permissions);
                    printf("%d  ", (int)file_stats.st_nlink);
                    printf("%s  ", pw->pw_name);
                    printf("%s  ", gr->gr_name);
                    printf("%lld", (long long)file_stats.st_size);
                    long long size = (long long)file_stats.st_size;
                    // printf("\n%d",size);
                    int num_digits = 0;
                    while (size != 0)
                    {
                        size = size / 10;
                        num_digits += 1;
                    }
                    // printf("%d\n",num_digits);
                    for (int i = 0; i < (10 - num_digits); i++)
                    {
                        printf(" ");
                    }
                    printf("%s  ", time_buffer);
                    // printf("%s  ", file->d_name);
                    if (file_type[index] == 0)
                    {
                        printf("%s  ", files[index]);
                    }
                    if (file_type[index] == 1)
                    {
                        printf(GREEN "%s  " RESET, files[index]);
                    }
                    if (file_type[index] == 2)
                    {
                        printf(BLUE "%s  " RESET, files[index]);
                    }
                    printf("\n");
                }
                else
                {
                    perror("stat");
                }
                index += 1;
            }
        }
    }
    if (numTokens == 4)
    {
        DIR *dir = opendir(directory_path);
        if (dir == NULL)
        {
            perror("opendir");
            return 1;
        }
        int index = 0;
        struct dirent *file;
        while ((file = readdir(dir)) != NULL)
        {
            char filePath[2305];
            sprintf(filePath, "%s/%s", directory_path, file->d_name);
            // printf("%c\n",(file->d_name)[0]);

            struct stat file_stats;
            if (stat(filePath, &file_stats) == 0)
            {
                char *file_permissions = get_premissions(file_stats.st_mode);

                struct passwd *pw = getpwuid(file_stats.st_uid);
                struct group *gr = getgrgid(file_stats.st_gid);

                char time_buffer[80];
                strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", localtime(&file_stats.st_mtime));

                printf("%s   ", file_permissions);
                printf("%d   ", (int)file_stats.st_nlink);
                printf("%s   ", pw->pw_name);
                printf("%s   ", gr->gr_name);
                printf("%lld", (long long)file_stats.st_size);
                long long size = (long long)file_stats.st_size;
                int num_digits = 0;
                while (size != 0)
                {
                    size = size / 10;
                    num_digits += 1;
                }
                // printf("\n%d\n",num_digits);
                for (int i = 0; i < (10 - num_digits); i++)
                {
                    printf(" ");
                }
                printf("%s   ", time_buffer);
                // printf("%s   ", file->d_name);
                if (file_type[index] == 0)
                {
                    printf("%s  ", files[index]);
                }
                if (file_type[index] == 1)
                {
                    printf(GREEN "%s  " RESET, files[index]);
                }
                if (file_type[index] == 2)
                {
                    printf(BLUE "%s  " RESET, files[index]);
                }
                printf("\n");
            }
            else
            {
                perror("stat");
            }
            index += 1;
        }
    }
}