/*Andrew Kwak 
* CSS430 Project 1 Part 1 Proccesses
* Piping for this program will go from top to bottom to produce an output. 
* Uses nested if statements. 
* Calling forks up front would have also worked. 
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <iostream> 
#include <cassert>

using namespace std;

int main (int argc, char* argv[]) {
	// read = 0, write = 1
	enum {read, write};
	// 2 Pipes. 
	// [0] is read, [1] is write, [2] is error. 
	// For the sake of making it easier for me to follow, names will be pretty literal. 
	int pipe1[2];
	int pipe2[2];

	// Create the first pipe. 
	// Error if pipe fails to create. 
	if (pipe(pipe1) == -1) {
		perror("pipe1 failed");		
	} 
	// Child. 
	// pid_t is a signed integer which is capable of representing a process ID. 
	pid_t pid;  
	// If pid is -1 the fork failed.
	
	if ((pid = fork()) == -1) {
		perror("fork failed");	  	
	} 
	// If pid is set to 0, it's a child.
	if (pid == 0) {
		dup2(pipe1[write], STDOUT_FILENO);
		close(pipe1[read]);
		execlp("/bin/ps", "ps", "-A", nullptr);		
		perror("Unable to execute ps");	

	// Parent Process.
	} else {
		wait(nullptr);
	}
	// If pid is greater than 0, this is now a parent. 
	if (pid > 0) {
		//Error if pipe fails to create. 
		if (pipe(pipe2) == -1) {
			perror("pipe2 failed");		
		}
		//Grandchild. grep.
		if ((pid = fork()) == -1) {
		perror("fork failed");	  	
		} 
		if (pid == 0) {
			// Redirect and close. 
			dup2(pipe1[read], STDIN_FILENO);
			dup2(pipe2[write], STDOUT_FILENO);
			close(pipe2[read]);
			close(pipe1[write]); 
			execlp("/bin/grep", "grep", argv[1], nullptr);	
			perror("Unable to execute grep");			
		}
		// Nested if statement for the next child. 
		else if (pid > 0) {
			// Great Grandchild. wc. 
			if ((pid = fork()) == -1) {
			perror("fork failed");	  	
			} 
			if (pid == 0) {
				// Redirect and close. 
				dup2(pipe2[read], STDIN_FILENO);
				close(pipe1[read]);
				close(pipe1[write]);
				close(pipe2[write]);
				execlp("/usr/bin/wc", "wc", "-l", nullptr);	
				perror("Unable to execute wc");
			}
		} 
	} 
	return 0;
}
