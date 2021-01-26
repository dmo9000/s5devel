#include "subprocess.h"


int Subprocess::Wait()
{
    int wstatus = 0;
    pid_t wait_pid = -1;
    if (debug) {
        std::cout << "Waiting for childpid = " << childpid << std::endl;
    }
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

    if (pipe(pipes[STDIN_PIPE]) < 0) {
        std::cout << "Couldn't create pipe_stdout" << std::endl;
        return 1;
    }

    if (pipe(pipes[STDOUT_PIPE]) < 0) {
        std::cout << "Couldn't create pipe_stdin" << std::endl;
        return 1;
    }

    if (pipe(pipes[STDERR_PIPE]) < 0) {
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

        d0 = dup2(CHILD_STDIN_FD, STDIN_FILENO);
        d1 = dup2(CHILD_STDOUT_FD, STDOUT_FILENO);
        d2 = dup2(CHILD_STDERR_FD, STDERR_FILENO);

        close(CHILD_STDIN_FD);
        close(CHILD_STDOUT_FD);
        close(CHILD_STDERR_FD);

        close(PARENT_STDIN_FD);
        close(PARENT_STDOUT_FD);
        close(PARENT_STDERR_FD);

        if (execve(sp_command.c_str(), sp_argv, NULL) !=0 ) {
            std::cerr << "Couldn't start process" << std::endl;
            _exit(1);
        } else {
            std::cerr << "Process started" << std::endl;
        }
    } else {
        /* parent process */
        int flags = 0;

        close(CHILD_STDIN_FD);
        close(CHILD_STDOUT_FD);
        close(CHILD_STDERR_FD);

        /* set to non blocking mode on all fd's */

        flags = fcntl(PARENT_STDOUT_FD, F_GETFL, 0);
        fcntl(PARENT_STDIN_FD, F_SETFL, flags | O_NONBLOCK);
        flags = fcntl(PARENT_STDOUT_FD, F_GETFL, 0);
        fcntl(PARENT_STDOUT_FD, F_SETFL, flags | O_NONBLOCK);
        flags = fcntl(PARENT_STDOUT_FD, F_GETFL, 0);
        fcntl(PARENT_STDERR_FD, F_SETFL, flags | O_NONBLOCK);

        /* open as streams */

        child_stdin = fdopen(PARENT_STDIN_FD, "wb");
        child_stdout = fdopen(PARENT_STDOUT_FD, "rb");
        child_stderr = fdopen(PARENT_STDERR_FD, "rb");

        assert(child_stdin);
        assert(child_stdout);
        assert(child_stderr);


    }

    if (debug) {
        std::cout << "parent process continuing, child pid is "
                  << childpid << std::endl;
    }


    return 0;

}

ssize_t Subprocess::WriteStdin(char *b, ssize_t l)
{

    /* bypasses buffering and sends data directly to stdin file descriptor */
    ssize_t wr = 0;

    wr = write(PARENT_STDIN_FD, b, l);
    if (wr == -1) {
        if (errno != EAGAIN) {
            perror("write");
            exit(1);
        } else {
            return 0;
        }
    }
    return wr;

}

ssize_t Subprocess::ReadStdout(char *b, ssize_t l)
{
    /* bypasses buffering and returns data directly from stdout file descriptor */
    ssize_t rd = 0;
    //std::cout << "Trying to read from stdout ..." << std::endl;
    rd = read(PARENT_STDOUT_FD, b,l );
    //std::cout << "Read " << rd << " bytes" << std::endl;
    if (rd > 0) {
        //std::cout << "Read " << rd << " bytes" << std::endl;
    }
    return rd;

}

ssize_t Subprocess::ReadStderr(char *b, ssize_t l)
{
    /* bypasses buffering and returns data directly from stderr file descriptor */
    ssize_t rd = 0;
    //std::cout << "Trying to read from stderr ..." << std::endl;
    rd = read(PARENT_STDERR_FD, b,l );
    //std::cout << "Read " << rd << " bytes" << std::endl;
    if (rd > 0) {
        //std::cout << "Read " << rd << " bytes" << std::endl;
    }
    return rd;
}

ssize_t Subprocess::BufferStderr()
{
    ssize_t rd = 0;
    char *bufptr = (char *) &buf_stderr;
    bufptr += len_stderr;

    if ((BUFSIZE - len_stderr - 1) <= 0) {
	DumpStderr(child_stderr);
        std::cerr << "Not enough room in STDERR buffer. (len_stderr = " << len_stderr << ")" << std::endl;
        assert(NULL);
    }
    rd = read(PARENT_STDERR_FD, bufptr, BUFSIZE - len_stderr - 1);
    if (rd > 0) {
        len_stderr += rd;
        //std::cout << len_stderr <<":[" << buf_stderr << "]" << std::endl;
    }
    /*
    std::cout << "+++ BufferStderr received " << rd << ", length=" << len_stderr << std::endl;
    if (len_stderr > 0) {
    	assert(NULL);
        	}
    */
    /* be paranoid about terminating the buffer */
    buf_stderr[BUFSIZE-1] = '\0';
    return len_stderr;
}

bool Subprocess::StderrContains(char *s)
{
    if (strstr(buf_stderr, s)) {
        return true;
    }

    return false;

}

void Subprocess::DumpStderr(FILE *fh)
{

    fprintf(fh, "%s", buf_stderr);
    return;
}

void Subprocess::ClearStderr()
{

    memset(&buf_stderr, 0, BUFSIZE);
    len_stderr = 0;
    return;

}
