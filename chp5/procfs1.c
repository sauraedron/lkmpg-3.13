#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>


MODULE_LICENSE("GPL");

#define procfs_name "Helloworld"
int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);
struct proc_dir_entry *opf;
static const struct file_operations fops = {	
	.owner = THIS_MODULE,
	.open = procfile_read
};

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;
	printk(KERN_INFO "Inside procfile_read /proc/%s\n",procfs_name);
	if(offset > 0)
	{
		ret=0;
	}
	else
	{
		ret = sprintf(buffer, "Helloworld\n");
	}
	return ret;
}
static int __init i_module(void)
{
	printk(KERN_INFO "Inserting the module\n");
  opf = proc_create(procfs_name, 0, NULL, &fops);
  if(!opf)
    return -ENOMEM;
  return 0;
}
static void __exit e_module(void)
{
  remove_proc_entry(procfs_name, NULL);
  printk(KERN_INFO "Removing the module owner %s\n", fops.owner);
}


module_init(i_module);
module_exit(e_module);



