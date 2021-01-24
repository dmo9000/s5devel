#include "subprocess.h"


int Subprocess::Wait()
{
    int wstatus = 0;
    std::cout << "Waiting for childpid = " << childpid << std::endl;
    waitpid(childpid, &wstatus, 0);
    return 0;
}

int Subprocess::SetCommand(std::string Command, int argc, char *argv[])
{

    int i = 0;
    if (debug) {
        std::cout << "Subprocess::SetCommand(" << Command << "," << argc << ", ";
        for (i = 0; i <= argc; i++) {
            if (argv[i] == NULL) {
                std::cout << "NULL" << ";";
            } else {
                std::cout << argv[i] << ", ";
            }
        }
        std::cout << ")" << std::endl;
    }

    sp_command = Command;
    sp_argc = argc;
    sp_argv = argv;
    return 0;

}

int Subprocess::StartProcess()
{

    if (debug) {
        std::cout << "Subprocess::StartProcess(" << sp_command << "... );" << std::endl;
    }

    if (pipe(pipe_stdin) < 0) {
        std::cout << "Couldn't create pipe_stdin" << std::endl;
        return 1;
    }
    if (pipe(pipe_stdout) < 0) {
        std::cout << "Couldn't create pipe_stdout" << std::endl;
        return 1;
    }
    if (pipe(pipe_stderr) < 0) {
        std::cout << "Couldn't create pipe_stderr" << std::endl;
        return 1;
    }

    childpid = fork();

    if (childpid < 0) {
        std::cout << "couldn't start subprocess" << std::endl;
        return 1;
    }

    /* otherwise, we are in parent or child */



    if (childpid == 0) {
        // char *argv_gunzip[] = { const_cast<char *>("/var/spool/pkg/S5LXcurl.pkg.gz"), NULL };
        std::cout << "child process starting ... " << std::endl;;
        /*
        close(pipe_stdin[0]);
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(pipe_stdout[1]);
        */

        close(pipe_stdin[0]);
        close (1);
        dup(pipe_stdin[1]);
        close (pipe_stdin[1]);
        close(pipe_stdout[1]);
        close (0);
        dup(pipe_stdout[0]);
        close(pipe_stdout[0]);
        if (execve(sp_command.c_str(), sp_argv, NULL) !=0 ) {
            std::cerr << "Couldn't start process" << std::endl;
            _exit(1);
        } else {
            std::cerr << "Process started" << std::endl;
        }
    } else {
        std::cout << "parent process continuing, child pid is " << childpid << std::endl;;
    }


    return 0;

}


