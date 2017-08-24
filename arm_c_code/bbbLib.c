/*
//Filename: libBBB.c
//Version : 0.1
//
//Project : Argonne National Lab - Forest
//Author  : Gavin Strunk
//Contact : gavin.strunk@gmail.com
//Date    : 28 June 2013
//
//Description - This is the main library file that
//		eases the interface to the BeagleBone
//		Black. It includes functions for GPIO,
//		UART, I2C, SPI, ADC, Timing, and Overlays.
//
//Revision History
//	0.1:  Wrote the basic framework for all the 
//		functions. \GS
*/

/*
Copyright (C) 2013 Gavin Strunk

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "bbbLib.h"

//Local functions not used by outside world

void initCMD(unsigned char cmd);



//*************************************************
//*                USR FUNCTIONS                  *
//*************************************************
int setUsrLedValue(char* led, int value)
{
	FILE *usr;
	char buf[20];
	char buf2[50] = "/sys/class/leds/beaglebone:green:";

	//build file path to usr led brightness
	sprintf(buf,"%s",led);
	strcat(buf2,strcat(buf,"/brightness"));

	usr = fopen(buf2, "w");
	if(usr == NULL) printf("USR Led failed to open\n");
	fseek(usr,0,SEEK_SET);
	fprintf(usr,"%d",value);
	fflush(usr);
	fclose(usr);

	return 0;
}

//*************************************************
//*               GPIO FUNCTIONS                  *
//*************************************************
int initPin(int pinnum)
{
	FILE *io;

	io = fopen("/sys/class/gpio/export", "w");
	if(io == NULL) printf("Pin failed to initialize\n");
	fseek(io,0,SEEK_SET);
	fprintf(io,"%d",pinnum);
	fflush(io);
	fclose(io);

	return 0;
}

int setPinDirection(int pinnum, char* dir)
{
	FILE *pdir;
	char buf[10];
	char buf2[50] = "/sys/class/gpio/gpio";

	//build file path to the direction file
	sprintf(buf,"%i",pinnum);
	strcat(buf2,strcat(buf,"/direction"));

	pdir = fopen(buf2, "w");
	if(pdir == NULL) printf("Direction failed to open\n");
	fseek(pdir,0,SEEK_SET);
	fprintf(pdir,"%s",dir);
	fflush(pdir);
	fclose(pdir);

	return 0;
}

int setPinValue(int pinnum, int value)
{
	FILE *val;
	char buf[5];
	char buf2[50] = "/sys/class/gpio/gpio";

	//build path to value file
	sprintf(buf,"%i",pinnum);
	strcat(buf2,strcat(buf,"/value"));

	val = fopen(buf2, "w");
	if(val == NULL) printf("Value failed to open\n");
	fseek(val,0,SEEK_SET);
	fprintf(val,"%d",value);
	fflush(val);
	fclose(val);

	return 0;
}

int getPinValue(int pinnum)
{
	FILE *val;
	int value;
	char buf[5];
	char buf2[50] = "/sys/class/gpio/gpio";

	//build file path to value file
	sprintf(buf,"%i",pinnum);
	strcat(buf2,strcat(buf,"/value"));

	val = fopen(buf2, "r");
	if(val == NULL) printf("Input value failed to open\n");
	fseek(val,0,SEEK_SET);
	fscanf(val,"%d",&value);
	fclose(val);

	return value;
}

//*************************************************
//*                PWM FUNCTIONS                  *
//*************************************************
int initPWM(int mgrnum, char* pin)
{
	FILE *pwm;
	char buf[5];
	char buf2[50] = "/sys/devices/bone_capemgr.";
	char buf3[20] = "bone_pwm_";

	//build file paths
	sprintf(buf,"%i",mgrnum);
	strcat(buf2,strcat(buf,"/slots"));

	strcat(buf3,pin);

	pwm = fopen(buf2, "w");
	if(pwm == NULL) printf("PWM failed to initialize\n");
	fseek(pwm,0,SEEK_SET);
	fprintf(pwm,"am33xx_pwm");
	fflush(pwm);
	fprintf(pwm,"%s",buf3);
	fflush(pwm);
	fclose(pwm);

	return 0;
}

int setPWMPeriod(int helpnum, char* pin, int period)
{
	FILE *pwm;
	char buf[5];
	char buf2[60] = "/sys/devices/ocp.2/pwm_test_";

	//build file path
	sprintf(buf,"%i",helpnum);
	printf("%s\n",pin);
	strcat(buf2,pin);
	strcat(buf2,".");
	strcat(buf2,strcat(buf,"/period"));
	
	printf("%s\n",buf2);
	pwm = fopen(buf2, "w");
	if(pwm == NULL) printf("PWM Period failed to open\n");
	fseek(pwm,0,SEEK_SET);
	fprintf(pwm,"%d",period);
	fflush(pwm);
	fclose(pwm);

	return 0;
}

int setPWMDuty(int helpnum, char* pin, int duty)
{
	FILE *pwm;
	char buf[5];
	char buf2[50] = "/sys/devices/ocp.2/pwm_test_";

	//build file path
	sprintf(buf,"%i",helpnum);
	strcat(buf2,pin);
	strcat(buf2,".");
	strcat(buf2,strcat(buf,"/duty"));

	pwm = fopen(buf2, "w");
	if(pwm == NULL) printf("PWM Duty failed to open\n");
	fseek(pwm,0,SEEK_SET);
	fprintf(pwm,"%d",duty);
	fflush(pwm);
	fclose(pwm);

	return 0;
}

int setPWMOnOff(int helpnum, char* pin, int run)
{
	FILE *pwm;
	char buf[5];
	char buf2[50] = "/sys/devices/ocp.2/pwm_test_";

	//build file path
	sprintf(buf,"%i",helpnum);
	strcat(buf2,pin);
	strcat(buf2,".");
	strcat(buf2,strcat(buf,"/run"));

	pwm = fopen(buf2, "w");
	if(pwm == NULL) printf("PWM Run failed to open\n");
	fseek(pwm,0,SEEK_SET);
	fprintf(pwm,"%d",run);
	fflush(pwm);
	fclose(pwm);

	return 0;
}

//*************************************************
//*               UART FUNCTIONS                  *
//*************************************************
int initUART()
{
	//return the int reference to the port
	struct termios old;
	struct termios new;
	int fd;

	fd = open(TTY, O_RDWR | O_NOCTTY);
	if(fd < 0)
	{
		printf("Port failed to open\n");
		return fd;
	}

	tcgetattr(fd,&old);
	bzero(&new, sizeof(new));

	new.c_cflag = B4800 | CS8 | CLOCAL | CREAD;
	new.c_iflag = IGNPAR | ICRNL;
	new.c_oflag = 0;
	new.c_lflag = 0;

	new.c_cc[VTIME] = 0;
	new.c_cc[VMIN]  = 1;

	//clean the line and set the attributes
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&new);

	return fd;
}

void closeUART(int fd)
{
	close(fd);
}

int configUART(UART u, int property, char* value)
{
	//This is used to set the configuration values
	//for the uart module
	
	
	return 0;
}

int txUART(int uart, unsigned char data)
{
	//write a single byte
	
	write(uart,&data,1);
	tcdrain(uart);

	return 0;
}

unsigned char rxUART(int uart)
{
	//read in a single byte
	unsigned char data;

	read(uart,&data,1);
	return data;
}

int UARTputstr(int uart,  char* buf)
{

	int i;

	for(i=0; i < strlen(buf); i++)
		txUART(uart,buf[i]);

	return 0;
}

int UARTgetstr(int uart,  char* buf)
{
    int i ;

    i = 0 ;
   while (1) {
        buf[i] = rxUART(uart) ;
        if (buf[i] == '\n') break ;
        i += 1 ;
    } 
    i += 1 ;
    buf[i] = '\x0'  ;
  
    return 0 ;
}



//*************************************************
//*                I2C FUNCTIONS                  *
//*************************************************
// Returns a handle for i2c device at "addr" on bus "bus"

int i2c_open(unsigned char bus, unsigned char addr)
{
  int file;
  char filename[16];
  sprintf(filename,"/dev/i2c-%d", bus);

  if ((file = open(filename,O_RDWR)) < 0)
  {
    fprintf(stderr, "i2c_open open error: %s\n", strerror(errno));
    return(file);
  }
  if (ioctl(file,I2C_SLAVE,addr) < 0)
  {
    fprintf(stderr, "i2c_open ioctl error: %s\n", strerror(errno));
    return(-1);
  }
  return(file);
}

// Write out a data buffer to i2c device

int i2c_write(int handle, unsigned char* buf, unsigned int length)
{
  if (write(handle, buf, length) != length)
  {
    fprintf(stderr, "i2c_write error: %s\n", strerror(errno));
    return(-1);
  }
  return(length);
}

//  Write out a single byte to i2c device

int i2c_write_byte(int handle, unsigned char val)
{
  if (write(handle, &val, 1) != 1)
  {
    fprintf(stderr, "i2c_write_byte error: %s\n", strerror(errno));
    return(-1);
  }
  return(1);
}

// Read a specified number of bytes from i2c device

int i2c_read(int handle, unsigned char* buf, unsigned int length)
{
  if (read(handle, buf, length) != length)
  {
    fprintf(stderr, "i2c_read error: %s\n", strerror(errno));
    return(-1);
  }
  return(length);
}

// Read a single byte from the device

int i2c_read_byte(int handle, unsigned char* val)
{
  if (read(handle, val, 1) != 1)
  {
    fprintf(stderr, "i2c_read_byte error: %s\n", strerror(errno));
    return(-1);
  }
  return(1);
}

// Close the handle to the device

int i2c_close(int handle)
{
  if ((close(handle)) != 0)
  {
    fprintf(stderr, "i2c_close error: %s\n", strerror(errno));
    return(-1);
  }
  return(0);
}

// Write and read

int i2c_write_read(int handle,
                   unsigned char addr_w, unsigned char *buf_w, unsigned int len_w,
                   unsigned char addr_r, unsigned char *buf_r, unsigned int len_r)
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg msgs[2];
	
	msgs[0].addr=addr_w;
	msgs[0].len=len_w;
	msgs[0].flags=0;
	msgs[0].buf=buf_w;
	
	msgs[1].addr=addr_r;
	msgs[1].len=len_r;
	msgs[1].flags=1;
	msgs[1].buf=buf_r;
	
	msgset.nmsgs=2;
	msgset.msgs=msgs;
	
	if (ioctl(handle,I2C_RDWR,(unsigned long)&msgset)<0)
  {
		fprintf(stderr, "i2c_write_read error: %s\n",strerror(errno));
    return -1;
  } 
  return(len_r);
}

// Write and ignore NACK

int i2c_write_ignore_nack(int handle,
                          unsigned char addr_w, unsigned char* buf, unsigned int length)
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg msgs[1];
	
	msgs[0].addr=addr_w;
	msgs[0].len=length;
	msgs[0].flags=0 | I2C_M_IGNORE_NAK;
	msgs[0].buf=buf;
	
	msgset.nmsgs=1;
	msgset.msgs=msgs;
	
	if (ioctl(handle,I2C_RDWR,(unsigned long)&msgset)<0)
  {
		fprintf(stderr, "i2c_write_ignore_nack error: %s\n",strerror(errno));
    return -1;
  } 
  return(length);
}

// Read and ignore no ACK

int i2c_read_no_ack(int handle, 
                    unsigned char addr_r, unsigned char* buf, unsigned int length)
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg msgs[1];
	
	msgs[0].addr=addr_r;
	msgs[0].len=length;
	msgs[0].flags=I2C_M_RD | I2C_M_NO_RD_ACK;
	msgs[0].buf=buf;
	
	msgset.nmsgs=1;
	msgset.msgs=msgs;
	
	if (ioctl(handle,I2C_RDWR,(unsigned long)&msgset)<0)
  {
		fprintf(stderr, "i2c_read_no_ack error: %s\n",strerror(errno));
    return -1;
  } 
  return(length);
}

// Delay for specified number of msec

int delay_ms(unsigned int msec)
{
  int ret;
  struct timespec a;
  if (msec>999)
  {
    fprintf(stderr, "delay_ms error: delay value needs to be less than 999\n");
    msec=999;
  }
  a.tv_nsec=((long)(msec))*1E6d;
  a.tv_sec=0;
  if ((ret = nanosleep(&a, NULL)) != 0)
  {
    fprintf(stderr, "delay_ms error: %s\n", strerror(errno));
  }
  return(0);
}

//*************************************************
//*                LCD FUNCTIONS                  *
//*************************************************
/*NOTE: DO NOT directly include libBBB.h for LCD functions!
 * 	Instead include libLCD.h as this implements the 
 * 	screen control and full initialization.
*/
int initLCD()
{
	//initialize the pins
	initPin(RS);
	initPin(E);
	initPin(D4);
	initPin(D5);
	initPin(D6);
	initPin(D7);

	//set direction
	setPinDirection(RS,OUT);
	setPinDirection(E,OUT);
	setPinDirection(D4,OUT);
	setPinDirection(D5,OUT);
	setPinDirection(D6,OUT);
	setPinDirection(D7,OUT);

	setPinValue(E,OFF);

	//initialize the screen
	pauseNanoSec(1500000);
	initCMD(0x30);
	pauseNanoSec(5000000);
	initCMD(0x30);
	pauseNanoSec(5000000);
	initCMD(0x30);
	pauseNanoSec(5000000);
	initCMD(0x20);

	pauseNanoSec(5000000);
	writeCMD(0x2C);
	pauseNanoSec(5000000);
	writeCMD(0x08);
	pauseNanoSec(5000000);
	writeCMD(0x01);
	pauseNanoSec(2000000);
	writeCMD(0x06);
	pauseNanoSec(5000000);
	writeCMD(0x0E);
	pauseNanoSec(5000000);

	return 0;
}

void initCMD(unsigned char cmd)
{
	//bring rs low for command
	setPinValue(RS,OFF);
	pauseNanoSec(500000);

	//send the highest nibble only
	setPinValue(E,ON);
	setPinValue(D7,((cmd >> 7) & 1));
	setPinValue(D6,((cmd >> 6) & 1));	
	setPinValue(D5,((cmd >> 5) & 1));	
	setPinValue(D4,((cmd >> 4) & 1));	
	pauseNanoSec(500000);
	setPinValue(E,OFF);
	pauseNanoSec(500000);
}

int writeChar(unsigned char data)
{
	//bring rs high for character
	pauseNanoSec(500000);
	setPinValue(RS,ON);
	pauseNanoSec(500000);

	//send highest nibble first
	setPinValue(E,ON);
	setPinValue(D7, ((data >> 7) & 1));
	setPinValue(D6, ((data >> 6) & 1));
	setPinValue(D5, ((data >> 5) & 1));
	setPinValue(D4, ((data >> 4) & 1));
	pauseNanoSec(500000);
	setPinValue(E,OFF);
	pauseNanoSec(500000);

	//send the low nibble
	setPinValue(E,ON);
	setPinValue(D7, ((data >> 3) & 1));
	setPinValue(D6, ((data >> 2) & 1));
	setPinValue(D5, ((data >> 1) & 1));
	setPinValue(D4, (data & 1));
	pauseNanoSec(500000);
	setPinValue(E,OFF);
	pauseNanoSec(500000);

	return 0;
}

int writeCMD(unsigned char cmd)
{
	//bring rs low for command
	setPinValue(RS, OFF);
	pauseNanoSec(500000);

	//send highest nibble first
	setPinValue(E,ON);
	setPinValue(D7, ((cmd >> 7) & 1));
	setPinValue(D6, ((cmd >> 6) & 1));
	setPinValue(D5, ((cmd >> 5) & 1));
	setPinValue(D4, ((cmd >> 4) & 1));
	pauseNanoSec(500000);
	setPinValue(E,OFF);
	pauseNanoSec(500000);

	//send the low nibble
	setPinValue(E,ON);
	setPinValue(D7, ((cmd >> 3) & 1));
	setPinValue(D6, ((cmd >> 2) & 1));
	setPinValue(D5, ((cmd >> 1) & 1));
	setPinValue(D4, (cmd & 1));
	pauseNanoSec(500000);
	setPinValue(E, OFF);
	pauseNanoSec(500000);

	return 0;
}


//*************************************************
//*                ADC FUNCTIONS                  *
//*************************************************
int initADC(int mgrnum)
{
	FILE *ain;
	char buf[5];
	char buf2[50] = "/sys/devices/bone_capemgr.";

	//build path to setup ain
	sprintf(buf,"%i",mgrnum);
	strcat(buf2,strcat(buf,"/slots"));

	ain = fopen(buf2, "w");
	if(ain == NULL) printf("Analog failed load\n");
	fseek(ain,0,SEEK_SET);
	fprintf(ain,"cape-bone-iio");
	fflush(ain);
	fclose(ain);

	return 0;
}

int readADC(int helpnum, char* ach)
{
	FILE *aval;
	int value;
	char buf[5];
	char buf2[50] = "/sys/devices/ocp.2/helper.";

	//build file path to read adc
	sprintf(buf,"%i",helpnum);
	strcat(buf2,strcat(buf,ach));
	
	aval = fopen(buf2, "r");
	if(aval == NULL) printf("Analog failed to open\n");
	fseek(aval,0,SEEK_SET);
	fscanf(aval,"%d",&value);
	fflush(aval);
	fclose(aval);

	return value;
}

//********************************************
//*            TIME FUNCTIONS                *
//********************************************
void pauseSec(int sec)
{
	time_t now,later;

	now = time(NULL);
	later = time(NULL);

	while((later - now) < (double)sec)
		later = time(NULL);
}

int pauseNanoSec(long nano)
{
	struct timespec tmr1,tmr2;

	//assume you are not trying to pause more than 1s
	tmr1.tv_sec = 0;
	tmr1.tv_nsec = nano;

	if(nanosleep(&tmr1, &tmr2) < 0)
	{
		printf("Nano second pause failed\n");
		return -1;
	}
	return 0;
}
