#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/completion.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct completion my_completion;
static struct task_struct *thread1, *thread2;

// Thread function that waits for completion
static int waiter_thread(void *data) {
    pr_info("Waiting for completion...\n");
    wait_for_completion(&my_completion);
    pr_info("Completion received! Resuming execution.\n");
    return 0;
}

// Thread function that triggers completion
static int signal_thread(void *data) {
    pr_info("Sleeping for 3 seconds before signaling completion...\n");
    msleep(3000);
    complete(&my_completion);
    pr_info("Completion signaled!\n");
    return 0;
}

static int __init completion_example_init(void) {
    init_completion(&my_completion);

    thread1 = kthread_run(waiter_thread, NULL, "waiter_thread");
    thread2 = kthread_run(signal_thread, NULL, "signal_thread");

    return 0;
}

static void __exit completion_example_exit(void) {
    pr_info("Exiting completion module.\n");
}

module_init(completion_example_init);
module_exit(completion_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthikeyan");
MODULE_DESCRIPTION("Linux Kernel Completion Example");
