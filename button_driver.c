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

static ssize_t driver_read(struct file* File, char* user_buffer, size_t count, loff_t* offs) {
    int to_copy, not_copied;
    char button_state[2]; // 두 버튼의 상태를 저장하기 위해

    // 버튼의 값을 읽어서 문자로 변환
    button_state[0] = gpio_get_value(22) + '0';
    button_state[1] = gpio_get_value(27) + '0';

    // 복사할 바이트 수를 가져옵니다 (button_state의 크기를 초과할 수 없음)
    to_copy = min(count, sizeof(button_state));

    // 사용자 버퍼로 데이터를 복사합니다.
    not_copied = copy_to_user(user_buffer, &button_state, to_copy);

    // 사용자 공간으로 복사된 바이트 수를 반환합니다.
    return to_copy - not_copied;
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


