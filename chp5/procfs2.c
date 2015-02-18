#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#define PROCFS_MAX_SIZE 1024
#define PROCFS_NAME "bufferlink"

static struct proc_dir_entry *opf;
int procfile_write(struct file *file, const char *buffer, unsigned long count, void *data);
int procfile_read(char *buffer, char **bufferlocation, off_t offset, int buffer_length, int *eof, void *data);
static struct file_operations fops =	{
	.owner = THIS_MODULE,
	.write = procfile_write,
	.read = procfile_read
};
static char procfs_buffer[PROCFS_MAX_SIZE];

static unsigned long procfs_buffer_size = 0;

int procfile_read(char *buffer, char **bufferlocation, off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;
	printk(KERN_INFO "procfile read /proc/%s called \n", PROCFS_NAME);
if(offset>0)	
	ret = 0;
else
{
	memcpy(buffer, procfs_buffer, procfs_buffer_size);
	ret  = procfs_buffer_size;
}
return ret;
}
int procfile_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	buffer = "AYO";
	procfs_buffer_size = count;
	/* get buffer size */
	if(procfs_buffer_size > PROCFS_MAX_SIZE)
	{
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}
	/* write to data */
	if(copy_from_user(procfs_buffer, buffer, procfs_buffer_size))
		return -EFAULT;

return procfs_buffer_size;

}

int init_module()
{
	opf = proc_create(PROCFS_NAME, 0, NULL, &fops);
	if(!opf)	{
		remove_proc_entry(PROCFS_NAME, NULL);
		printk(KERN_ALERT "Error Could not initialize /proc/%s \n", PROCFS_NAME);
		return -ENOMEM;
	}
	printk(KERN_INFO "Process created /proc/%s\n", PROCFS_NAME);

	return 0;
}

void cleanup_module()
{
	remove_proc_entry(PROCFS_NAME, NULL);
	printk(KERN_INFO "REmoved %s", PROCFS_NAME);
}
