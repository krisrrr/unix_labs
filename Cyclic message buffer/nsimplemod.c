#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/time.h>


static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class


#define DEVICE_NAME "simpmod"
#define REGION_NAME "simpmodreg"
#define ROUND_BUFFER_SIZE 20
#define MESSAGE_MAX_SIZE 50

typedef struct{
    struct timespec ts;
    char message[MESSAGE_MAX_SIZE];
} buf_message;

typedef struct{
    buf_message messages[ROUND_BUFFER_SIZE];
    unsigned int insert_ptr;
    unsigned int first_ptr;
    unsigned int len;
} round_buffer;

typedef struct {
    buf_message message;
    int number;
} message_request;

static round_buffer buffer;

#define GET_BUFFER _IOR('a','a', int *)
#define GET_MESSAGE _IOWR('a','b', message_request *)

static int dev_open(struct inode*, struct file*);
static int dev_close(struct inode*, struct file*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);
static long dev_ioctl (struct file *file, unsigned int cmd, unsigned long arg);


static struct file_operations pugs_fops = {
        .open = dev_open,
        .release = dev_close,
        .write = dev_write,
        .unlocked_ioctl = dev_ioctl
};


static int dev_open(struct inode* i, struct file* f){
    printk(KERN_ALERT "Device opened\n");
    return 0;
}

static int dev_close(struct inode* i, struct file* f){
    printk(KERN_ALERT "Device closed\n");
    return 0;
}

static ssize_t dev_write(struct file* f, const char* buf, size_t len, loff_t* off){
    if (len > MESSAGE_MAX_SIZE - 1){
       len = MESSAGE_MAX_SIZE - 1;
    }

    if(copy_from_user(buffer.messages[buffer.insert_ptr].message, buf, len)){
        return -EFAULT;
    }

    buffer.messages[buffer.insert_ptr].message[len] = '\0';
    getnstimeofday(&buffer.messages[buffer.insert_ptr].ts);
    buffer.insert_ptr = (buffer.insert_ptr + 1) % ROUND_BUFFER_SIZE;
    if (buffer.insert_ptr == buffer.first_ptr){
        buffer.first_ptr = (buffer.first_ptr + 1) % ROUND_BUFFER_SIZE;
    }
    if (buffer.len < ROUND_BUFFER_SIZE){
        buffer.len ++;
    }

    printk(KERN_INFO "Driver: write. ptr: %d, fptr: %d, bl: %d\n", buffer.insert_ptr, buffer.first_ptr, buffer.len);
    return len;
}

static long dev_ioctl (struct file *file, unsigned int cmd, unsigned long arg){
    message_request req;
    int msg_num;
    switch(cmd) {
        case GET_BUFFER:
            copy_to_user((int*) arg, &buffer.len, sizeof(buffer.len));
            printk(KERN_INFO "Value = %d\n", buffer.len);
            break;
        case GET_MESSAGE:
            copy_from_user(&req, (int*) arg, sizeof(req));
            msg_num = (buffer.first_ptr + req.number) % ROUND_BUFFER_SIZE;
            printk(KERN_INFO "Copying = %d\n", msg_num);
            req.message = buffer.messages[msg_num];
            copy_to_user( (int*) arg, &req, sizeof(req));
            break;
    }
    return 0;
}

static int modtest_init(void){
    buffer.first_ptr = 0;
    buffer.insert_ptr = 0;
    buffer.len = 0;

    printk(KERN_ALERT "Hello !!!\n");
    if (alloc_chrdev_region(&first, 0, 1, REGION_NAME) < 0){
        printk(KERN_ALERT "error alloc!!!\n");
        return -1;
    }
    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL){
        printk(KERN_ALERT "error class create!!!\n");
        unregister_chrdev_region(first, 1);
        return -1;
    }
    if (device_create(cl, NULL, first, NULL, DEVICE_NAME) == NULL){
        printk(KERN_ALERT "error create deivece!!!\n");
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    cdev_init(&c_dev, &pugs_fops);
    if (cdev_add(&c_dev, first, 1) == -1)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    printk(KERN_ALERT "device created!!!\n");
    return 0;
}

static void modtest_exit(void){
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_ALERT "Bye !!!\n");
}


module_init(modtest_init);
module_exit(modtest_exit);

MODULE_LICENSE("GPL"); // fix Unknown symbol in module
