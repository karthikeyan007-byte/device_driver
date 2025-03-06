#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "my_misc_device"

// Message to be read by userspace
static char message[] = "Hello from misc device\n";
static size_t message_len = sizeof(message);

// Read function
static ssize_t misc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    if (*ppos >= message_len)
        return 0; // EOF

    if (count > message_len - *ppos)
        count = message_len - *ppos;

    if (copy_to_user(buf, message + *ppos, count))
        return -EFAULT;

    *ppos += count;
    return count;
}

// Write function
static ssize_t misc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    char kernel_buf[100];

    if (count > sizeof(kernel_buf) - 1)
        count = sizeof(kernel_buf) - 1;

    if (copy_from_user(kernel_buf, buf, count))
        return -EFAULT;

    kernel_buf[count] = '\0';
    pr_info("Misc Device Received: %s\n", kernel_buf);

    return count;
}

// File operations structure
static const struct file_operations misc_fops = {
    .owner = THIS_MODULE,
    .read = misc_read,
    .write = misc_write,
};

// Misc device structure
static struct miscdevice my_misc_device = {
    .minor = MISC_DYNAMIC_MINOR, // Dynamic minor number allocation
    .name = DEVICE_NAME,         // Device name in /dev/
    .fops = &misc_fops,          // File operations
};

// Module initialization function
static int __init misc_init(void) {
    int ret;

    ret = misc_register(&my_misc_device);
    if (ret) {
        pr_err("Failed to register misc device\n");
        return ret;
    }

    pr_info("Misc device registered as /dev/%s\n", DEVICE_NAME);
    return 0;
}

// Module exit function
static void __exit misc_exit(void) {
    misc_deregister(&my_misc_device);
    pr_info("Misc device unregistered\n");
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("karthikeyan");
MODULE_DESCRIPTION("A simple misc device driver example");
