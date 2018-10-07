#include<reg51.h>
#include<intrins.h>
 
#define lcd_data P0
 
sfr16 DPTR=0x82;
sbit trig=P3^5;
sbit echo=P3^2;
sbit relay=P2^5;
sbit alc=P2^0;
sbit rs=P2^6;
sbit en=P2^7;
sbit buz=P3^7;
sbit led=P2^3;
unsigned int range=0;
 
void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay();
void delay();
 
void lcd_init()
{
 cmd(0x01);
 cmd(0x38);
 cmd(0x0e);
 cmd(0x06);
 cmd(0x0c);
 cmd(0x80);
}
 
void cmd(unsigned char a)
{
 lcd_data=a;
 rs=0;
 en=1;
 lcd_delay();
 en=0;
}
 
void dat(unsigned char b)
{
 lcd_data=b;
 rs=1;
 en=1;
 lcd_delay();
 en=0;
}
 
void show(unsigned char *s)
{
 while(*s) 
     {
    dat(*s++);
   }
}
 
void lcd_delay()
{
 unsigned int i;
 for(i=0;i<=1000;i++);
}
 
 
void send_pulse(void) 
{
 TH0=0x00;TL0=0x00; 
 trig=1;
 _nop_();_nop_();_nop_();_nop_();_nop_();
 _nop_();_nop_();_nop_();_nop_();_nop_();
 trig=0;
}
 
unsigned char ultrasonic()
{
 unsigned char get;
 send_pulse();
 while(!echo);
 while(echo);
 DPH=TH0;
 DPL=TL0;
 TH0=TL0=0xff;
 if(DPTR<38000){
    get=(DPTR/59);
    }
   else
      get=0;
 return get;
}
 
 
void main()
{
 relay=1;    
 P1=0xFF;
	buz=1;
	led=1;
 TMOD=0x09;
 TH0=TL0=0;
 TR0=1;
 lcd_init();
 cmd(0x84);
 show("Welcome!");
 delay();
 delay();
 delay();
 cmd(0x01);
 P3|=(1<<2);
 while(1) {
 if(alc==0) //if alcohol is detected
    { P1=0xFF;
			cmd(0x01); delay();			//clear display screen 
      buz=1;  
			while(alc==0){
            relay=0;
        cmd(0x80); delay();  //cursor on line 1 ,location1
        show("Alcohol Detected");
					led=0;
            delay();
        }
    cmd(0x01);    
    }
		led=1;
 relay=1;
 cmd(0x84);
 show("Distance:");
 range=ultrasonic();
      if(range<150){
					buz=0;
           P1=0x00;}
         else{
						buz=1;
             P1=0xFF;}
 cmd(0xC6);
 dat((range/100)+48);
 dat(((range/10)%10)+48);
 dat((range%10)+48);
 show("cm");
 }
}

void delay(){
    int i,j;
    for(i=0;i<1000;i++)
    for(j=0;j<10;j++);
} 
