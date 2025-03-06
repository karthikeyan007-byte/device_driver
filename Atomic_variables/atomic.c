#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/err.h>

atomic_t etx_global_variable = ATOMIC_INIT(0);
unsigned int etc_bit_check = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
static struct task_struct *etx_thread1;
static struct task_struct *etx_thread2;

static int etx_open(struct inode *inode, struct file *file) {
    pr_info("Device File Opened\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file) {
    pr_info("Device File Closed\n");
    return 0;
}

int thread_function1(void *pv) {
    while (!kthread_should_stop()) {
        atomic_inc(&etx_global_variable);
        pr_info("Function1 [value : %u]\n", atomic_read(&etx_global_variable));
        msleep(1000);
    }
    return 0;
}

int thread_function2(void *pv) {
    while (!kthread_should_stop()) {
        atomic_inc(&etx_global_variable);
        pr_info("Function2 [value : %u]\n", atomic_read(&etx_global_variable));
        msleep(1000);
    }
    return 0;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = etx_open,
    .release = etx_release,
};

static int __init etx_driver_init(void) {
    if ((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) < 0) {
        pr_info("Cannot allocate major number\n");
        return -1;
    }

    cdev_init(&etx_cdev, &fops);
    if ((cdev_add(&etx_cdev, dev, 1)) < 0) {
        pr_info("Cannot add the device\n");
        goto r_class;
    }

    if (IS_ERR(dev_class = class_create("etx_class"))) {
        pr_info("Cannot create class\n");
        goto r_class;
    }

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device"))) {
        pr_info("Cannot create device\n");
        goto r_device;
    }

    etx_thread1 = kthread_run(thread_function1, NULL, "eTx Thread1");
    if (!etx_thread1) {
        pr_err("Cannot create kthread1\n");
        goto r_device;
    }

    etx_thread2 = kthread_run(thread_function2, NULL, "eTx Thread2");
    if (!etx_thread2) {
        pr_err("Cannot create kthread2\n");
        goto r_device;
    }

    pr_info("Device Driver Insert...Done\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    cdev_del(&etx_cdev);
    return -1;
}

static void __exit etx_driver_exit(void) {
    kthread_stop(etx_thread1);
    kthread_stop(etx_thread2);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver Remove...Done\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthikeyan");
MODULE_DESCRIPTION("A simple device driver - Atomic Variables");
MODULE_VERSION("1.27");
