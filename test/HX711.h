#ifndef  __HX711_H_
#define  __HX711_H_
#define uchar unsigned char
#define uint  unsigned int
#include <eeprom52.h>
//函数或者变量声明
extern void Delay_ms(unsigned int n);
extern void Get_Maopi();
extern void Get_Weight();
unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
unsigned long Weight_Maopi_0 = 0;
long Weight_Shiwu = 0;
long wsbeep=200;    //系统设定值为200g
uint chazhi=20;     //重量正常值在180-220之间

unsigned int qupi=0;
uint GapValue,GapValue1;

//定义标识
volatile bit FlagTest = 0;		//定时测试标志，每0.5秒置位，测完清0
sbit LED=P3^5;
sbit Buzzer = P1^0;
/////////////////////AD称重读取程序/////////////////

//IO设置
sbit HX711_DOUT=P1^2; 
sbit HX711_SCK=P1^3;


//****************************************************
//延时函数
//****************************************************
void Delay__hx711_us(void)
{
	_nop_();
	_nop_();
}

//****************************************************
//读取HX711
//****************************************************
unsigned long HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_DOUT=1; 
	Delay__hx711_us();
  	HX711_SCK=0; 
  	count=0; 
  	while(HX711_DOUT); 
  	for(i=0;i<24;i++)				
	{ 
	  	HX711_SCK=1; 
	  	count=count<<1; 
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
	} 
 	HX711_SCK=1; 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	Delay__hx711_us();
	HX711_SCK=0;  
	return(count);
}

/******************把数据保存到单片机内部eeprom中******************/
void write_eeprom()
{
	SectorErase(0x2000);
	GapValue1=GapValue&0x00ff;
	byte_write(0x2000, GapValue1);
	GapValue1=(GapValue&0xff00)>>8;
	byte_write(0x2001, GapValue1);
	byte_write(0x2060, a_a);	
}

/******************把数据从单片机内部eeprom中读出来*****************/
void read_eeprom()
{
	GapValue   = byte_read(0x2001);
	GapValue   = (GapValue<<8)|byte_read(0x2000);
	a_a      = byte_read(0x2060);
}

/**************开机自检eeprom初始化*****************/
void init_eeprom() 
{
	read_eeprom();		//先读
	if(a_a != 1)		//新的单片机初始单片机内问eeprom
	{
		GapValue  = 3500;
		a_a = 1;
		write_eeprom();	   //保存数据
	}	
}

//显示重量，单位kg，两位整数，三位小数
void Display_Weight()
{
	lcd_pos(2,3); 
//	lcd_wdat(' ');
	lcd_wdat(Weight_Shiwu/10000 + 0x30);
	lcd_wdat(Weight_Shiwu%10000/1000+0x30);
	lcd_wdat('.');
	lcd_wdat(Weight_Shiwu%1000/100 + 0x30);
//	lcd_wdat(Weight_Shiwu%100/10 + 0x30);
//	lcd_wdat(Weight_Shiwu%10 +0x30);
	lcd_wdat('k');
	lcd_wdat('g');
}

//****************************************************
//称重
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取净重
	Weight_Shiwu = (unsigned int)((float)(Weight_Shiwu*10)/GapValue)-qupi; 	//计算实物的实际重量																
	if(Weight_Shiwu >= 10000)		//超重报警
	{
		Buzzer = 0;	
		LED=0;
		Delay_ms(500);
		Buzzer = 1;	
		LED=1;
		
		lcd_pos(2,3); 
		lcd_wdat('-');
		lcd_wdat('-');
		lcd_wdat('.');
		lcd_wdat('-');
		lcd_wdat('-');
		lcd_wdat('-'); 
	}
	else
	{
		if(Weight_Shiwu==0)     ;
		else if(Weight_Shiwu>0) ; 
		if(Weight_Shiwu>(wsbeep+chazhi)||Weight_Shiwu<(wsbeep-chazhi))
		{
		 Buzzer=1;
		 LED=1;
		}
		else{Buzzer = 0;LED=0;}
		Display_Weight();
	}
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
	unsigned char clear;
mm:	Weight_Maopi_0 = HX711_Read();
	for(clear=0;clear<10;clear++)
	{
		Buzzer=1;
		Delay_ms(100);
		Delay_ms(100);	
	}
	Weight_Maopi = HX711_Read();
	if(Weight_Maopi/GapValue!=Weight_Maopi_0/GapValue)
	goto mm;
	Buzzer=0;
	Delay_ms(500);
	Buzzer=1;
} 

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<121;j++);
}

//定时器0初始化
void Timer0_Init()
{
	ET0 = 1;        //允许定时器0中断
	TMOD = 1;       //定时器工作方式选择
	TL0 = 0xb0;     
	TH0 = 0x3c;     //定时器赋予初值
	TR0 = 1;        //启动定时器
}

//定时器0中断
void Timer0_ISR (void) interrupt 1 using 0
{
	uchar Counter;
	TL0 = 0xb0;
	TH0 = 0x3c;     //定时器赋予初值

	//每0.5秒钟刷新重量
    Counter ++;
    if (Counter >= 10)
    {
       FlagTest = 1;
	   Counter = 0;
    }
}
/*在主函数中调用下列语句*/
/*
在使用下列代码段之前需
uchar  dis1[]={"Weight:00.000 kg"};
init_eeprom();  //开始初始化保存的数据
lcd_init();                //初始化LCD  							
EA = 0;
Timer0_Init();
//初中始化完成，开断
EA = 1;
Get_Maopi();
/***********************
while(1)
	{
       //每0.5秒称重一次
		if (FlagTest==1)
		{
			Get_Weight();
			FlagTest = 0;
		}	 
	}
*/
#endif