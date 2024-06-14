#include "headers.h"

int proclore(char *command)
{
    char *tokens[100];
    int number_of_tokens = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        tokens[number_of_tokens++] = strdup(token);
        token = strtok(NULL, " ");
    }
    char status_path[1038];
    char proc_id[1024];
    char mem_path[1038];
    // printf("YES");
    if (number_of_tokens == 1)
    {
        int proc_id = getpid();

        sprintf(status_path, "/proc/%d/status", proc_id);

        FILE *status = fopen(status_path, "r");
        printf("Process id: %d\n", proc_id);
        char buffer[2048];
        int count = 0;
        while (fgets(buffer, sizeof(buffer), status))
        {
            count = count + 1;
            if (count == 3)
            {
                printf("Process Status: %c\n", buffer[7]);
                break;
            }
            // printf("%s", buffer);
        }
        pid_t pgid = getpgid(proc_id);
        printf("Process Group: %d\n", pgid);

        sprintf(mem_path, "/proc/%d/statm", proc_id);
        FILE *memory_file = fopen(mem_path, "r");

        // Virtual Memory
        unsigned long vm_size;
        fscanf(memory_file, "%lu", &vm_size);
        printf("Virtual Memory: %lu KB\n", vm_size);
        char exe_path[1038];
        sprintf(exe_path, "/proc/%d/exe", proc_id);

        char resolved_path[1038];
        ssize_t len = readlink(exe_path, resolved_path, sizeof(resolved_path) - 1);
        // printf("Execution Path: %s\n",resolved_path);
        if (len != -1)
        {
            resolved_path[len] = '\0';
            printf("Execution Path: %s\n", resolved_path);
        }
    }
    if (number_of_tokens == 2)
    {
        strcpy(proc_id, tokens[1]);
        int pid = atoi(proc_id);
        int result = kill(pid, 0);

        if (result == 0)
        {
            // printf("Process with PID %d exists.\n", pid);
        }
        else
        {
            printf("Process with PID %d does not exist.\n", pid);
            return 0;
        }

        // strcpy(status_path, "/proc");
        // strcat(status_path, proc_id);
        // strcat(status_path, "/status");

        sprintf(status_path, "/proc/%s/status", proc_id);

        FILE *status = fopen(status_path, "r");

        char buffer[256];
        int count = 0;
        printf("Process id: %s\n", proc_id);
        while (fgets(buffer, sizeof(buffer), status))
        {
            count = count + 1;
            if (count == 3)
            {
                printf("Process Status: %c\n", buffer[7]);
                break;
            }
        }
        int p_id = atoi(proc_id);
        // printf("\n%d\n",p_id);
        pid_t pgid = getpgid(p_id);
        printf("Process Group: %d\n", pgid);

        // strcpy(mem_path, "/proc");
        // strcat(mem_path, proc_id);
        // strcat(mem_path, "/statm");

        sprintf(mem_path, "/proc/%s/statm", proc_id);
        FILE *memory_file = fopen(mem_path, "r");

        // Virtual Memory
        unsigned long vm_size;
        if (fscanf(memory_file, "%lu", &vm_size) == 1)
        {
            printf("Virtual Memory: %lu KB\n", vm_size);
        }

        // FILE *memory = fopen(mem_path, "r");
        char exe_path[1038];
        sprintf(exe_path, "/proc/%s/exe", proc_id);

        char resolved_path[1038];
        ssize_t len = readlink(exe_path, resolved_path, sizeof(resolved_path) - 1);
        // printf("Execution Path: %s\n",resolved_path);
        if (len != -1)
        {
            resolved_path[len] = '\0';
            printf("Execution Path: %s\n", resolved_path);
        }
    }
}