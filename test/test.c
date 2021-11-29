#include <stc12c5a60s2.h>
#include <lcd12864.h>
#include "DS1302.h"
#include <HX711.h>
#include <eeprom52.h>
#include <intrins.h>
typedef unsigned int u16;
typedef unsigned char u8;
unsigned char speed=8;
unsigned char code motor_Z[8][4]={{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,1,1},{0,0,0,1},{1,0,0,1}};
unsigned char code motor_F[8][4]={{1,0,0,1},{0,0,0,1},{0,0,1,1},{0,0,1,0},{0,1,1,0},{0,1,0,0},{1,1,0,0},{1,0,0,0}};
	
unsigned char pwm_motor_val=10;
unsigned char pwm_t;
	int shi=0;

int miao=30;
int wait=5;
int q=0;
//sbit _4IN=P3^1;
//sbit _4IN1=P3^0;
//sbit key_s2 = P3^0;
//sbit key_s3 = P3^1;
sbit MOOD0=P3^0;
sbit MOOD1=P3^1;
sbit MOOD2=P1^1;
sbit MOOD3=P3^2;

uint temp = 1;

void delay2(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
	for(y=114;y>0;y--);
}

void delay1ms(u16 aa)
{
	unsigned char bb;
	while(aa--)
	{
	for(bb=0;bb<115;bb++)
		{;
		}
	}
	
}
void Motor_ZZ()
{
u8 i,j;
	for(j=0;j<8;j++)
	{
		for(i=0;i<8;i++)
		{
			MOOD3=motor_Z[i][0];
				MOOD2=motor_Z[i][1];
				MOOD1=motor_Z[i][2];
				MOOD0=motor_Z[i][3];
			delay1ms(speed);
		}
	}
	
}

void Motor_FZ()
{
u8 i,j;
	for(j=0;j<8;j++)
	{
		for(i=0;i<8;i++)
		{
			MOOD3=motor_F[i][0];
				MOOD2=motor_F[i][1];
				MOOD1=motor_F[i][2];
				MOOD0=motor_F[i][3];
			delay1ms(speed);
		}
	}
	
}


//void Timer1Init()
//{
//	TMOD |= 0x20;
//	TH1 = 220;
//	TL1 = 220;
//	TR1 = 1;
//	ET1 = 1;
//	EA	= 1;
//}

//void timer1() interrupt 3
//{
//	pwm_t++;
//	if(pwm_t == 255)
//		_4IN = 0;
//	if(pwm_motor_val == pwm_t)
//		_4IN = 1;					 
//}
void miaobiaoDisplay()
{

	if(q==1){
		uchar i=0;
		lcd_pos(3,0);
		lcd_wdat('S');
			lcd_wdat('C');
		lcd_wdat(':');
//			lcd_wdat((fen/10%10)+48);
//		lcd_wdat((fen%10)+48);
//		lcd_wdat(':');
			lcd_wdat((miao/10%10)+48);
		lcd_wdat((miao%10)+48);
		delay2(6000);
	
	
	miao--;
	if(miao==0){
	miao=0;	
	}
	
	wait--;
	if(wait==0){
//		_4IN=~_4IN;
//		_4IN1=~_4IN1;
		Motor_ZZ();
			
		wait=5;
		Motor_FZ();
	}
	
		}
	}
	void modle1()
	{
    lcd_write(0,5,"模式一");
		lcd_write(1,0,"常温");
		lcd_write(1,3,"800");
			lcd_write(1,6,"3KG");
//		_4IN=1;
//	_4IN1=0;
//	
			Motor_ZZ();
	}
 void modle2()
	{
		
    lcd_write(0,5,"模式二");
		lcd_write(1,0,"常温");
		lcd_write(1,3,"800");
			lcd_write(1,6,"3KG");
		//miao=miao+45;
	miao=45;
	}
	
	 void modle3()
	{
		
    lcd_write(0,5,"模式叁");
		lcd_write(1,0,"常温");
		lcd_write(1,3,"800");
		lcd_write(1,6,"3KG");
		miao=60;
	}
	
	 void ershi()
	{
	
		lcd_write(1,0,"20度");
		 
		
		 
	}
	 void sanshi()
	{
	
		lcd_write(1,0,"30度");
		 
		
		 
	}
	 void sishi()
	{
	
		lcd_write(1,0,"40度");
		 
		
		 
	}
	void yiqian()
	{
	
		lcd_write(1,3,"1000");
		 
		
		 
	}
	void yiqianer()
	{
	
		lcd_write(1,3,"1200");
		 
		
		 
	}
		void wukg()
	{
	
		lcd_write(1,6,"5KG");
		 
		
		 
	}
		void bakg()
	{
	
	
		 	lcd_write(1,6,"8KG");
		
		 
	}
	void qingping()
	{
		
    lcd_write(0,5,"    ");
		lcd_write(1,0,"    ");
		lcd_write(1,4,"    ");
		lcd_write(1,6,"    ");
		
	}
/*------------------------------
**********按键扫描**************
------------------------------*/
uchar KeyScan()
{
	uchar cord_l,cord_h;
	P2 = 0xf0;//1111 0000
	if( (P2 & 0xf0) != 0xf0)
	{
		delay(5);
		if( (P2 & 0xf0) != 0xf0)
		{
			  cord_l = P2 & 0xf0;
			  P2 = cord_l | 0x0f;
			  cord_h = P2 & 0x0f;
			  while( (P2 & 0x0f) != 0x0f );
			  return (cord_l + cord_h);
		}	
	}
}

void KeyPro()
{
	switch( KeyScan() )
	{
		case 0xee:lcd_wcmd(0x01);;q=1;modle1();break;//清屏
		case 0xde:lcd_wcmd(0x01);q=1;modle2();break;//模式二
		case 0xbe:lcd_wcmd(0x01);q=1;modle3();break;、//模式三
//		case 0x7e:_4IN=1;_4IN1=0;break;//风扇模式一
//		
		case 0xed:ershi();break;//20
		case 0xdd:yiqian();break;//1000
		case 0xbd:wukg();break;//5KG
//		case 0x7d:_4IN=1;_4IN1=1;break;
	
		case 0xeb:sanshi();break;//20
		case 0xdb:yiqianer();break;//1200
		case 0xbb:bakg();break;//8KG
//		case 0x7b:fan_timing();break;//风扇定时

	  case 0xe7:sishi();break;//改变时
		case 0xd7:syiqianer();break;//改变分
		case 0xb7:Motor_ZZ();break;//改变周
//		case 0x77:add=~add;break;//时间调整标志位
	}	
}
/////////////////////////////////////////////////////////



void main()
{
	

//	_4IN=1;
//	_4IN1=1;
    delay(10);                 //延时
   	
    lcd_init();                //初始化LCD  
	
	  init_eeprom();  //开始初始化保存的数据	
		Timer0_Init();
	  Get_Maopi();	
	
    Ds_init();	//初始化1302

//	
		EA = 0;

//	 	Timer1Init();
//		//初中始化完成，开断
		EA = 1;
		

lcd_gunping();	
	
while(1)
 {	
	 
	 miaobiaoDisplay();//显示倒计时
	
	 lcd_TimeDispaly();//时钟模块显示时间
	 Ds_get();	
	 if(Weight_Shiwu<3000&&Weight_Shiwu>100){
modle1();

}
if(Weight_Shiwu>3000&&Weight_Shiwu<5000){
modle2();

}
if(Weight_Shiwu>5000){
modle3();

}
if(Weight_Shiwu==0){
	//qingping();

}

if(miao<2){
		LED=0;
		Buzzer=0;
	delay(10);
	Buzzer=1;
//		_4IN=1;
//	_4IN1=1;
	}
if(miao==0)
{
	miao=0;
	miao++;
}

	 
	 /*---称重-----*/
	 		lcd_write(2,0,"重量:");
	       //每0.5秒称重一次
		if (FlagTest==1)
		{
			Get_Weight();
			FlagTest = 0;
		}	
		/*-------------*/
	 
   KeyScan();
	 KeyPro();
   
	
	}
 }

	