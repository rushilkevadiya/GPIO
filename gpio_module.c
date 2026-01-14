#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

/* Proc file names */
#define PROC_LED_FILE     "led"
#define PROC_BUTTON_FILE  "button"

/* Simulated GPIO states */
static int led_status = 0;        // 0 = LED OFF, 1 = LED ON
static int button_status = 0;     // 0 = Button released, 1 = Button pressed

/* Proc file entry pointers */
static struct proc_dir_entry *led_proc_entry;
static struct proc_dir_entry *button_proc_entry;

/* ---------------------------------------------------
 * Generic write handler (User → Kernel)
 * --------------------------------------------------- */
static ssize_t write_gpio_state(struct file *file,
                                const char __user *user_buffer,
                                size_t buffer_length,
                                loff_t *offset,
                                int *gpio_state,
                                const char *gpio_name)
{
    char kernel_buffer[10];

    /* Prevent buffer overflow */
    if (buffer_length > sizeof(kernel_buffer))
        return -EINVAL;

    /* Copy data safely from user space */
    if (copy_from_user(kernel_buffer, user_buffer, buffer_length))
        return -EFAULT;

    /* Update GPIO state */
    *gpio_state = (kernel_buffer[0] == '1') ? 1 : 0;

    printk(KERN_INFO "Simulated %s is %s\n",
           gpio_name,
           *gpio_state ? "ON / PRESSED" : "OFF / RELEASED");

    return buffer_length;
}

/* ---------------------------------------------------
 * Generic read handler (Kernel → User)
 * --------------------------------------------------- */
static ssize_t read_gpio_state(struct file *file,
                               char __user *user_buffer,
                               size_t buffer_length,
                               loff_t *offset,
                               int *gpio_state)
{
    char state_char;

	if(*offset>0)
       {
         	return 0;
       }
    state_char = *gpio_state ? '1' : '0';

    /* Send state to user space */
    if (copy_to_user(user_buffer, &state_char, 1))
        return -EFAULT;

    *offset=1;
    return 1;
}

/* ---------------------------------------------------
 * LED proc file handlers
 * --------------------------------------------------- */
static ssize_t led_write_handler(struct file *file,
                                 const char __user *buffer,
                                 size_t count,
                                 loff_t *offset)
{
    return write_gpio_state(file, buffer, count, offset,
                            &led_status, "LED");
}

static ssize_t led_read_handler(struct file *file,
                                char __user *buffer,
                                size_t count,
                                loff_t *offset)
{
    return read_gpio_state(file, buffer, count, offset,
                           &led_status);
}

/* ---------------------------------------------------
 * Button proc file handlers
 * --------------------------------------------------- */
static ssize_t button_write_handler(struct file *file,
                                    const char __user *buffer,
                                    size_t count,
                                    loff_t *offset)
{
    return write_gpio_state(file, buffer, count, offset,
                            &button_status, "Button");
}

static ssize_t button_read_handler(struct file *file,
                                   char __user *buffer,
                                   size_t count,
                                   loff_t *offset)
{
    return read_gpio_state(file, buffer, count, offset,
                           &button_status);
}

/* ---------------------------------------------------
 * Proc operations structures
 * --------------------------------------------------- */
static const struct proc_ops led_proc_operations = {
    .proc_read  = led_read_handler,
    .proc_write = led_write_handler,
};

static const struct proc_ops button_proc_operations = {
    .proc_read  = button_read_handler,
    .proc_write = button_write_handler,
};

/* ---------------------------------------------------
 * Module initialization
 * --------------------------------------------------- */
static int __init simulated_gpio_init(void)
{
    led_proc_entry = proc_create(PROC_LED_FILE, 0666, NULL,
                                 &led_proc_operations);
    if (!led_proc_entry)
        return -ENOMEM;

    button_proc_entry = proc_create(PROC_BUTTON_FILE, 0666, NULL,
                                    &button_proc_operations);
    if (!button_proc_entry) {
        proc_remove(led_proc_entry);
        return -ENOMEM;
    }

    printk(KERN_INFO "Simulated GPIO LED/Button module loaded\n");
    return 0;
}

/* ---------------------------------------------------
 * Module cleanup
 * --------------------------------------------------- */
static void __exit simulated_gpio_exit(void)
{
    proc_remove(led_proc_entry);
    proc_remove(button_proc_entry);

    printk(KERN_INFO "Simulated GPIO LED/Button module unloaded\n");
}

module_init(simulated_gpio_init);
module_exit(simulated_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rushil Kevadiya");
MODULE_DESCRIPTION("Simulated GPIO LED and Button for Ubuntu x86_64");

