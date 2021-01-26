#include <unistd.h>
#include <stdio.h>
 
/* since pipes are unidirectional, we need two pipes.
   one for data to flow from parent's stdout to child's
   stdin and the other for child's stdout to flow to
   parent's stdin */
 
#define NUM_PIPES          3 
 
#define PARENT_WRITE_PIPE  0
#define PARENT_READ_PIPE   1
 
int pipes[NUM_PIPES][3];
 
/* always in a pipe[], pipe[0] is for read and 
   pipe[1] is for write */
#define READ_FD  0
#define WRITE_FD 1
 
#define PARENT_STDOUT_FD ( pipes[PARENT_READ_PIPE][READ_FD]   )
#define PARENT_STDIN_FD  ( pipes[PARENT_WRITE_PIPE][WRITE_FD] )
 
#define CHILD_STDIN_FD   ( pipes[PARENT_WRITE_PIPE][READ_FD]  )
#define CHILD_STDOUT_FD  ( pipes[PARENT_READ_PIPE][WRITE_FD]  )
 
void main()
{
    int outfd[2];
    int infd[2];
     
    // pipes for parent to write and read
    pipe(pipes[PARENT_READ_PIPE]);
    pipe(pipes[PARENT_WRITE_PIPE]);
     
    if(!fork()) {
 
        dup2(CHILD_STDIN_FD, STDIN_FILENO);
        dup2(CHILD_STDOUT_FD, STDOUT_FILENO);
 
        /* Close fds not required by child. Also, we don't
           want the exec'ed program to know these existed */
        close(CHILD_STDIN_FD);
        close(CHILD_STDOUT_FD);
        close(PARENT_STDOUT_FD);
        close(PARENT_STDIN_FD);

	fprintf(stdout, "hello\n");
          
    } else {
        char buffer[100];
        int count;
 
        /* close fds not required by parent */       
        close(CHILD_STDIN_FD);
        close(CHILD_STDOUT_FD);
 
        // Write to child’s stdin
  
        // Read from child’s stdout
        count = read(PARENT_STDOUT_FD, buffer, sizeof(buffer)-1);
        if (count > 0) {
            buffer[count] = 0;
            printf("1> %s", buffer);
        } else {
            printf("received %d bytes from child's stdout\n", count);
        }
    }
}
