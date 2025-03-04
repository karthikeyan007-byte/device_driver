#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>   // For kmalloc, kfree
#include <linux/list.h>   // Linked list API

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthikeyan");
MODULE_DESCRIPTION("Kernel Linked List Example");

// Function Prototypes
void add_device(int id);
void print_devices(void);
void delete_devices(void);

// Define a structure containing data and list node
struct device_node {
    int id;
    struct list_head list;  // Linked list node
};

// Declare and initialize the linked list head
static LIST_HEAD(device_list);

// Function to add a device node
void add_device(int id) {
    struct device_node *new_node;

    new_node = kmalloc(sizeof(*new_node), GFP_KERNEL);
    if (!new_node)
        return;

    new_node->id = id;
    INIT_LIST_HEAD(&new_node->list);

    list_add_tail(&new_node->list, &device_list);  // Add at the end
    pr_info("Added Device ID: %d\n", id);
}

// Function to print all devices
void print_devices(void) {
    struct device_node *node;
    
    pr_info("Device List:\n");
    list_for_each_entry(node, &device_list, list) {
        pr_info("Device ID: %d\n", node->id);
    }
}

// Function to delete all devices
void delete_devices(void) {
    struct device_node *node, *temp;
    
    list_for_each_entry_safe(node, temp, &device_list, list) {
        pr_info("Deleting Device ID: %d\n", node->id);
        list_del(&node->list);
        kfree(node);
    }
}

// Module init function
static int __init linked_list_init(void) {
    pr_info("Initializing Linked List Module\n");

    add_device(101);
    add_device(102);
    add_device(103);
    
    print_devices();
    return 0;
}

// Module exit function
static void __exit linked_list_exit(void) {
    pr_info("Exiting Linked List Module\n");
    delete_devices();
}

module_init(linked_list_init);
module_exit(linked_list_exit);
