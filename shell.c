#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>

typedef int bool;
#define true 1
#define false 0

char** resetCmds(int num_row, int num_col) {
	int i;
	char** list;
	list = malloc(sizeof(char*)*num_row);
	for(i=0;i<num_row; i++) 
		list[i] = malloc(sizeof(char)*num_col);
	return list;
}

void freeMem(char** list, int number_of_row) {
	int i;
	for(i=0;i<number_of_row; i++) 
		free(list[i] );
	free(list); 
}

void getCmds(char** list, int num_rows) {
	int i;
	for (i = 0; i < num_rows; i++) {
		printf("%s %d", list[i], i);
	}
}

char** getInput(char* input, char* temp, int num_row, int num_col) {
			// We will use num_row-1 as a way to signify a NULL ending to the arguments
			int i, k, cmd_index;
			i = 0;
			k = 0;
			cmd_index = 0;
			memset(temp, 0, num_col);
			char **cmd_array;
			cmd_array = resetCmds(num_row, num_col);
			// While there is still stuff in the output buffer
			while (input[i] != '\n' && cmd_index < num_row-1) {
			// if we've reach a space, store temp in the cmd_array and reset temp
				if (input[i] == ' ') {
					// Put the cmd in the array and reset temp
					strcpy(cmd_array[cmd_index++], temp);
					memset(temp, 0, num_col);
					k = 0;
					i++;
				}
				temp[k] = input[i];
				k++;
				i++;			
			}	
			// Put the cmd in the array and reset temp
			// we must do this one more time for the last word
			k = 0;
			if (cmd_index < num_row-1) {
				strcpy(cmd_array[cmd_index++], temp);
			}
			cmd_array[cmd_index] = NULL;
			memset(temp, 0, num_col);
			return cmd_array;
};

char** getAllCmds() {
	char** allCmds;
	allCmds = resetCmds(20, 15);
	strcpy(allCmds[0], "ls");
	strcpy(allCmds[1], "cat");
	strcpy(allCmds[2], "rmdir");
	strcpy(allCmds[3], "mv");
	strcpy(allCmds[4], "rm");
	strcpy(allCmds[5], "mkdir");
	strcpy(allCmds[6], "pwd");
	strcpy(allCmds[7], "grep");
	strcpy(allCmds[8], "ff");
	strcpy(allCmds[9], "who");
	strcpy(allCmds[10], "chmod");
	strcpy(allCmds[11], "more");
	strcpy(allCmds[12], "cp");
	strcpy(allCmds[13], "gzip");
	strcpy(allCmds[14], "man");
	strcpy(allCmds[15], "passwd");
	strcpy(allCmds[16], "mk");
	strcpy(allCmds[17], "exit");
	strcpy(allCmds[18], "exit");
	strcpy(allCmds[19], "exit");
	return allCmds;
}

char** getSpecialCmds() {
	char** specialCmds;
	specialCmds = resetCmds(8, 15);
	strcpy(specialCmds[0], ">");
	strcpy(specialCmds[1], ">>");
	strcpy(specialCmds[2], "1>");
	strcpy(specialCmds[3], "2>");
	strcpy(specialCmds[4], "2>>");
	strcpy(specialCmds[5], "&");
	strcpy(specialCmds[6], "<");
	strcpy(specialCmds[7], "|");
	return specialCmds;
}

int verifySpecial(char** cmd_array) {
	char** all_cmds;
	all_cmds = getSpecialCmds();
	int i, j;
	i = 0;
	while (cmd_array[i]) {
		for (j = 0; j < 8; j++) {
			if (strcmp(cmd_array[i], all_cmds[j]) == 0) {
				return i;
			}
		}
		i++;
	}
	/**/
	return -1;
}

char* verifyCmd(char** cmd_array) {
	char** all_cmds;
	int num_of_cmds;
	
	all_cmds = getAllCmds();
	int i, j;
	i = 0;
	while (cmd_array[i]) {
		for (j = 0; j < 20; j++) {
			if (strcmp(cmd_array[i], all_cmds[j]) == 0) {
				return cmd_array[i];
			}
		}
		i++;
	}
	/**/
	return "ERROR";
}

// This is not used
char** getArgs(char* cmd, char** allInputs) {
	char** returnedInputs;
	int i;
	i = 0; 
	while (allInputs[i] != NULL) {
		strcpy(returnedInputs[i], allInputs[i]);
		i++;	
	}
	return returnedInputs;
}

int getLength(char** in) {
	int i;
	i = 0;
	while(in[i]) {
		i++;
	}
	return i;
}

char** setFirstCmd(char** cmd_array, int specialIndex) {	
	char** first;
	first = resetCmds(specialIndex, 15);
	int i;
	i = 0;	
	while(i < specialIndex) {
		strcpy(first[i], cmd_array[i]);
		i++;
	}
	return first;
}

char** setSecondCmd(char** cmd_array, int specialIndex, int length) {
	char** second;
	int i, j, k;
	k = (length -(specialIndex + 1));
	second = resetCmds(k, 15);
	j = 0;
	i = specialIndex + 1;
	while(j < k) {
		strcpy(second[j], cmd_array[i]);
		j++;
		i++;
	}
	return second;
}

void redirection(char** first, char** second, char* special) {
	pid_t childPid;
	if (strcmp(special, "<") == 0) {
		childPid = fork();
		if (childPid == 0) {
			int fd0 = open(second[0], O_RDONLY, 0);
			dup2(fd0, STDIN_FILENO);
			close(fd0);
			execvp(verifyCmd(first), first);
			perror("< failed");
            exit(1);
		} else {
			wait(0);
		}	
	} else if (strcmp(special, "|") == 0) {
		int des_p[2];
        if(pipe(des_p) == -1) {
          perror("Pipe failed");
          exit(1);
        }
        childPid = fork();
		if (childPid == 0) {       //first fork
            close(1);          //closing stdout
            dup(des_p[1]);     //replacing stdout with pipe write 
            close(des_p[0]);   //closing pipe read
            close(des_p[1]);
            execvp(verifyCmd(second), second);
            perror("execvp of 1 failed");
            exit(1);
        }
        if(fork() == 0)        //creating 2nd child
        {
            close(0);          //closing stdin
            dup(des_p[0]);     //replacing stdin with pipe read
            close(des_p[1]);   //closing pipe write
            close(des_p[0]);
            execvp(verifyCmd(first), first);
			perror("execvp of 2 failed");
            exit(1);
        }
        close(des_p[0]);
        close(des_p[1]);
        wait(0);
        wait(0);
        return;
	
	} else if ((strcmp(special, ">") == 0) || (strcmp(special, "1>") == 0)) {
		childPid = fork();
		if (childPid == 0) {
			int fd1 = creat(second[0], 0644);
			dup2(fd1, STDOUT_FILENO);
			close(fd1);
			execvp(verifyCmd(first), first);
			perror("> failed");
            exit(1);
		} else {
			wait(0);
		}	
	} else if (strcmp(special, ">>") == 0) {
		childPid = fork();
		if (childPid == 0) {
		int file = open(second[0],
        O_CREAT | O_RDWR | O_APPEND,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		dup2(file,1);
		close(file);
		execvp(verifyCmd(first), first);
			perror("> failed");
            exit(1);
		} else {
			wait(0);
		}	
	} else if (strcmp(special, "2>") == 0) {
		childPid = fork();
		if (childPid == 0) {
			int fd1 = creat(second[0], 0644);
			dup2(fd1, STDERR_FILENO);
			close(fd1);
			execvp(verifyCmd(first), first);
			perror("2> failed");
            exit(1);
		} else {
			wait(0);
		}			
	} else if (strcmp(special, "2>>") == 0) {
		childPid = fork();
		if (childPid == 0) {
			int file = open(second[0],
			O_CREAT | O_RDWR | O_APPEND,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
			dup2(file,STDERR_FILENO);
			close(file);
			execvp(verifyCmd(first), first);
			perror("2>> failed");
            exit(1);
		} else {
			int returnStatus;    
			waitpid(childPid, &returnStatus, 0);  // Parent process waits here for child to terminate.
			if (returnStatus == 0) { // Verify child process terminated without error.  

			} else if (returnStatus == 1) {
			   printf("The child process terminated with an error!.\n");    
			}
		}	
	}

}

bool startProcess(char** cmd_array, int num_row) {
	char* cmd;
	int lengthOfCmds;
	int specialIndex;
	pid_t childPid;
	cmd = verifyCmd(cmd_array);
	if (strcmp("exit", cmd) == 0)
		return false;
	specialIndex = verifySpecial(cmd_array);
	if (strcmp("ERROR", cmd) == 0) {
		printf("Couldn't find cmd\n");
	} else if (specialIndex != -1) {
		lengthOfCmds = getLength(cmd_array);
		char** first;
		char** second;
		first = setFirstCmd(cmd_array, specialIndex);
		second = setSecondCmd(cmd_array, specialIndex, lengthOfCmds);
		int i, j;
		i = 0;
		j = 0;
		redirection(first, second, cmd_array[specialIndex]);
	} else {
		childPid = fork();
		if (childPid == 0) {
		   execvp(cmd, cmd_array);
		   perror("Error\n");
		} else if (childPid < 0) {
			printf("Error in child\n");
		} else {
			int returnStatus;    
			waitpid(childPid, &returnStatus, 0);  // Parent process waits here for child to terminate.
			if (returnStatus == 0) { // Verify child process terminated without error.  

			} else if (returnStatus == 1) {
			   printf("The child process terminated with an error!.\n");    
			}
		}
	}
	
	return true;
}

int main() {

	// Set up variables
	bool should_run = true;
	int i, k, iterator, array_size;
	int num_row;
	int num_col;
	pid_t child_in;
	
	// Define variables
	num_row = 15;
	num_col = 200;
	char input[num_col];
	char temp[num_col];
	char **cmd_array;
	
	// Shell should run unless input is exit
	while(should_run) {
	
		// Reset counter for input
		printf("osh>");
		
		// Get input 
		fgets(input, sizeof(input), stdin);
		
		// Break up input into array
		cmd_array = getInput(input, temp, num_row, num_col);
		
		should_run = startProcess(cmd_array, num_row);
		
		// Free the memory
		freeMem(cmd_array, num_row);
		}
}

