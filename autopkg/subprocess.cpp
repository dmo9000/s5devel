#include "subprocess.h"


int Subprocess::Wait()
{
    int wstatus = 0;
    pid_t wait_pid = -1;
    std::cout << "Waiting for childpid = " << childpid << std::endl;
    wait_pid = waitpid(childpid, &wstatus, 0);
    if (wait_pid != childpid) {
        std::cout << "wait_pid did not match childpid" << std::endl;
        _exit(1);
    }

    return wstatus;
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

    if (pipe(pipes[PARENT_STDOUT_PIPE]) < 0) {
        std::cout << "Couldn't create pipe_stdin" << std::endl;
        return 1;
    }
    if (pipe(pipes[PARENT_STDIN_PIPE]) < 0) {
        std::cout << "Couldn't create pipe_stdout" << std::endl;
        return 1;
    }

    childpid = fork();

    if (childpid < 0) {
        std::cout << "couldn't start subprocess" << std::endl;
        return 1;
    }

    /* otherwise, we are in parent or child */

    if (childpid == 0) {

        /* child */
        int d0 = 0, d1 = 0, d2 = 0;
        pid_t mypid = getpid();

        d0 = dup2(PARENT_STDIN_PIPE, STDIN_FILENO);
        std::cout << mypid << ": dupping " << STDIN_FILENO << " to " << PARENT_STDIN_PIPE << " == " << d0 <<std::endl;
        d1 = dup2(PARENT_STDOUT_PIPE, STDOUT_FILENO);
        std::cout << mypid << ": dupping " << STDOUT_FILENO << " to " << PARENT_STDOUT_PIPE << " == " << d1 << std::endl;
        d2 = dup2(PARENT_STDERR_PIPE, STDERR_FILENO);
        std::cout << mypid << ": dupping " << STDERR_FILENO << " to " << PARENT_STDERR_PIPE << " == " << d2 << std::endl;

        close(PARENT_STDIN_PIPE);
        close(PARENT_STDOUT_PIPE);
        close(PARENT_STDERR_PIPE);
        close(PARENT_STDIN_FD);
        close(PARENT_STDOUT_FD);
        close(PARENT_STDERR_FD);

        //close (2); /* stderr */

        if (execve(sp_command.c_str(), sp_argv, NULL) !=0 ) {
            std::cerr << "Couldn't start process" << std::endl;
            _exit(1);
        } else {
            std::cerr << "Process started" << std::endl;
        }
    } else {
        /* parent process */
	int flags = 0;

	/*
        close(PARENT_STDIN_PIPE);
        close(PARENT_STDOUT_PIPE);
        close(PARENT_STDERR_PIPE);
	*/
	/*
        flags = fcntl(PARENT_STDOUT_FD, F_GETFL, 0);
        fcntl(PARENT_STDIN_FD, F_SETFL, flags | O_NONBLOCK);
        flags = fcntl(PARENT_STDOUT_FD, F_GETFL, 0);
        fcntl(PARENT_STDOUT_FD, F_SETFL, flags | O_NONBLOCK);
        flags = fcntl(PARENT_STDOUT_FD, F_GETFL, 0);
        fcntl(PARENT_STDERR_FD, F_SETFL, flags | O_NONBLOCK);
	*/
    }

    std::cout << "parent process continuing, child pid is "
              << childpid << std::endl;


    return 0;

}

ssize_t Subprocess::WriteStdin(const char * data, size_t length)
{

    ssize_t wr = 0;
    wr = write(PARENT_STDIN_FD, data, length);
    std::cout << "Wrote " << wr << " bytes" << std::endl;
    if (wr == -1) {
        perror("write");
        exit(1);
    }
    return length;

}

ssize_t Subprocess::ReadStdout()
{
    char buffer[8192];
    ssize_t rd = 0;
    rd = read(PARENT_STDOUT_FD, &buffer, 8192);
    if (rd != -1) {
        std::cout << "Read " << rd << " bytes" << std::endl;
    }
    return rd;
}

ssize_t Subprocess::ReadStderr(char *b, int l)
{
    ssize_t rd = 0;
    //std::cout << "Trying to read from stderr ..." << std::endl;
    rd = read(PARENT_STDERR_FD, b,l );
    //std::cout << "Read " << rd << " bytes" << std::endl;
    if (rd > 0) {
        //std::cout << "Read " << rd << " bytes" << std::endl;
    }
    return rd;
}



