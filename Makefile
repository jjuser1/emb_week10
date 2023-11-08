obj-m += seg_example_driver.o
RESULT1 = seg_example_program1
RESULT2 = seg_example_program2
SRC1 = $(RESULT1).c
SRC2 = $(RESULT2).c

all:
	make -C /home/jjuser/working/kernel M=$(PWD) modules
	aarch64-linux-gnu-gcc -o $(RESULT1) $(SRC1)
	aarch64-linux-gnu-gcc -o $(RESULT2) $(SRC2)

clean:
	make -C $(HOME)/working/kernel M=$(PWD) clean
	rm -rf $(RESULT1)
	rm -rf $(RESULT2)
