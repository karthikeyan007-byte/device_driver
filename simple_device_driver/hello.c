/***************************************************************************//**
*  \file       hello.c
*
*  \details    Simple hello world driver
*
*  \author     
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
 
/*
** Module Init function
*/
static int __init hello_world_init(void)
{
    printk(KERN_INFO "Hello World\n");
    printk(KERN_INFO "My name is Karthik - Junior Engineer\n");
    printk(KERN_INFO "Hello Kernel Module Inserted Successfully...\n");
    return 0;
}

/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    printk(KERN_INFO "Hello Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthik");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("2:1.0");
