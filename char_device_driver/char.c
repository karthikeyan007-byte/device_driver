#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>  // For class_create(), device_create()

#define DEVICE_NAME "my_char_device"

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static char device_buffer_read[100] = "Hello from kernel\n";
static char device_buffer[100] = "Hello from kernel\n";

static int my_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t my_read(struct file *filp, char __user *buffer, size_t len, loff_t *offset) {
    int bytes_to_copy = min(len, strlen(device_buffer));

    if (*offset >= strlen(device_buffer_read)) {
        return 0;  // Prevent infinite loop
    }

    if (copy_to_user(buffer, device_buffer_read, bytes_to_copy)) {
        return -EFAULT;
    }

    *offset += bytes_to_copy;
    return bytes_to_copy;
}

static ssize_t my_write(struct file *filp, const char __user *buffer, size_t len, loff_t *offset) {
    int bytes_to_copy = min(len, sizeof(device_buffer) - 1);
    
    if (copy_from_user(device_buffer, buffer, bytes_to_copy)) {
        return -EFAULT;
    }
    
    device_buffer[bytes_to_copy] = '\0';
    printk(KERN_INFO "Received from user: %s\n", device_buffer);
    return bytes_to_copy;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

static int __init my_init(void) {
    // Allocate major & minor numbers dynamically
    if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "Failed to allocate device numbers\n");
        return -1;
    }

    // Initialize character device
    cdev_init(&my_cdev, &fops);
    if (cdev_add(&my_cdev, dev_num, 1) < 0) {
        printk(KERN_ALERT "Failed to add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    // Create device class (visible in /sys/class/)
    my_class = class_create(DEVICE_NAME);
    if (IS_ERR(my_class)) {
        printk(KERN_ALERT "Failed to create class\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_class);
    }

    // Create device file (visible in /dev/)
    my_device = device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        printk(KERN_ALERT "Failed to create device\n");
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_device);
    }

    printk(KERN_INFO "Character device driver loaded\n");
    return 0;
}

static void __exit my_exit(void) {
    // Remove device file
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    
    // Remove character device
    cdev_del(&my_cdev);
    
    // Free allocated device number
    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "Character device driver unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Character Device Driver with Automatic Device File Creation");
