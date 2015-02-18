#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include<linux/sched.h>
#include <asm/uaccess.h>

int len,temp;

char *msg;

int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{

if(count>temp)
{
count=temp;
}
temp=temp-count;
copy_to_user(buf,msg, count);
if(count==0)
temp=len;
   
return count;
}

struct file_operations proc_fops = {
read: read_proc
};
void create_new_proc_entry(void) 
{
proc_create("hello",0,NULL,&proc_fops);

msg=" Hello World ";
len=strlen(msg);
temp=len;
printk(KERN_INFO "1.len=%d",len);
}


int proc_init (void) {
 create_new_proc_entry();
 return 0;
}

void proc_cleanup(void) {
 remove_proc_entry("hello",NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
