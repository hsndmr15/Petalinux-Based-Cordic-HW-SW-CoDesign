#define TERMINAL "/dev/ttyPS0"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include <time.h>

char karakter[255];
int sayi1[255];
int sayi2[255];
int i;
int j;

int main()
{
	unsigned int gpio_size = 0x8000;
	off_t gpio_pbase = 0x41200000;
	long long *gpio64_vptr;
	int fdgpio;
	
	char *portname = TERMINAL;
	int fd;
	int wlen;
	char *xstr = "Aralarında '-' karakteri kullanarak işleme girecek iki adet sayı yazınız \n";
	int xlen = strlen(xstr);
fd = open (portname, O_RDWR | O_NOCTTY| O_SYNC);
if (fd < 0) {
	printf("Error opening %s: %s\n", portname, strerror(errno));
	return 1;
}
	wlen = write(fd, xstr, xlen); 
	if (wlen != xlen){
		printf("Error from write: Xd, %d\n", wlen, errno);
	}

do{
	unsigned char buf[80];
	int rdlen;

	rdlen = read (fd, buf, sizeof(buf) -1); 
	if (rdlen > 0){

	buf[rdlen]=0;
	printf("Girdiğiniz sayılar: %s \n", buf);
	
    j = strlen(buf);
    int kontrol = 0;
	
    for (i = 0; i < j; i++) 
    {
        if(buf[i]== '-'){ 
            kontrol = 1;
        } else{
            if(kontrol == 0){
                sayi1[i] = buf[i] - '0'; 
            }
            else {
                sayi2[i] = buf[i] - '0';
            }
        }
    }

    int valuefirst = sayi1[1] + sayi1[0]*10; 
    printf("Deger1:%d \n", valuefirst);
    int valuesecond = sayi2[4] + sayi2[3]*10;
    printf("Deger2:%d \n", valuesecond);
	
	if ((fdgpio = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {

		gpio64_vptr = (long long *)mmap(NULL, gpio_size,PROT_READ|
		PROT_WRITE, MAP_SHARED, fdgpio, gpio_pbase); 
		//printf("MMAP Sanal Adress: %x\n", gpio64_vptr); 
		*(gpio64_vptr)  = valuefirst; 
		*(gpio64_vptr + 1) = valuesecond; 
	
		long sonuc = *(gpio64_vptr + 1); 
		printf("SONUC: %d \n:" , sonuc);
		char *mtn = "SONUC: \n";
		int mtnlen = strlen(mtn);
		write(fd, mtn, mtnlen);
		
		char Gsonuc[20];
		sprintf(Gsonuc, "%d", sonuc);
		int Gsonuclen = strlen(Gsonuc);
		write(fd, Gsonuc, Gsonuclen); 
		close(fdgpio);
		}
	} else if (rdlen < 0) {
		printf("Error from read: %d: %s\n", rdlen, strerror (errno));
	}else {
		printf("Timeout from read\n");
	}

	}while (1);
}