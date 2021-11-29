#ifndef  __HX711_H_
#define  __HX711_H_
#define uchar unsigned char
#define uint  unsigned int
#include <eeprom52.h>
//�������߱�������
extern void Delay_ms(unsigned int n);
extern void Get_Maopi();
extern void Get_Weight();
unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
unsigned long Weight_Maopi_0 = 0;
long Weight_Shiwu = 0;
long wsbeep=200;    //ϵͳ�趨ֵΪ200g
uint chazhi=20;     //��������ֵ��180-220֮��

unsigned int qupi=0;
uint GapValue,GapValue1;

//�����ʶ
volatile bit FlagTest = 0;		//��ʱ���Ա�־��ÿ0.5����λ��������0
sbit LED=P3^5;
sbit Buzzer = P1^0;
/////////////////////AD���ض�ȡ����/////////////////

//IO����
sbit HX711_DOUT=P1^2; 
sbit HX711_SCK=P1^3;


//****************************************************
//��ʱ����
//****************************************************
void Delay__hx711_us(void)
{
	_nop_();
	_nop_();
}

//****************************************************
//��ȡHX711
//****************************************************
unsigned long HX711_Read(void)	//����128
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
    count=count^0x800000;//��25�������½�����ʱ��ת������
	Delay__hx711_us();
	HX711_SCK=0;  
	return(count);
}

/******************�����ݱ��浽��Ƭ���ڲ�eeprom��******************/
void write_eeprom()
{
	SectorErase(0x2000);
	GapValue1=GapValue&0x00ff;
	byte_write(0x2000, GapValue1);
	GapValue1=(GapValue&0xff00)>>8;
	byte_write(0x2001, GapValue1);
	byte_write(0x2060, a_a);	
}

/******************�����ݴӵ�Ƭ���ڲ�eeprom�ж�����*****************/
void read_eeprom()
{
	GapValue   = byte_read(0x2001);
	GapValue   = (GapValue<<8)|byte_read(0x2000);
	a_a      = byte_read(0x2060);
}

/**************�����Լ�eeprom��ʼ��*****************/
void init_eeprom() 
{
	read_eeprom();		//�ȶ�
	if(a_a != 1)		//�µĵ�Ƭ����ʼ��Ƭ������eeprom
	{
		GapValue  = 3500;
		a_a = 1;
		write_eeprom();	   //��������
	}	
}

//��ʾ��������λkg����λ��������λС��
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
//����
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡ����
	Weight_Shiwu = (unsigned int)((float)(Weight_Shiwu*10)/GapValue)-qupi; 	//����ʵ���ʵ������																
	if(Weight_Shiwu >= 10000)		//���ر���
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
//��ȡëƤ����
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
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<121;j++);
}

//��ʱ��0��ʼ��
void Timer0_Init()
{
	ET0 = 1;        //����ʱ��0�ж�
	TMOD = 1;       //��ʱ��������ʽѡ��
	TL0 = 0xb0;     
	TH0 = 0x3c;     //��ʱ�������ֵ
	TR0 = 1;        //������ʱ��
}

//��ʱ��0�ж�
void Timer0_ISR (void) interrupt 1 using 0
{
	uchar Counter;
	TL0 = 0xb0;
	TH0 = 0x3c;     //��ʱ�������ֵ

	//ÿ0.5����ˢ������
    Counter ++;
    if (Counter >= 10)
    {
       FlagTest = 1;
	   Counter = 0;
    }
}
/*���������е����������*/
/*
��ʹ�����д����֮ǰ��
uchar  dis1[]={"Weight:00.000 kg"};
init_eeprom();  //��ʼ��ʼ�����������
lcd_init();                //��ʼ��LCD  							
EA = 0;
Timer0_Init();
//����ʼ����ɣ�����
EA = 1;
Get_Maopi();
/***********************
while(1)
	{
       //ÿ0.5�����һ��
		if (FlagTest==1)
		{
			Get_Weight();
			FlagTest = 0;
		}	 
	}
*/
#endif