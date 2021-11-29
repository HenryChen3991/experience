#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/slab.h> 
#include <linux/unistd.h> 
#include <linux/sched.h> 
#include <linux/fs.h> 
#include <linux/file.h> 
#include <linux/mm.h> 
#include <asm/uaccess.h> 
 
mm_segment_t oldfs; 
 
struct file *openFile(char *path,int flag,int mode) 
{ 
struct file *fp; 
 
fp=filp_open(path, flag, 0); 
if (fp) return fp; 
else return NULL; 
} 
 
int readFile(struct file *fp,char *buf,int readlen) 
{ 
if (fp->f_op && fp->f_op->read) 
return fp->f_op->read(fp,buf,readlen, &fp->f_pos); 
else 
return -1; 
} 
 
int closeFile(struct file *fp) 
{ 
filp_close(fp,NULL); 
return 0; 
} 
 
void initKernelEnv(void) 
{ 
oldfs = get_fs(); 
set_fs(KERNEL_DS); 
} 
 
static int __init readfile_init(void) 
{ 
char buf[1024]; 
struct file *fp; 
int ret; 
printk("Henry-- readfile init\n");
initKernelEnv(); 
fp=openFile("/home/disk_D/henry03/cTest/kernel_Test/config",O_RDONLY,0); 
if (fp!=NULL) 
{ 
memset(buf,0,1024); 
if ((ret=readFile(fp,buf,1024))>0) 
printk("buf:%s\n",buf); 
else printk("read file error %d\n",ret); 
closeFile(fp); 
} 
return 0; 
} 
 
static void __exit readfile_exit(void) 
{ 
 
printk("read file module remove successfully\n"); 
} 
module_init(readfile_init); 
module_exit(readfile_exit); 
 
MODULE_DESCRIPTION("read a file in kernel module"); 
MODULE_AUTHOR("Joey Cheng<jemicheng@gmail.com>"); 
MODULE_LICENSE("GPL"); 
MODULE_ALIAS("read file module");
