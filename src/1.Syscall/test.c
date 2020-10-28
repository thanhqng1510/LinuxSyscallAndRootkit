#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
int main(){
	printf("Choose the following options:\n1. pnametoid \n2. pidtoname\n");
	int choose;
	printf("You choose: ");
	scanf("%d", &choose);
	if(choose == 1){
		char name[100];
		printf("Enter your name: ");
		scanf("%s", name);
		strtok(name, "\n");
		int pid = syscall(351, name);
		printf("System call return %d\n", pid); 	
	}
	else if(choose == 2){
		char bufName[100];
		int pid;
		printf("Enter your pid: ");
		scanf("%d", &pid);
		int result = syscall(352, pid, bufName, 100);
		printf("System call returned %d\n", result);
		if(result == 0){
			printf("Your process name: %s\n", bufName);
		}
		else if (result == -1){
			printf("Cannot find the process with pid = %d\n", pid);
		}
		else {
			printf("Buff must have %d bytes \n", result);
		}
	}
       	return 0;
}
