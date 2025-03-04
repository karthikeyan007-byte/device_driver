#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

static struct hrtimer my_hrtimer;  // Declare the high-resolution timer
static ktime_t kt;                 // Time variable

// Callback function that runs when the timer expires
static enum hrtimer_restart my_timer_callback(struct hrtimer *timer)
{
    printk(KERN_INFO "HRTimer Callback Executed\n");

    // Uncomment the below line if you want the timer to restart automatically
    // hrtimer_forward_now(timer, kt);
    // return HRTIMER_RESTART;

    return HRTIMER_NORESTART;  // Stop timer after execution
}

// Module initialization
static int __init my_module_init(void)
{
    printk(KERN_INFO "Initializing High-Resolution Timer Module\n");

    // Set the timer expiration to 50ms
    kt = ktime_set(0, 50000000); // 50ms (0 seconds, 50,000,000 nanoseconds)

    // Initialize the high-resolution timer
    hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    my_hrtimer.function = my_timer_callback;

    // Start the timer
    hrtimer_start(&my_hrtimer, kt, HRTIMER_MODE_REL);

    return 0;
}

// Module exit function
static void __exit my_module_exit(void)
{
    int ret;

    // Cancel the timer if it's still running
    ret = hrtimer_cancel(&my_hrtimer);
    if (ret)
        printk(KERN_INFO "HRTimer was active when cancelled\n");

    printk(KERN_INFO "Exiting High-Resolution Timer Module\n");
}

// Register module entry and exit points
module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Karthikeyan");
MODULE_DESCRIPTION("High-Resolution Timer Example");
