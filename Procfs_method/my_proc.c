#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "etx_proc"
#define PROC_DIR "etx"
#define PROC_BUF_SIZE 100

static struct proc_dir_entry *parent;
static char proc_data[PROC_BUF_SIZE] = "try_proc_array\n";
static int proc_data_len = 0;

static int open_proc(struct inode *inode, struct file *file) {
    pr_info("proc file opened.\n");
    return 0;
}

static int release_proc(struct inode *inode, struct file *file) {
    pr_info("proc file released.\n");
    return 0;
}

static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    if (*offset > 0 || proc_data_len == 0) {
        return 0;  // EOF
    }

    if (copy_to_user(buffer, proc_data, proc_data_len)) {
        pr_err("Failed to send data to user\n");
        return -EFAULT;
    }

    *offset = proc_data_len;
    return proc_data_len;
}

static ssize_t write_proc(struct file *filp, const char __user *buff, size_t len, loff_t *off) {
    if (len > PROC_BUF_SIZE - 1) {
        len = PROC_BUF_SIZE - 1;
    }

    if (copy_from_user(proc_data, buff, len)) {
        pr_err("Failed to receive data from user\n");
        return -EFAULT;
    }

    proc_data[len] = '\0';  // Ensure null termination
    proc_data_len = len;

    pr_info("proc file wrote: %s\n", proc_data);
    return len;
}

static struct proc_ops proc_fops = {
    .proc_open = open_proc,
    .proc_read = read_proc,
    .proc_write = write_proc,
    .proc_release = release_proc,
};

static int __init etx_procfs_init(void) {
    parent = proc_mkdir(PROC_DIR, NULL);
    if (!parent) {
        pr_err("Failed to create /proc/%s\n", PROC_DIR);
        return -ENOMEM;
    }
    
    if (!proc_create(PROC_NAME, 0666, parent, &proc_fops)) {
        pr_err("Failed to create /proc/%s/%s\n", PROC_DIR, PROC_NAME);
        proc_remove(parent);
        return -ENOMEM;
    }
    
    pr_info("Procfs module loaded successfully\n");
    return 0;
}

static void __exit etx_procfs_exit(void) {
    proc_remove(parent);
    pr_info("Procfs module unloaded\n");
}

module_init(etx_procfs_init);
module_exit(etx_procfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthikeyan");
MODULE_DESCRIPTION("Simple Linux procfs example");
MODULE_VERSION("1.0");
