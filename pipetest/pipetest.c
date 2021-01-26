#include <unistd.h>
#include <stdio.h>
 
/* since pipes are unidirectional, we need two pipes.
   one for data to flow from parent's stdout to child's
   stdin and the other for child's stdout to flow to
   parent's stdin */
 
#define NUM_PIPES          3 
 
#define STDIN_PIPE    0
#define STDOUT_PIPE   1
#define STDERR_PIPE   2 
 
int pipes[NUM_PIPES][3];
 
/* always in a pipe[], pipe[0] is for read and 
   pipe[1] is for write */
#define READ_FD  0
#define WRITE_FD 1
 
#define PARENT_STDIN_FD  ( pipes[STDIN_PIPE][WRITE_FD] )
#define PARENT_STDOUT_FD ( pipes[STDOUT_PIPE][READ_FD]   )
#define PARENT_STDERR_FD ( pipes[STDERR_PIPE][READ_FD]   )
 
#define CHILD_STDIN_FD   ( pipes[STDIN_PIPE][READ_FD]  )
#define CHILD_STDOUT_FD  ( pipes[STDOUT_PIPE][WRITE_FD]  )
#define CHILD_STDERR_FD  ( pipes[STDERR_PIPE][WRITE_FD]  )
 
void main()
{
     
    // pipes for parent to write and read
    pipe(pipes[STDOUT_PIPE]);
    pipe(pipes[STDIN_PIPE]);
    pipe(pipes[STDERR_PIPE]);
     
    if(!fork()) {
 
        dup2(CHILD_STDIN_FD, STDIN_FILENO);
        dup2(CHILD_STDOUT_FD, STDOUT_FILENO);
        dup2(CHILD_STDERR_FD, STDERR_FILENO);
 
        /* Close fds not required by child. Also, we don't
           want the exec'ed program to know these existed */
        close(CHILD_STDIN_FD);
        close(CHILD_STDOUT_FD);
        close(PARENT_STDOUT_FD);
        close(PARENT_STDIN_FD);

	fprintf(stdout, "stdout, hello\n");
	fprintf(stderr, "stderr, hello\n");
          
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

        // Read from child’s stderr
        count = read(PARENT_STDERR_FD, buffer, sizeof(buffer)-1);
        if (count > 0) {
            buffer[count] = 0;
            printf("2> %s", buffer);
        } else {
            printf("received %d bytes from child's stderr\n", count);
        }


    }
}
