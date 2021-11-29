#ifndef __DS1302_H_
#define __DS1302_H_

/*******************************************************************/
/*                                                                 */
/*  DS1302                                                         */
/*                                                                 */
/*******************************************************************/

/*DS1302 �˿ڶ���*/
sbit SCK = P3^7;		
sbit DA = P3^6;		
sbit RST = P3^5;
bit add;

void lcd_TimeDispaly();
uint set_year();
uchar K1=1,K2=1,K3=1;
uchar shijian[]={1,55,13,29,12,5,20};  //1��55��13ʱ18��12��5��20��
uint mon,day,hour,min,year,sec,week;
//uint y=20,w=5,m=12,d=18,h=13,mi=55,s=55;
uchar zhou[]={"��"};

void Write_Ds1302_Byte(unsigned  char temp) //д�ֽ�
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		DA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}

void Write_Ds1302( unsigned char address,unsigned char dat )   //д����  
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
 	_nop_();  
 	Write_Ds1302_Byte(address);	
 	Write_Ds1302_Byte((dat/10<<4)|(dat%10));		
 	RST=0; 
}

unsigned char Read_Ds1302 ( unsigned char address )  //������
{
 	unsigned char i,temp=0x00,dat1,dat2;
 	RST=0;
_nop_();
 	SCK=0;
	_nop_();
 	RST=1;
	nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(DA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;
	_nop_();
 	RST=0;
	SCK=0;
	_nop_();
	SCK=1;
_nop_();
	DA=0;
	_nop_();
	DA=1;
	_nop_();
	dat1=temp/16;
	dat2=temp%16;
	temp=dat1*10+dat2;
	return (temp);			
}

// DS1302��λ(����ֵд��1302)
void Ds_init(void)
{
  unsigned char i,add;
  add=0x80;
  Write_Ds1302(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		Write_Ds1302(add,shijian[i]);
		add=add+2;
	}
	Write_Ds1302(0x8e,0x80);
}

void Ds_get(void)//��ȡʱ��
{
   unsigned char i, add;
   add=0x81;
   Write_Ds1302(0x8e,0x00);
   for(i=0;i<7;i++)
	{
		shijian[i]=Read_Ds1302(add);
		add=add+2;
	}
	Write_Ds1302(0x8e,0x80);
	year=shijian[6];
  mon=shijian[4];
	day=shijian[3];
	hour=shijian[2];
	min=shijian[1];
	sec=shijian[0];
	week=shijian[5];  
}

void lcd_TimeDispaly()  // ʱ����ʾ����
{
	//Ds_get();		
	lcd_pos(0,0); 	//��ʾ����
//		
//  lcd_wdat(year/10 + 48);//��
//	lcd_wdat(year%10 + 48);
//	lcd_wdat(0x2f);		
//	lcd_wdat(mon/10 + 48);//��
//	lcd_wdat(mon%10 + 48);
//	lcd_wdat(0x2f); 
//  lcd_wdat(day/10 + 48);//�� 	
//	lcd_wdat(day%10 + 48); 
//	for(i=0;i<2;i++)
//		{
//			lcd_wdat(zhou[i]);	
//		}
//  lcd_wdat(week+ 48);//�� 
  	
 	
//  lcd_pos(1,0);           //��ʾʱ��
	lcd_wdat(hour/10 + 48);//ʱ
  lcd_wdat(hour%10 + 48);
	lcd_wdat(0x3a);
  lcd_wdat(min/10 + 48);//��
  lcd_wdat(min%10 + 48); 	
	lcd_wdat(0x3a);
	lcd_wdat(sec/10 + 48);//��
  lcd_wdat(sec%10 + 48);
}

/*******************************************************************/
/*                                                                 */
/*����ʱ��                                                         */
/*                                                                 */
/*******************************************************************/
void set_time( uchar num)
{
	switch(num)
				{
					case 0:
							if(add){year++;}
								else{
									year--;								
								};
								delay(500);
						break;
					case 1:
							if(add){mon++;}
								else{
									mon--;
								};
								if(mon>12){mon=0;};
								delay(500);
						break;
					case 2:
							if(add){day++;}
								else{
									day--;
								};
								if(day>31){day=0;};
								delay(500);
						break;
					case 3:
							if(add){hour++;}
								else{
									hour--;
								};
								if(hour>23){hour=0;};
								delay(500);
						break;
					case 4:
							if(add){min++;}
								else{
									min--;
								};
								if(min>59){min=0;};
								delay(500);
						break;
					case 5:
							if(add){week++;}
								else{
									week--;
								};
								if(week>7){week=0;};
								if(week==0){week=7;};
								delay(500);
						break;
         }	
}

void change_time(uchar year,uchar mon,uchar day,uchar hour,uchar min,uchar week)
{
	shijian[6]=year;
	shijian[4]=mon;
	shijian[3]=day;
	shijian[2]=hour;
	shijian[1]=min;
	shijian[0]=sec;
	shijian[5]=week;
	 Ds_init();
}
#endif