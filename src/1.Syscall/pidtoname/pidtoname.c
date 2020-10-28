#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include "pidtoname.h"

asmlinkage long sys_process_id(int pid, char* buf, int len){
    struct task_struct *task;
    int len_process_name=0;
    
    for_each_process(task){
       
        if((int)task_pid_nr(task) == pid){
            len_process_name = strlen(task->comm);
            
            if(len >= len_process_name) {
                copy_to_user(buf,task->comm,len);
                return 0;
            }
            if(len < len_process_name)
                return len_process_name;
	
         }
    }
        
    return -1;
}


