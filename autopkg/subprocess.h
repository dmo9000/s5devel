#pragma once
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define NUM_PIPES		2
#define PARENT_STDIN_PIPE	0
#define PARENT_STDOUT_PIPE	1
#define PARENT_STDERR_PIPE	2

#define READ_FD			0
#define WRITE_FD		1

#define	PARENT_STDOUT_FD  ( pipes[PARENT_STDOUT_PIPE][READ_FD]   )
#define	PARENT_STDIN_FD ( pipes[PARENT_STDIN_PIPE][WRITE_FD] )
#define PARENT_STDERR_FD (pipes[PARENT_STDERR_PIPE][READ_FD] )
#define	CHILD_STDIN_FD   ( pipes[PARENT_STDIN_PIPE][READ_FD]  )
#define	CHILD_STDOUT_FD  ( pipes[PARENT_STDOUT_PIPE][WRITE_FD] )
#define CHILD_STDERR_FD	 ( pipes[PARENT_STDERR_PIPE][WRITE_FD] )


class Subprocess {

public:
    int Set_stdin();
    int Set_stdout();
    int Set_stderr();
    int SetCommand(std::string Command, int argc, char **argv);
    int StartProcess();
    ssize_t WriteStdin(const char *data, size_t length);
    ssize_t ReadStdout();
    ssize_t ReadStderr(char *, int l);
    int debug = 1;
    int Wait();

private:
    std::string sp_command;
    int sp_argc;
    char **sp_argv;
    int pipes[NUM_PIPES][2];
    pid_t childpid = -1;
};
