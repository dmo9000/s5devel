#pragma once
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

#define BUFSIZE	65536

#define NUM_PIPES		3
#define STDIN_PIPE		0
#define STDOUT_PIPE		1
#define STDERR_PIPE		2

#define READ_FD			0
#define WRITE_FD		1

#define	PARENT_STDIN_FD  ( pipes[STDIN_PIPE][WRITE_FD] )
#define	PARENT_STDOUT_FD ( pipes[STDOUT_PIPE][READ_FD] )
#define PARENT_STDERR_FD ( pipes[STDERR_PIPE][READ_FD] )

#define	CHILD_STDIN_FD   ( pipes[STDIN_PIPE][READ_FD]   )
#define	CHILD_STDOUT_FD  ( pipes[STDOUT_PIPE][WRITE_FD] )
#define CHILD_STDERR_FD	 ( pipes[STDERR_PIPE][WRITE_FD] )


class Subprocess {

public:
    int Set_stdin();
    int Set_stdout();
    int Set_stderr();
    int SetCommand(std::string Command, int argc, char **argv);
    int StartProcess();
    int debug = 0;
    int Wait();
    ssize_t BufferStderr();
    void ClearStderr();
    void DumpStderr(FILE *);

    /* should be protected ? */
    ssize_t WriteStdin(char *, ssize_t l);
    ssize_t ReadStdout(char *, ssize_t l);
    ssize_t ReadStderr(char *, ssize_t l);
    bool StderrContains(char *s);
private:
    std::string sp_command;
    int sp_argc;
    char **sp_argv;
    int pipes[NUM_PIPES][2];
    pid_t childpid = -1;
    /* FIXME: create buffer class for these maybe? */
    FILE *child_stdin = NULL;
    FILE *child_stdout = NULL;
    FILE *child_stderr = NULL;
    char buf_stdin[BUFSIZE+1];
    char buf_stdout[BUFSIZE+1];
    char buf_stderr[BUFSIZE+1];
    ssize_t len_stdin = 0;
    ssize_t len_stdout = 0;
    ssize_t len_stderr = 0;
};
