#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/slab.h>
#include <linux/kern_levels.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/fdtable.h>
#include <asm/unistd.h>
#include <asm/paravirt.h>
#include <asm/cacheflush.h>
#include <asm/pgtable_types.h>


#define LICENSE "GPL"
#define VERSION "1.0"
#define AUTHOR "Quy Thanh - Tam Phuc - Thanh Vy"
#define DESCRIPTION "A module to hook syscall open and write"

MODULE_LICENSE(LICENSE);
MODULE_VERSION(VERSION);
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);


unsigned long** SYS_CALL_TABLE;

void EnablePageWriting(void) {
    write_cr0(read_cr0() & (~0x10000));
} 

void DisablePageWriting(void) {
    write_cr0(read_cr0() | 0x10000);
} 

asmlinkage int (*original_open)(const char* path, int flags);

asmlinkage ssize_t (*original_write)(int fd, const void* buf, size_t count);

asmlinkage int HookedOpen(const char* path, int flags) {
	char letter = 'a'; // dummy letter
	int i = 0;
	char dir[255];

	while (letter != 0) {
    	get_user(letter, path + i);
	dir[i++] = letter;
	}

    if (strcmp(dir, "\0") > 0 )
		printk(KERN_INFO "\"%s\" open \"%s\"\n", current->comm, dir);

	return (*original_open)(path, flags);
}

asmlinkage ssize_t HookedWrite(int fd, const void* buf, size_t count) {
	spin_lock(&current->files->file_lock);

    struct file* file = fcheck_files(current->files, fd);
    if (!file) { // no file
        spin_unlock(&current->files->file_lock);
        return -ENOENT; 
    }

    struct path* path = &file->f_path;
    path_get(path);

    spin_unlock(&current->files->file_lock);

    char* tmp = (char *)__get_free_page(GFP_KERNEL);
    if (!tmp) {
        path_put(path);
        return -ENOMEM;
    }

    char* pathName = d_path(path, tmp, PAGE_SIZE);
    path_put(path);
	if (IS_ERR(pathName)) {
        free_page((unsigned long)tmp);
        return PTR_ERR(pathName);
    }
    free_page((unsigned long)tmp);

	ssize_t bytes = (*original_write)(fd, buf, count);

	printk(KERN_INFO "\"%s\" write %ldB to \"%s\"\n", current->comm, bytes, pathName);

    return bytes;
}

static int __init HookInit(void) {
    SYS_CALL_TABLE = (unsigned long**)kallsyms_lookup_name("sys_call_table"); 

    EnablePageWriting();
    original_write = (void*)SYS_CALL_TABLE[__NR_write];
    SYS_CALL_TABLE[__NR_write] = (unsigned long*)HookedWrite;
	original_open = (void*)SYS_CALL_TABLE[__NR_open];
    SYS_CALL_TABLE[__NR_open] = (unsigned long*)HookedOpen;
    DisablePageWriting();

    printk(KERN_INFO "Hooked!\n");

    return 0;
}

static void __exit HookExit(void) {
    EnablePageWriting();
    SYS_CALL_TABLE[__NR_write] = (unsigned long*)original_write;
    SYS_CALL_TABLE[__NR_open] = (unsigned long*)original_open;
    DisablePageWriting();

    printk(KERN_INFO "Unhooked!\n");
}

module_init(HookInit);
module_exit(HookExit);
