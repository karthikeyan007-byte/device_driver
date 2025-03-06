
/* driver2.c - Importing function and variable */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

extern int etx_count;
extern void etx_shared_func(void);
dev_t dev = 0;
static struct cdev etx_cdev;

static int etx_open(struct inode *inode, struct file *file) { return 0; }
static int etx_release(struct inode *inode, struct file *file) { return 0; }
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    etx_shared_func();
    pr_info("Shared function called %d times\n", etx_count);
    return 0;
}
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) { return len; }

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = etx_read,
    .write = etx_write,
    .open = etx_open,
    .release = etx_release,
};

static int __init etx_driver_init(void) {
    if (alloc_chrdev_region(&dev, 0, 1, "etx_Dev2") < 0) return -1;
    cdev_init(&etx_cdev, &fops);
    if (cdev_add(&etx_cdev, dev, 1) < 0) return -1;
    pr_info("Device Driver 2 Inserted\n");
    return 0;
}

static void __exit etx_driver_exit(void) {
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver 2 Removed\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("EXPORT_SYMBOL Driver - 2");