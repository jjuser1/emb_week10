obj-m += seg_example_driver.o button_driver.o
KDIR = ~/working/kernel

RESULT1 = seg_example_program1
RESULT2 = seg_example_program2
RESULT3 = segment

SRC1 = $(RESULT1).c
SRC2 = $(RESULT2).c
SRC3 = $(RESULT3).c

CCC = aarch64-linux-gnu-gcc

all:
	make -C $(KDIR) M=$(PWD) modules
	$(CCC) -o $(RESULT1) $(SRC1)
	$(CCC) -o $(RESULT2) $(SRC2)
	$(CCC) -o $(RESULT3) $(SRC3)

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf $(RESULT1)
	rm -rf $(RESULT2)
	rm -rf $(RESULT3)
