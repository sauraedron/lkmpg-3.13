#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/init.h>

#define PROC_ENTRY_FILENAME "buffer2k"
#define PROCFS_MAX_SIZE 2048

static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long int proc_fs_buffer = 0;
static  struct proc_dir_entry *opf;
static unsigned int procfs_buffer_size = 0;

static ssize_t procfs_read(struct file *flip,
						 	char *buffer,
						 	 size_t length,
						 	  loff_t *offset)
{
	static int finished = 0;

	if(finished)	{
		printk(KERN_INFO "procfs_read END\n");
		finished = 0;
		return 0;
	}
	finished = 1;
	/* copy_to_user_page(vma, page, vaddr, dst, src, len) */
	if(copy_to_user(buffer, procfs_buffer, procfs_buffer_size))
	{
		return -EFAULT;
	}
	printk(KERN_INFO "procfs read : %int bytes \n", procfs_buffer_size);
	return procfs_buffer_size ;
}

static ssize_t procfs_write(struct file *flie,
 const char *buffer,
  size_t len, loff_t *off)
{
	if(len > PROCFS_MAX_SIZE)
	{
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}
	else
	{
		procfs_buffer_size = len;
	}
	if(copy_from_user(procfs_buffer, buffer, procfs_buffer_size));
		return procfs_buffer_size;
}


/** 	int (*permission) (struct inode *, int); */
static int module_permission(struct inode *inode, int op)
{
	/** We allow everybody to read from our module , but only root uid = 0 may write to it*/
	if(op==4 || (op ==2 && current->pid == 0))
		return 0;
	

	return -EACCES;
}

/** file is open */

int procfs_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}

int procfs_close(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0;
}

static struct file_operations fops_4 = {
	.read = procfs_read,
	.write  = procfs_write,
	.open = procfs_open,
	.release =procfs_close,
};

/*
Inode operations for our proc file. We need it so we will have som place to specify the file operations
structure we want to use, and the function we use for permissions. It's also possible to sepcify functions
to be called for anything else which could be done to an inode*/

static struct inode_operations Inode_ops = {
	.permission = module_permission
};

int init_module()
{
	opf = proc_create(PROC_ENTRY_FILENAME, 0, NULL, &fops_4);

	if(opf==NULL)
	{
		printk(KERN_ALERT "Error \n");
		return -ENOMEM;
	}
	//opf->owner = THIS_MODULE;
	printk(KERN_INFO"STUff created\n");
	return 0;
}
void cleanup_modules(void)
{
	remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
	printk(KERN_INFO"stuff removed\n");
}