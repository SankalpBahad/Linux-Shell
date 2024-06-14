#include "headers.h"

int fg(int pid)
{
    char* command=get_activity(pid);
    int l=strlen(command);
    // printf("'%s'\n",command);
    // return 1;
    char new_comm[256];
    strncpy(new_comm,command,l-2);
    // printf("%s\n",new_comm);
    system_calls(new_comm);
    return 1;
}
int bg(int pid)
{
    char* command=get_activity(pid);
    // printf("%s\n",command);
    system_calls(command);
    remove_from_activities(pid);
    return 1;
}