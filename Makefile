obj-m += seg_example_driver.o
KDIR = ~/working/Raspbian/kernel

RESULT1 = seg_example_program1
RESULT2 = seg_example_program2
SRC1 = $(RESULT1).c
SRC2 = $(RESULT2).c
CCC = arm-linux-gnueabihf-gcc

all:
	make -C $(KDIR) M=$(PWD) modules
	$(CCC) -o $(RESULT1) $(SRC1)
	$(CCC) -o $(RESULT2) $(SRC2)

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf $(RESULT1)
	rm -rf $(RESULT2)
