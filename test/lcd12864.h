#ifndef __LCD_12864_H_
#define __LCD_12864_H_
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int

#define LCD_data  P0             //数据口
//sbit LCD_RS  =  P1^0;            //寄存器选择输入 
//sbit LCD_RW  =  P1^1;            //液晶读/写控制
//sbit LCD_EN  =  P1^2;            //液晶使能控制
//sbit LCD_PSB =  P1^3;            //串/并方式控制

//sbit LCD_RS  =  P1^5;            //寄存器选择输入 
//sbit LCD_RW  =  P1^6;            //液晶读/写控制
//sbit LCD_EN  =  P1^7;            //液晶使能控制
//sbit LCD_PSB =  P4^2;            //串/并方式控制

//sbit LCD_RS = P3^5; //寄存器选择输入
//sbit LCD_RW = P3^6; //液晶读/写控制
//sbit LCD_EN = P3^4; //液晶使能控制
//sbit LCD_PSB = P3^7; //串/并方式控制
sbit LCD_RS = P1^5; //???????
sbit LCD_RW = P1^6; //???/???
sbit LCD_EN = P1^7; //??????


/*******************************************************************/
/*                                                                 */
/*  延时函数                                                       */
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
/*检查LCD忙状态                                                    */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */
/*                                                                 */
/*******************************************************************/
bit lcd_busy()
 { 
                           
    bit result;
    P0=0X00;              //关掉数码管的信号。阻止数码管受到P0口信号的影响
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
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
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
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
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
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 

//  LCD_PSB = 1;         //并口方式
    
    lcd_wcmd(0x34);      //扩充指令操作
    delay(5);
    lcd_wcmd(0x30);      //基本指令操作
    delay(5);
    lcd_wcmd(0x0C);      //显示开，关光标
    delay(5);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(5);
}

/*********************************************************/
/*                                                       */
/* 设定显示位置                                          */
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
   lcd_wcmd(pos);     //显示地址
}
////////////////////////////////////////////////////////
void lcd_write(int a,int b,uchar dis[])
{
	uint i;
  lcd_pos(a,b);             //设置显示位置为第一行的第1个字符
     i = 0;
    while(dis[i] != '\0')
     {                         //显示字符
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