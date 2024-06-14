#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <termios.h>


#define YELLOW "\x1B[33m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"
#define BLUE "\x1B[34m"


#include "prompt.h"

void warp(char *c, char *in_add, char *cur_add);
int peek(char *command);
int seek(char* command);
int proclore(char* command);
int seek_recursive(char *rel_path, char *target, int flag, int flag2);
int pastevents(char *command, char **past_commands, int num_commands, char* initial_address, char* current_address);
void execute(char *command, char **past_commands, int command_count, char *initial_address, char* current_address);
int ping(char *command);
int activities(char *command);
int system_calls(char *command);
char* io_redrn(char *command, int inp_flg, int out_flg, int over_flg, char **past_commands, int command_count, char *initial_address, char* current_address);
int pipes(char *command, char **past_commands, int command_count, char *initial_address, char* current_address);
void add_to_activities(char *command, int pid);
void remove_from_activities(int pid);
char* get_activity(int pid);
// void get_activity(int pid);
int fg(int pid);
int bg(int pid);
int iman(char* command);
int neonate(char* command);

#endif