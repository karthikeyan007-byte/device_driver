// #include<linux/kernel.h>
// #include<linux/init.h>
// #include<linux/module.h>

// static int my_val=0;
// module_param(my_val,int,0660);
// MODULE_PARAM_DESC(my_val, "An integer value");
// static int __init arg_init(void)
//  {
//     printk(KERN_INFO "ARG Driver is loaded and my_value= %d",my_val);
//     return 0;
//  }
// static void __exit arg_exit(void)
// {
//     printk(KERN_INFO "ARG Driver is unloaded\n");

// }
// module_init(arg_init);
// module_exit(arg_exit);

// MODULE_AUTHOR("Karthikeyan");
// MODULE_LICENSE("GPL");
// MODULE_DESCRIPTION("PASSING ARGUMENTS IN THE TERMINAL TO THE DRIVER MODULE");



#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int my_value = 0;  // Default value

// Declaring the module parameter
module_param(my_value, int, 0660);
MODULE_PARM_DESC(my_value, "An integer parameter");

static int __init my_driver_init(void)
{
    printk(KERN_INFO "Args Driver Loaded. my_value = %d\n", my_value);
    return 0;
}

static void __exit my_driver_exit(void)
{
    printk(KERN_INFO "Args Driver Unloaded\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthikeyan");
MODULE_DESCRIPTION("Example Driver with Module Parameter");
