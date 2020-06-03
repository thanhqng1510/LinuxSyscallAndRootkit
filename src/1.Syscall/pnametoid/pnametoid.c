#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/string.h>
#include "pnametoid.h"
asmlinkage long sys_process_name(char* process_name){
    struct task_struct *task; 
    char name[32];

    copy_from_user(name, process_name, 32);

    for_each_process(task){
	
        if(strcmp(task->comm, name) == 0){
            return task_pid_nr(task);

        }
    }
    return -1;
}

