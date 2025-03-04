#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/kobject.h>
#include <linux/workqueue.h>  // Workqueue header

#define DEVICE_NAME "etx_device"
#define IRQ_NO 11  // Interrupt request number (for testing)

static int irq = IRQ_NO, dev_major;
static struct cdev etx_cdev;
static struct class *etx_class;
static struct kobject *etx_kobj;
static int irq_counter = 0;  // Counter for IRQ triggers
static struct work_struct etx_work;  // Work structure

// Workqueue function to process IRQ in process context
static void etx_work_function(struct work_struct *work) {
    printk(KERN_INFO "ETX Workqueue: Processing deferred work. IRQ Count = %d\n", irq_counter);
}

// Interrupt Handler
static irqreturn_t irq_handler(int irq, void *dev_id) {
    irq_counter++;  // Increment counter
    printk(KERN_INFO "ETX Interrupt Handler: Interrupt Occurred! Count = %d\n", irq_counter);
    schedule_work(&etx_work); // Schedule work on global workqueue
    return IRQ_HANDLED;
}

// Sysfs write function to trigger the interrupt manually
static ssize_t trigger_irq(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    printk(KERN_INFO "ETX: Triggering IRQ Manually\n");
    irq_handler(IRQ_NO, NULL);  // Manually call the IRQ handler
    return count;
}

// Sysfs read function to get the IRQ count
static ssize_t irq_count_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", irq_counter);
}

// Create sysfs attributes
static struct kobj_attribute irq_attr = __ATTR(trigger_irq, 0660, NULL, trigger_irq);
static struct kobj_attribute irq_count_attr = __ATTR(irq_count, 0444, irq_count_show, NULL);

// File operations structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
};

// Module Init Function
static int __init etx_driver_init(void) {
    int ret;
    dev_t dev;

    printk(KERN_INFO "Initializing ETX Interrupt Driver with Global Workqueue\n");

    // Allocate device number dynamically
    ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "ETX: Failed to allocate device number\n");
        return ret;
    }
    dev_major = MAJOR(dev);

    // Register character device
    cdev_init(&etx_cdev, &fops);
    etx_cdev.owner = THIS_MODULE;
    ret = cdev_add(&etx_cdev, dev, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev, 1);
        printk(KERN_ERR "ETX: Failed to add cdev\n");
        return ret;
    }

    // Create device class and device file
    etx_class = class_create("etx_class");
    device_create(etx_class, NULL, dev, NULL, DEVICE_NAME);

    // Initialize work structure for global workqueue
    INIT_WORK(&etx_work, etx_work_function);

    // Register interrupt
    ret = request_irq(irq, irq_handler, IRQF_SHARED, "etx_device", (void *)(irq_handler));
    if (ret) {
        printk(KERN_ERR "ETX: Cannot register IRQ\n");
        device_destroy(etx_class, dev);
        class_destroy(etx_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
        return ret;
    }

    // Create sysfs entry
    etx_kobj = kobject_create_and_add("etx_sysfs", kernel_kobj);
    if (sysfs_create_file(etx_kobj, &irq_attr.attr) || sysfs_create_file(etx_kobj, &irq_count_attr.attr)) {
        printk(KERN_ERR "ETX: Failed to create sysfs entries\n");
    }

    printk(KERN_INFO "ETX Interrupt Driver with Global Workqueue Initialized Successfully!\n");
    return 0;
}

// Module Exit Function
static void __exit etx_driver_exit(void) {
    dev_t dev = MKDEV(dev_major, 0);

    // Remove sysfs entry
    sysfs_remove_file(etx_kobj, &irq_attr.attr);
    sysfs_remove_file(etx_kobj, &irq_count_attr.attr);
    kobject_put(etx_kobj);

    // Free IRQ
    free_irq(irq, (void *)(irq_handler));

    // Cleanup character device
    device_destroy(etx_class, dev);
    class_destroy(etx_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);

    printk(KERN_INFO "ETX Interrupt Driver with Global Workqueue Unloaded\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthikeyan");
MODULE_DESCRIPTION("Interrupt Example Driver with Sysfs, IRQ Counter, and Global Workqueue");
