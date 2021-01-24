#pragma once
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


class Subprocess {

public:
    int Set_stdin();
    int Set_stdout();
    int Set_stderr();
    int SetCommand(std::string Command, int argc, char **argv);
    int StartProcess();
    int debug = 1;
    int Wait();



private:
    std::string sp_command;
    int sp_argc;
    char **sp_argv;
    int pipe_stdin[2];
    int pipe_stdout[2];
    int pipe_stderr[2];
    pid_t childpid = -1;

};
