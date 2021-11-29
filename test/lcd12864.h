#ifndef __LCD_12864_H_
#define __LCD_12864_H_
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int

#define LCD_data  P0             //���ݿ�
//sbit LCD_RS  =  P1^0;            //�Ĵ���ѡ������ 
//sbit LCD_RW  =  P1^1;            //Һ����/д����
//sbit LCD_EN  =  P1^2;            //Һ��ʹ�ܿ���
//sbit LCD_PSB =  P1^3;            //��/����ʽ����

//sbit LCD_RS  =  P1^5;            //�Ĵ���ѡ������ 
//sbit LCD_RW  =  P1^6;            //Һ����/д����
//sbit LCD_EN  =  P1^7;            //Һ��ʹ�ܿ���
//sbit LCD_PSB =  P4^2;            //��/����ʽ����

//sbit LCD_RS = P3^5; //�Ĵ���ѡ������
//sbit LCD_RW = P3^6; //Һ����/д����
//sbit LCD_EN = P3^4; //Һ��ʹ�ܿ���
//sbit LCD_PSB = P3^7; //��/����ʽ����
sbit LCD_RS = P1^5; //???????
sbit LCD_RW = P1^6; //???/???
sbit LCD_EN = P1^7; //??????


/*******************************************************************/
/*                                                                 */
/*  ��ʱ����                                                       */
/*                                                                 */
/*******************************************************************/
void delay(int ms)
{
    while(ms--)
	{
      unsigned char i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}

/*******************************************************************/
/*                                                                 */
/*���LCDæ״̬                                                    */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */
/*                                                                 */
/*******************************************************************/
bit lcd_busy()
 { 
                           
    bit result;
    P0=0X00;              //�ص�����ܵ��źš���ֹ������ܵ�P0���źŵ�Ӱ��
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delay(1);
    result = (bit)(P0&0x80);
    LCD_EN = 0;
    return(result); 
 }
 
/*******************************************************************/
/*                                                                 */
/*дָ�����ݵ�LCD                                                  */
/*RS=L��RW=L��E=�����壬D0-D7=ָ���롣                             */
/*                                                                 */
/*******************************************************************/
void lcd_wcmd(unsigned char cmd)
{   
                         
   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
 delay(1);
    LCD_EN = 1;
 delay(1);
    LCD_EN = 0;  
}

/*******************************************************************/
/*                                                                 */
/*д��ʾ���ݵ�LCD                                                  */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(unsigned char dat)
{                          
   while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delay(1);
    LCD_EN = 1;
    delay(1);
    LCD_EN = 0; 
}

/*******************************************************************/
/*                                                                 */
/*  LCD��ʼ���趨                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 

//  LCD_PSB = 1;         //���ڷ�ʽ
    
    lcd_wcmd(0x34);      //����ָ�����
    delay(5);
    lcd_wcmd(0x30);      //����ָ�����
    delay(5);
    lcd_wcmd(0x0C);      //��ʾ�����ع��
    delay(5);
    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay(5);
}

/*********************************************************/
/*                                                       */
/* �趨��ʾλ��                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(unsigned char X,unsigned char Y)
{                          
   unsigned char  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd_wcmd(pos);     //��ʾ��ַ
}
////////////////////////////////////////////////////////
void lcd_write(int a,int b,uchar dis[])
{
	uint i;
  lcd_pos(a,b);             //������ʾλ��Ϊ��һ�еĵ�1���ַ�
     i = 0;
    while(dis[i] != '\0')
     {                         //��ʾ�ַ�
       lcd_wdat(dis[i]);
       i++;
     }
}
///////////////////////////////////////////////////////
lcd_gunping()
{
	lcd_wcmd(0x01);
  lcd_write(2,0,"ZNXYJKZXT");
	lcd_write(3,0,"0311");
	delay(2000);
	
	lcd_wcmd(0x01);
  lcd_write(1,0,"ZNXYJKZXT");
	lcd_write(2,0,"0311");
	delay(2000);
	
	lcd_wcmd(0x01);
  lcd_write(0,0,"ZNXYJKZXT");
	lcd_write(1,0,"0311");
	delay(2000);
	
	lcd_wcmd(0x01);
  lcd_write(3,0,"ZNXYJKZXT");
	lcd_write(0,0,"0311");
	delay(2000);
	
	lcd_wcmd(0x01);
  lcd_write(2,0,"ZNXYJKZXT");
	lcd_write(3,0,"0311");
	delay(2000);
	
	lcd_wcmd(0x01);
}
#endif