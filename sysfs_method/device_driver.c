#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>


static struct kobject *kobj_ref;  // Pointer to sysfs kobject
static int etx_value = 0;         // Variable to expose in sysfs

// Function to show the value when reading from sysfs
static ssize_t etx_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", etx_value);
}

// Function to store the value when writing to sysfs
static ssize_t etx_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &etx_value);
    return count;
}

// Creating a sysfs attribute
static struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, etx_show, etx_store);

// Module Initialization
static int __init sysfs_example_init(void)
{
    // Create a directory in /sys/kernel/
    kobj_ref = kobject_create_and_add("etx_sysfs", kernel_kobj);
    if (!kobj_ref) {
        pr_err("Cannot create kobject\n");
        return -ENOMEM;
    }

    // Create the file associated with etx_value
    if (sysfs_create_file(kobj_ref, &etx_attr.attr)) {
        pr_err("Cannot create sysfs file.\n");
        kobject_put(kobj_ref);
        return -ENOMEM;
    }

    pr_info("Sysfs example module loaded\n");
    return 0;
}

// Module Exit
static void __exit sysfs_example_exit(void)
{
    // Remove sysfs file and directory
    sysfs_remove_file(kobj_ref, &etx_attr.attr);
    kobject_put(kobj_ref);
    pr_info("Sysfs example module unloaded\n");
}

module_init(sysfs_example_init);
module_exit(sysfs_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthikeyan");
MODULE_DESCRIPTION("A simple Linux sysfs example driver");
