#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main() {
    enum {READ, WRITE};     // pipe file descriptors READ=0 WRITE=1
    int fd[2], fd_gc[2];
    pid_t proc_id, proc_id_gchild;
    char buffer[100], buffer_gc[100];

    int pipe_success = pipe(fd);
    if (pipe_success < 0) {
        perror("error creating pipe");
    }
    
    proc_id = fork();
    if (proc_id < 0) {
        perror("fork error");
    }

    if (proc_id_gchild == 0) {

            cout << "\t\tin grandchild process" << endl;
            close(fd_gc[WRITE]);
            int n_gc = read(fd_gc[READ], buffer_gc, 100);
            write(STDOUT_FILENO, buffer_gc, 100);
            cout << "\t\tending grandchild process" << endl;

    }
    else if (proc_id == 0) {         // proc_id for child process == 0
        // commented below line to add a grandchild process
        // close(fd[WRITE]);       // child has fd also; close WRITE pipe
        cout << "\tin child process" << endl;
        write(fd[WRITE], "\t\t**** This is the grandchild - HOW DOES IT WORK???\n", 52);
        cout << "\twrote to grandchild read pipe from child process; about to hit WAIT call in child process" << endl;
        wait(NULL);
        pid_t proc_id_gchild = fork();
        if (proc_id_gchild < 0) {
            perror("gchild fork error");
        }
        int pipe_gc_success = pipe(fd_gc);
        if (pipe_gc_success < 0) {
            perror("error creating granchild pipe");
        }

        int n = read(fd[READ], buffer, 100);    // read child's READ pipe connection
        cout << "\tnow writing data to console in child fork" << endl;
        write(STDOUT_FILENO, buffer, n);
        cout << "\tending child process" << endl;
    }       // end child logical block
    else {
        close(fd[READ]);        // parent has fd; close READ pipe
        cout << "writing data to PIPE in parent" << endl;
        write(fd[WRITE], "\tWow does this WORK???\n", 23);
        wait(NULL);
        cout << "ending parent process" << endl;
    }

    return 0;
}