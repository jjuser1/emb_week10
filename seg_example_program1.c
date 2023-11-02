#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char** argv) {
	short buff;

	int dev = open("/dev/my_segment", O_RDWR); //READ and Write
	
	if (argc < 2) {
		printf("put arg 0x0000 or int\n");
		return -1;
	}

	if (dev == -1) {
		printf("Opening was notpossible!\n");
		return -1;
	}
	printf("Opening was successfull!\n");

	if (argv[1][0] == '0' && (argv[1][1] == 'x' || argv[1][1] == 'x')) {
		buff = (unsigned short)strtol(&argv[1][2], NULL, 16);
	}
	else
		buff = (unsigned short)strtol(&argv[1][0], NULL, 10);

	write(dev, &buff, 2);
	close(dev);
	return 0;
}


static void_exit ModuleExit(void) {
	gpio_set_value(2, 0);
	gpio_set_value(3, 0);
	gpio_set_value(4, 0);
	gpio_set_value(17, 0);
	gpio_set_value(21, 0);
	gpio_set_value(20, 0);
	gpio_set_value(16, 0);
	gpio_set_value(12, 0);
	gpio_set_value(7, 0);
	gpio_set_value(8, 0);
	gpio_set_value(25, 0);
	gpio_set_value(24, 0);
	gpio_free(2);
	gpio_free(3);
	gpio_free(4);
	gpio_free(17);
	gpio_free(21);
	gpio_free(20);
	gpio_free(16);
	gpio_free(12);
	gpio_free(7);
	gpio_free(8);
	gpio_free(25);
	gpio_free(24);
	cdev_del(&my_device);
	device_destory(my_class, my_device_nr);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye , Kernel\n");

}
module_init(ModuleInit);
module_exit(ModuleExit);

	