#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <iostream> 
#include <cassert>

using namespace std;

int main () {
	// 2 Pipes. 
	// [0] is read, [1] is write, [2] is error. 
	// For the sake of making it easier for me to follow, names will be pretty literal. 
	int pipe1[2];
	int pipe2[2];
	// pid_t is a signed integer which is capable of representing a process ID. 
	//pid_t pid; 

	//======================================Pipes======================================//
	// Error if pipe fails to create. 
	if (pipe(pipe1) == -1) {
		perror("pipe1 failed");
		exit(1);
	} else {
		cout << "pipe1 created" << endl;
	}

	//======================================Forks======================================//

	// Child. wc -l 
	int status;

	pid_t child = fork(); 
	// If pid is -1 the fork failed.
	if (child == -1) {
		cout << "Failed child fork" << endl; 
		exit(1);
	} 
	// If pid is set to 0, it's a child.
	else if (child == 0) {
		cout << "Child" << endl; 
		// child(pipe1, pipe2);
		dup2(pipe1[0], STDIN_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);
		execlp("/usr/bin/wc", "wc", "-l", nullptr);	
		perror("Unable to execute wc");
		exit(1);
	}

	// Error if pipe fails to create. 
	if (pipe(pipe2) == -1) {
		perror("pipe2 failed");
		exit(1);
	} else {
		cout << "pipe2 created"<< endl;
	}

	// Grandchild. grep argv[1].
	pid_t grandchild = fork(); 
	if (grandchild == -1) {
		cout << "Failed child fork" << endl; 
		exit(1);
	} 
	else if (grandchild == 0) {
		// child(pipe1, pipe2).
		cout << "Grandchild" << endl; 
		dup2(pipe1[0], STDIN_FILENO);
		dup2(pipe2[1], STDOUT_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
		execlp("/bin/grep", "grep", "argv[1]", nullptr);	
		perror("Unable to execute grep");
		exit(1);
	}

	close(pipe1[0]);
	close(pipe1[1]);

	// Great Grandchild. ps -A. 
	pid_t greatgrandchild = fork();
	if (greatgrandchild == -1) {
		cout << "Failed child fork" << endl; 
		exit(1);
	} 
	else if (greatgrandchild == 0) {
		cout << "Great Grandchild" << endl; 
		// child(pipe1, pipe2);
		dup2(pipe2[1], STDOUT_FILENO);
		close(pipe2[0]);
		close(pipe2[1]);
		execlp("/bin/ps", "ps", "-A", nullptr);	
		perror("Unable to execute ps");
		exit(1); 	
	} 
	else {
		// Back to the parent. 
		wait(&status);
		wait(&status);
		cout << "Back on Parent" << endl; 
	}
	return 0;
}
//execlp("/usr/bin/wc", "wc", "-l", nullptr);
// void child (int pipe1[2], int pipe2[2]) {
// 	dup2(pipe1[1], STDOUT_FILENO);
// 	close(pipe1[0]);
// 	close(pipe1[1]);
// 	execlp("/usr/bin/wc", "wc", "-l", nullptr);
// 	perror("Unable to execute wc");
// 	exit(1);
// }

// void grandchild (int pipe1[2], int pipe2[2]) {
	
// }

// void greatgrandchild (int pipe1[2], int pipe2[2]) {
	
// }