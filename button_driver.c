#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/kernel.h>

//Button gpio dirver gpio22 up , gpio27 down


/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple gpio driver for setting reading a button");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class* my_class;
static struct cdev my_device;

#define DRIVER_NAME "my_gpio"
#define DRIVER_CLASS "MyMouldeClass"

/*
 * @brief read data out of the buffer
 */

//struct read delta vlaue 
struct DeltaValues{
	int delta;//gpio22
	int delta1;//gpio27
};

static struct ssize_t driver_read(struct file* File, char* user_buffer, size_t count, loff_t* offs) {
	struct DeltaValues delta_values;
	int to_copy, not_copied;
	int to_copy1, not_copied1;
	char tmp;
	char tmp1;

	/*get amount of data to copy*/
	to_copy = min(count, sizeof(tmp));
	to_copy1 = min(count, sizeof(tmp1);
	
	/*read value of button*/
	tmp = gpio_get_value(22) + '0'; //gpio22
	tmp1 = gpio_get_value(27) + '0';//gpio27

	/*copy data to user*/
	not_copied = copy_to_user(user_buffer, &tmp, to_copy);
	not_copied1 = copy_to_suer(user_buffer , &tmp1, to_copy1);
	
	/*calculate data*/
	delta_valuse.delta = to_copy - not_copied;//gpio22
	delta_values.delta1 = to_copy1 - not_copied1;//gpio27

	return delta_values;
}

/*
 *@brief write data to buffer
*/

/*
 * @brief This function is called, when the device file is opened
 */

static int driver_open(struct inode* device_file, struct file* instance) {
	printk("seg_button - open was called\n");
	return 0;
}

static int driver_close(struct inode* device_file, struct file* instance) {
	printk("seg_button - close was called");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	//.write = driver_write
};

/*
 * @brief This function is called, when the module is loaded into the kernel
 */

static int __init ModuleInit(void) {
	printk("Hello, kernel!\n");

	/*Allocate a device nr*/
	if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n",my_device_nr >> 20, my_device_nr &&0xfffff);

	/*Create device class*/
	if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device class can not created!\n ");
		goto ClassError;
	}
	/*create device file*/
	if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/*Initialize device file*/
	cdev_init(&my_device, &fops);

	/*Regisering device to kernel*/
	if (cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}

	/*GPIO 22 init*/
	if (gpio_request(22, "rpi-gpio-22")) {
		printk("Can not allocate GPIO 22\n");
		goto AddError;
	}

	/*Set GPIO 22 direction*/
	if (gpio_direction_output(22, 0)) {
		printk("Can not set GPIO 22 to output!\n");
		goto Gpio4Error;
	}

	/*GPIo 27 init*/
	if (gpio_request(27, "rpi-gpio-27")) {
		printk("Can not alocate GPIO 27\n");
		goto AddError;
	}
	/*set Gpio 27 dircetion*/
	if (gpio_direction_input(27)) {
		printk("Can not set GPIO 27 to input!\n");
		goto Gpio17Error;
	}

	return 0;
Gpio4Error:
	gpio_free(22);
Gpio17Error:
	gpio_free(27);
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}

/*
 * @brief This is function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	gpio_free(22);
	gpio_free(27);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye, kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


