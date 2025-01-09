#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>

#define procfs_name "tsu"
static struct proc_dir_entry *our_proc_file = NULL;

static long minutes_from_first_time_travel(void){
	
	long minutes_difference;
	time64_t first_travel_use, current_time;

	current_time = ktime_get_real_seconds();
	
	first_travel_use=mktime64(1985,10,5,1,21,0);

	minutes_difference = (current_time-first_travel_use)/60;
	
	return minutes_difference;
}
static ssize_t procfile_read(struct file *file_pointer,char __user *buffer, size_t buffer_length, loff_t* offset){
	long first_travel = minutes_from_first_time_travel();

	static int rd = 0;
	
	if (rd){
		return 0;
}
	rd =1;

	char s[9];
	int c;
	sprintf(s,"%ld\n",first_travel);
	c = copy_to_user(buffer,s,9);
	
	pr_info("procfile read %s %ld\n", file_pointer->f_path.dentry->d_name.name,first_travel);
	
	return 9;
}

static const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read,
};


static int __init procfs1_init(void) {
	our_proc_file = proc_create(procfs_name,0644,NULL,&proc_file_fops);
	pr_info("Module loaded");
	return 0;
}

static void __exit procfs1_exit(void) {
	proc_remove(our_proc_file);
	pr_info("/proc/tsu removed\n");
}

module_init(procfs1_init);
module_exit(procfs1_exit);

MODULE_LICENSE("GPL");
