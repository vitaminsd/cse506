#include <defs.h>
#include <stdarg.h>
#include <mem.h>
#define COLOR 0x07
#define COLS 80
#define ROWS 25
#define VGA_START 0xB8000
#define PRINTABLE(c) (c>=' ')
//uint32_t char
//uint16_t unsigned char (old unit8_t)
uint32_t *Scrn;
int Curx, Cury = 0;
void putchar(char c);
void puts(char* str);
void itoa(char *buf, int base, int d);
void update_cursor(int row, int col);
//void update_cursor(int row,int col){
//	unsigned short position = (row*COLS)+col;
//	outb(0x3D4,0x0F);
//	outb(0x3D5,(unsigned char)(position&0xFF));
//	outb(0x3D4,0x0E);
//	outb(0x3D5,(unsigned char)((position>>8)&0xFF));
//}	
void scroll(){
    memcpy((unsigned char *)VGA_START, (unsigned char *)(VGA_START+2*COLS), 2*(ROWS-1)*COLS);
    memset((unsigned char *)(VGA_START+2*(ROWS-1)*COLS),0,2*COLS);
    Curx = 0;
    Cury = ROWS-1;
    return;
}

void itoa(char *buf, int base, int d){
	char *p = buf;
	char *p1,*p2;
	unsigned long ud = d;

	int divisor = 10;
	if(base == 'd' && d < 0) //%d and minus
	{
		*p++ = '-';
		buf ++;
		ud = -d;
	}
	else if (base == 'x'||base == 'p')
		divisor = 16;
	do{
		int remainder = ud % divisor;
		*p++ = (remainder<10) ? remainder + '0': remainder +'a'-10;
	}while(ud /= divisor);

	*p = '\0';

	p1 = buf;
	p2 = p-1;
	while(p1<p2)
	{
		char temp = *p1;
		*p1 = *p2;
		*p2 =temp;
		p1++;
		p2--;
	}

}
void putchar(char c){
	char *addr;
	if(c == '\t')
		Curx =((Curx +4)/4)*4; //tab
	else if(c == '\r')
		Curx = 0; //return
	else if (c == '\n')//new line
	{
		Curx = 0;
		Cury ++;
		if (Cury >= ROWS){
			scroll();
		}
	}
	else if (c == 0x08 &&Curx != 0) 
		Curx --;//backspace
	else if(PRINTABLE(c))
	{
		//addr = Scrn +2*(Cury*COLS+Curx);
		addr =(char*) VGA_START+2*(Cury*COLS+Curx);
		*addr =(COLOR<<8)|c;
		Curx ++;
		if (Curx >= COLS){
			Cury += Curx / COLS;
			Curx = Curx%COLS;
		}
		if (Cury >= ROWS){
			scroll();
		}

	}
}

void puts(char* str)
{
	while(*str)
	{
		putchar(*str);
		str++;
	}
}

void printf(const char *format,...)
{
	int c;
	va_list ap;
	char* ss;
	char cc;
	int ii;
	char buf[20];
	va_start(ap,format);
	while((c = *format++)!= 0)
	{
		if(c!='%')
			putchar(c);
		else{
			
			c = *format++;
			switch(c){
				case 's':
				ss = va_arg(ap,char*);
				if( ss ==NULL)
					ss="null\n";
				while(*ss)
					putchar(*ss++);
				break;

				case 'c':
				cc = va_arg(ap,int);
				putchar(cc);
				break;
				
				case 'd':
				ii = va_arg(ap,int);
				itoa(buf,c,ii);
				ss = buf;
				while(*ss)
					putchar(*ss++);
				break;	
				
				case 'p':
				ii = va_arg(ap,int);
				itoa(buf,c,ii);
				ss = "0x";
				while (*ss)
					putchar(*ss++);
				ss = buf;
				while(*ss)
					putchar(*ss++);
				break;

				case 'x':
				ii = va_arg(ap,int);
				itoa(buf,c,ii);
				ss = buf;
				while(*ss)
					putchar(*ss++);
				break;

				default:
				va_end(ap);
				break;
			}
		}
	}
}
