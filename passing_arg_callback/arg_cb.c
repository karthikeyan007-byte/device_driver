#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

// Variable to store the parameter
static int my_value = 50;  // Default value

// Custom "set" function
static int my_value_set(const char *val, const struct kernel_param *kp)
{
    int temp;
    int ret = kstrtoint(val, 10, &temp);  // Convert string to integer

    if (ret < 0) {
        printk(KERN_ERR "Invalid input for my_value\n");
        return ret;
    }

    if (temp < 0 || temp > 100) {
        printk(KERN_ERR "my_value must be between 0 and 100\n");
        return -EINVAL;  // Return error
    }

    printk(KERN_INFO "my_value changed from %d to %d\n", my_value, temp);
    my_value = temp;  // Update value

    return 0;
}

// Custom "get" function
static int my_value_get(char *buffer, const struct kernel_param *kp)
{
    return sprintf(buffer, "%d", my_value);  // Convert int to string
}

// Define parameter operations structure
const struct kernel_param_ops my_value_ops = {
    .set = my_value_set,  // Assign custom set function
    .get = my_value_get,  // Assign custom get function
};

// Register the module parameter with custom callback
module_param_cb(my_value, &my_value_ops, &my_value, 0660);
MODULE_PARM_DESC(my_value, "An integer parameter between 0 and 100");

static int __init my_driver_init(void)
{
    printk(KERN_INFO "Driver Loaded. my_value = %d\n", my_value);
    return 0;
}

static void __exit my_driver_exit(void)
{
    printk(KERN_INFO "Driver Unloaded\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("karthikeyan");
MODULE_DESCRIPTION("Example Driver using module_param_cb");
