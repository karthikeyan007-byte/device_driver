#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>


static int my_array[3] = {11, 22, 33};  // Default values
static int arr_len = 3;

// Register array parameter
module_param_array(my_array, int, &arr_len, 0660);
MODULE_PARM_DESC(my_array, "An array of integers");

static int __init my_driver_init(void)
{
    int i;
    printk(KERN_INFO "Driver Loaded. Array values: ");
    for (i = 0; i < arr_len; i++) {
        printk(KERN_INFO "my_array[%d] = %d", i, my_array[i]);
    }
    return 0;
}

static void __exit my_driver_exit(void)
{
    printk(KERN_INFO "My Driver Unloaded\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);


MODULE_AUTHOR("kARTHIK");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PASSING ARRAY AS ARGUMENT IN DRIVER MODULE");
