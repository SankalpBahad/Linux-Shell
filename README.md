# Description

run "make" followed by "./a.out"

Commands implemented are:
1. peek 
2. seek
3. warp
4. proclore
5. system commands
6. pastevents
7. ping
8. activities
9. iMan
10. neonate
11. fg and bg

Color encoding: directories blue, files white, executables green.

# Assumptions

1. File path lengths are fixed
2. For background processes, put a space between command and '&', for example (sleep 5 &)
3. For 'fg' command, it only executes the background process in the foreground if it has stopped in the background. Else, it does nothing.
4. For 'bg' command, a new process with a different pid is created instead of reviving the old process, i.e, the activity of the specified pid is executed but the new process has different pid.
5. For iMan, the name of command is only specified under the Name section and not in the top right and top left corners.
6. The pastevents command sometimes prints extra '\n' characters in the terminal.
7. On pressing Ctrl-C when a process is not running, it shows ^C but if enter is pressed or command is typed, it works normally.
8. On pressing Ctrl-D, "Killed" is displayed.

