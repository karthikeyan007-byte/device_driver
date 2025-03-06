#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/seqlock.h>
#include <linux/kthread.h>
#include <linux/delay.h>

// Shared resource (e.g., temperature reading)
static int shared_temperature = 25;  // Initial value

// SeqLock declaration
seqlock_t temp_seqlock;

// Writer and Reader threads
static struct task_struct *writer_thread;
static struct task_struct *reader_thread1;
static struct task_struct *reader_thread2;

// Writer function: updates temperature periodically
static int writer_function(void *data) {
    while (!kthread_should_stop()) {
        write_seqlock(&temp_seqlock);  // Lock for writing
        shared_temperature++;  // Simulating temperature change
        pr_info("Writer: Updated temperature to %d°C\n", shared_temperature);
        write_sequnlock(&temp_seqlock);  // Unlock after writing

        msleep(1000); // Sleep for 1 second
    }
    return 0;
}

// Reader function: Reads temperature using SeqLock
static int reader_function(void *data) {
    int thread_id = *(int *)data;
    int local_temp;
    unsigned int seq;

    while (!kthread_should_stop()) {
        do {
            seq = read_seqbegin(&temp_seqlock); // Start reading
            local_temp = shared_temperature;   // Read shared data
        } while (read_seqretry(&temp_seqlock, seq)); // Retry if data changed

        pr_info("Reader %d: Read temperature = %d°C\n", thread_id, local_temp);
        msleep(500); // Sleep for 0.5 seconds
    }
    return 0;
}

// Module Initialization
static int __init seqlock_demo_init(void) {
    static int id1 = 1, id2 = 2;

    pr_info("SeqLock Demo Module Loaded\n");

    // Initialize SeqLock
    seqlock_init(&temp_seqlock);

    // Create Writer Thread
    writer_thread = kthread_run(writer_function, NULL, "writer_thread");
    if (IS_ERR(writer_thread)) {
        pr_err("Failed to create writer thread\n");
        return PTR_ERR(writer_thread);
    }

    // Create Reader Threads
    reader_thread1 = kthread_run(reader_function, &id1, "reader_thread1");
    if (IS_ERR(reader_thread1)) {
        pr_err("Failed to create reader thread 1\n");
        kthread_stop(writer_thread);
        return PTR_ERR(reader_thread1);
    }

    reader_thread2 = kthread_run(reader_function, &id2, "reader_thread2");
    if (IS_ERR(reader_thread2)) {
        pr_err("Failed to create reader thread 2\n");
        kthread_stop(writer_thread);
        kthread_stop(reader_thread1);
        return PTR_ERR(reader_thread2);
    }

    return 0;
}

// Module Cleanup
static void __exit seqlock_demo_exit(void) {
    pr_info("SeqLock Demo Module Unloading\n");

    // Stop Threads
    kthread_stop(writer_thread);
    kthread_stop(reader_thread1);
    kthread_stop(reader_thread2);
}

module_init(seqlock_demo_init);
module_exit(seqlock_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("SeqLock Demo in Linux Kernel");
