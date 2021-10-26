// MERT KALENDER - 26351

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


int main(){ //As default it prints to a.out as you know(only PID part). Could not achieve to change it to console. But of course relevant part is printed into output.txt

	pid_t cpid;
	
	cpid = fork();
	
	if(cpid < 0){
		perror("fork");
		exit(1);
	}
	else if(cpid == 0){
		
		int fd[2];
		
		if(pipe(fd) < 0){
			perror("pipe");
			exit(1);
		}
		
		cpid = fork();
		if(cpid < 0){
			perror("fork");
			exit(1);
		}
		
		else if(cpid == 0) { //grandchild process
			
			printf("I'm GREP process with PID: %d - My command is: grep -A 1 -- \" -c,\" \n", getpid());
			close(fd[1]); //close the write end
			dup2(fd[0],0);
			int pfd = open("output.txt", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
			dup2(pfd,1);
			char* arg[] = {"grep", "-A", "2", "--", " -c,", NULL};
			execvp(arg[0], arg);
			
		}
		
		
		else{ //child process
			printf("I'm MAN process with PID: %d - My command is: man touch \n", getpid());
			close(fd[0]); //close the read end
			dup2(fd[1],1); // grep writes to the pipe
			char* arg2[] = {"man", "touch", NULL};
			execvp(arg2[0], arg2);
		}
	}
	
	
	else{ //parent process
		printf("I'm SHELL process with PID: %d - Main command is: man touch | grep -A 1 -- \" -c,\" > output.txt\n", getpid());
		int rc_wait = wait(NULL);
		waitpid(cpid, NULL, 0);
		printf("I'm SHELL process with PID: %d - execution is completed, you can find the results at output.txt\n", getpid());
		
	}	
	return 0;		
}
