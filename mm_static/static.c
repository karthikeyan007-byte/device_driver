#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>

dev_t dev = MKDEV(235,0);

static int __init static_init(void)
{   register_chrdev_region(dev, 1, "static_example_Dev");
    printk(KERN_INFO "Welcome to this program\n");
    printk(KERN_INFO "Major = %d and Minor = %d\n",MAJOR(dev),MINOR(dev));
    printk(KERN_INFO "Linux Device Module is successfully loaded\n");
    return 0;

}

static void __exit static_exit(void)
{
    unregister_chrdev_region(dev,1);
    printk(KERN_INFO "Linux Device Module is Unloaded\n");

}

module_init(static_init);
module_exit(static_exit);

MODULE_AUTHOR("KARTHIKEYAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("THIS MODULE DEFINES HOW MAJOR AND MINOR NUMBER IS STATICALLY ALLOCATED\n");

