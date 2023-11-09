#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<fcntl.h>
#include<termios.h>

static struct termios init_setting, new_setting;
char seg_num[10] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x90 };
char seg_dnum[10] = { 0x40, 0x79,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10 };
int num3 = 0, num2 = 0, num1 = 0, num0 = 0, num = 0;

#define D1 0x01
#define D2 0x02
#define D3 0x04
#define D4 0x08

void init_keyboard()
{
    tcgetattr(STDIN_FILENO, &init_setting);
    new_setting = init_setting;
    new_setting.c_lflag &= ~ICANON;
    new_setting.c_lflag &= ~ECHO;
    new_setting.c_cc[VMIN] = 0;
    new_setting.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_setting);
}

void close_keyboard()
{
    tcsetattr(0, TCSANOW, &init_setting);
}

char get_key()
{
    char ch = -1;

    if (read(STDIN_FILENO, &ch, 1) != 1)
        ch = 1;
    return ch;
}

void print_menu()
{
    printf("\n----------menu----------\n");
    printf("[p] : count setting\n");
    printf("[u]: up count"\n);
    printf("[d] :down count"\n);
    printf("[q] : program exit\n");
    printf("------------------------\n\n");
}

//distplay digit
int seg_display(int num, int port) {

    int result;
    
    if (port == 0)
    {
        port = D1;
    }
    else if (port == 1)
    {
        port = D2;
    }
    else if (port == 2)
    {
        port = D3;
    }
    else if (port == 3)
    {
        port = D4;
    }

    result = seg_num[num] << 4 | port;

    return result;
    
}

void up_count() {
    num++;
    num3 = num / 1000; // thousands place of num
    num2 = (num / 100) % 10; // hundreds place of num
    num1 = (num / 10) % 10; // tens place of num
    num0 = num % 10; // ones place of num

    if (num > 9999) {
        num = 0;
    }

}

void down_count() {
    num--;
    num3 = num / 1000; // thousands place of num
    num2 = (num / 100) % 10; // hundreds place of num
    num1 = (num / 10) % 10; // tens place of num
    num0 = num % 10; // ones place of num

    if (num < 0) {
        num = 9999;
    }

}

int main(int argc, char** argv)
{
    unsigned short data[4];
    char key;
    int tmp_n;
    int delay_time;

    int dev = open("/dev/my_segment", O_RDWR); // if you want read = 'O_RDONLY' write ='O_WRDONLY', read&write='O_RDWR'

    if (dev == -1) {
        printf("Opening was not possible!\n");
        return -1;
    }
    printf("device opening was sucessful!\n");

    init_keyboard();
    print_menu();
    tmp_n = 0;
    delay_time = 1000;

    //display init 0
    data[0] = seg_display(num3, 0);
    data[1] = seg_display(num2, 1);
    data[2] = seg_display(num1, 2);
    data[3] = seg_display(num0, 3);


    while (1) {
        key = get_key();
        if (key == 'q') {
            printf("exit this program.\n");
            break;
        }
        else{
            if (key == 'u') {
                up_count();
            }
            else if (key == 'd') {
                down_count();
            }
            else if (key == 'p') {
                printf("count setting: ");
		scanf("%d", &num);
		num3 = num / 1000; // thousands place of num
	      	num2 = (num / 100) % 10; // hundreds place of num
	    	num1 = (num / 10) % 10; // tens place of num
	    	num0 = num % 10; // ones place of num

           }

            data[0] = seg_display(num3, 0);
            data[1] = seg_display(num2, 1);
            data[2] = seg_display(num1, 2);
            data[3] = seg_display(num0, 3);
        
        write(dev, &data[tmp_n], 2);
        usleep(delay_time);//delay 1ms

        tmp_n++;

        if (tmp_n > 3) {
            tmp_n = 0;
            }
        }
    }

    close_keyboard();
    write(dev, 0x0000, 2);
    close(dev);
    return 0;
}

