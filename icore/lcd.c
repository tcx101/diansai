#include "lcd.h"
#include "lcd_init.h"
#include "lcdfont.h"



/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
                color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(color);
} 


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要绘制的点是否过远
		{
			b--;
		}
	}
}

/******************************************************************************
      函数说明：显示汉字字符
      入口数据：x,y显示坐标
                *s 要显示的汉字字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	while(*s!=0)
	{
		if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}


/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp,sizex;
	uint16_t num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}

void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    // 确保坐标在有效范围内
    if(x1 >= LCD_W) x1 = LCD_W-1;
    if(x2 >= LCD_W) x2 = LCD_W-1;
    if(y1 >= LCD_H) y1 = LCD_H-1;
    if(y2 >= LCD_H) y2 = LCD_H-1;
    
    // ST7735的地址设置 - 不要添加偏移，让屏幕使用完整显示区域
    LCD_WR_REG(0x2A);    // 列地址设置
    LCD_WR_DATA8(0x00);  // 起始列高8位
    LCD_WR_DATA8(x1);    // 起始列低8位
    LCD_WR_DATA8(0x00);  // 结束列高8位
    LCD_WR_DATA8(x2);    // 结束列低8位
    
    LCD_WR_REG(0x2B);    // 行地址设置
    LCD_WR_DATA8(0x00);  // 起始行高8位
    LCD_WR_DATA8(y1);    // 起始行低8位
    LCD_WR_DATA8(0x00);  // 结束行高8位
    LCD_WR_DATA8(y2);    // 结束行低8位
    
    LCD_WR_REG(0x2C);    // 准备写入显示RAM
}

void LCD_show()
{
	
	  if(page==1)
	  {
		
			LCD_Fill(0, 0, 128, 90, BLACK);
			/*AB*/
      //LCD_DrawLine(scale_480_to_128(point_data.A1[0]), scale_480_to_128(point_data.A1[1]),scale_480_to_128(point_data.B1[0]),scale_480_to_128(point_data.B1[1]),RED);
     // LCD_DrawLine(scale_480_to_128(point_data.A2[0]), scale_480_to_128(point_data.A2[1]),scale_480_to_128(point_data.B2[0]),scale_480_to_128(point_data.B2[1]),BLUE);
      LCD_DrawLine(scale_480_to_128(cal_mid(point_data.A1[0],point_data.A2[0])), scale_480_to_128(AB_line[cal_mid(point_data.A1[0],point_data.A2[0])]),scale_480_to_128(cal_mid( point_data.B1[0],point_data.B2[0])),scale_480_to_128(AB_line[cal_mid( point_data.B1[0],point_data.B2[0])]),RED);

			
			//LCD_DrawLine(scale_480_to_128(cal_mid(point_data.A1[0],point_data.A2[0])), scale_480_to_128(cal_mid(point_data.A1[1],point_data.A2[1])),scale_480_to_128(cal_mid( point_data.B1[0],point_data.B2[0])),scale_480_to_128( cal_mid(point_data.B1[1],point_data.B2[1])),RED);
		  /*BC*/
			
			LCD_DrawLine(scale_480_to_128(BC_line[cal_mid(point_data.B1[1],point_data.B2[1])]), scale_480_to_128(cal_mid(point_data.B1[1],point_data.B2[1])),scale_480_to_128(BC_line[cal_mid(point_data.C1[1],point_data.C2[1])]),scale_480_to_128(cal_mid(point_data.C1[1],point_data.C2[1])),BLUE);
			//LCD_DrawLine(scale_480_to_128(cal_mid(point_data.B1[0],point_data.B2[0])), scale_480_to_128(BC_line[cal_mid(point_data.B1[0],point_data.B2[0])]),scale_480_to_128(cal_mid( point_data.C1[0],point_data.C2[0])),scale_480_to_128(BC_line[cal_mid( point_data.C1[0],point_data.C2[0])]),BLUE);

			//LCD_DrawLine(scale_480_to_128(point_data.B1[0]), scale_480_to_128(point_data.B1[1]),scale_480_to_128(point_data.C1[0]),scale_480_to_128(point_data.C1[1]),RED);
      //LCD_DrawLine(scale_480_to_128(point_data.B2[0]), scale_480_to_128(point_data.B2[1]),scale_480_to_128(point_data.C2[0]),scale_480_to_128(point_data.C2[1]),BLUE);

			//LCD_DrawLine(scale_480_to_128(cal_mid(point_data.B1[0],point_data.B2[0])), scale_480_to_128(cal_mid(point_data.B1[1],point_data.B2[1])),scale_480_to_128(cal_mid( point_data.C1[0],point_data.C2[0])),scale_480_to_128( cal_mid(point_data.C1[1],point_data.C2[1])),BLUE);
			//LCD_DrawLine(scale_480_to_128(point_data.B1[0]), scale_480_to_128(point_data.B1[1]),scale_480_to_128(point_data.C1[0]),scale_480_to_128(point_data.C1[1]),YELLOW);
			//LCD_DrawLine(scale_480_to_128(cal_mid(point_data.B1[0],point_data.B2[0])), scale_480_to_128(cal_mid(point_data.B1[1],point_data.B2[1])),scale_480_to_128(cal_mid( point_data.C1[0],point_data.C2[0])),scale_480_to_128( cal_mid(point_data.C1[1],point_data.C2[1])),YELLOW);
			/*CD*/
			LCD_DrawLine(scale_480_to_128(cal_mid(point_data.C1[0],point_data.C2[0])), scale_480_to_128(CD_line[cal_mid(point_data.C1[0],point_data.C2[0])]),scale_480_to_128(cal_mid( point_data.D1[0],point_data.D2[0])),scale_480_to_128(CD_line[cal_mid( point_data.D1[0],point_data.D2[0])]),WHITE);

			//LCD_DrawLine(scale_480_to_128(point_data.C1[0]), scale_480_to_128(point_data.C1[1]),scale_480_to_128(point_data.D1[0]),scale_480_to_128(point_data.D1[1]),RED);
      //LCD_DrawLine(scale_480_to_128(point_data.C2[0]), scale_480_to_128(point_data.C2[1]),scale_480_to_128(point_data.D2[0]),scale_480_to_128(point_data.D2[1]),BLUE);

		//	LCD_DrawLine(scale_480_to_128(point_data.C1[0]), scale_480_to_128(point_data.C1[1]),scale_480_to_128(point_data.D1[0]),scale_480_to_128(point_data.D1[1]),BLUE);
      //LCD_DrawLine(scale_480_to_128(cal_mid(point_data.C1[0],point_data.C2[0])), scale_480_to_128(cal_mid(point_data.C1[1],point_data.C2[1])),scale_480_to_128(cal_mid( point_data.D1[0],point_data.D1[0])),scale_480_to_128( cal_mid(point_data.D1[1],point_data.D2[1])),BLUE);
      /*AD*/
			LCD_DrawLine(scale_480_to_128(AD_line[cal_mid(point_data.A1[1],point_data.A2[1])]), scale_480_to_128(cal_mid(point_data.A1[1],point_data.A2[1])),scale_480_to_128(AD_line[cal_mid(point_data.D1[1],point_data.D2[1])]),scale_480_to_128(cal_mid(point_data.D1[1],point_data.D2[1])),GREEN);

			//LCD_DrawLine(scale_480_to_128(point_data.A1[0]), scale_480_to_128(point_data.A1[1]),scale_480_to_128(point_data.D1[0]),scale_480_to_128(point_data.D1[1]),RED);
      //LCD_DrawLine(scale_480_to_128(point_data.A2[0]), scale_480_to_128(point_data.A2[1]),scale_480_to_128(point_data.D2[0]),scale_480_to_128(point_data.D2[1]),BLUE);
			//LCD_DrawLine(scale_480_to_128(point_data.A1[0]), scale_480_to_128(point_data.A1[1]),scale_480_to_128(point_data.D1[0]),scale_480_to_128(point_data.D1[1]),BLUE);

      //LCD_DrawLine(scale_480_to_128(cal_mid(point_data.A1[0],point_data.A2[0])), scale_480_to_128(cal_mid(point_data.A1[1],point_data.A2[1])),scale_480_to_128(cal_mid( point_data.D1[0],point_data.D1[0])),scale_480_to_128( cal_mid(point_data.D1[1],point_data.D2[1])),GREEN);
			/*激光点*/
			LCD_DrawPoint(scale_480_to_128(point_data.light[0]),scale_480_to_128(point_data.light[1]),RED);
			/*中点*/
      LCD_DrawPoint(scale_480_to_128(point_data.mid[0]),scale_480_to_128(point_data.mid[1]),BROWN);
			
//    LCD_DrawPoint(scale_480_to_128(point_data.light[0]-1),scale_320_to_64(point_data.light[1]),GREEN);		
//	  LCD_DrawPoint(scale_480_to_128(point_data.light[0]),scale_320_to_64(point_data.light[1]-1),GREEN);
//	  LCD_DrawPoint(scale_480_to_128(point_data.light[0]),scale_320_to_64(point_data.light[1]+1),GREEN);
			
		LCD_ShowString(X_1,Y_7,"S1P:",BLACK,WHITE,16,0);
		LCD_ShowFloatNum1(X_5,Y_7,servo1.p,3,BLACK,WHITE,16);
		//LCD_ShowIntNum(X_5,Y_7,servo1.p,2,BLACK,WHITE,16);
			
	  LCD_ShowString(X_9,Y_7,"1I:",BLACK,WHITE,16,0);
	  LCD_ShowFloatNum1(X_12,Y_7,servo1.i,3,BLACK,WHITE,16);
		//LCD_ShowIntNum(X_12,Y_7,servo1.i,2,BLACK,WHITE,16);
		
    LCD_ShowString(X_1,Y_8,"S1D:",BLACK,WHITE,16,0);
		LCD_ShowFloatNum1(X_5,Y_8,servo1.d,3,BLACK,WHITE,16);
		//LCD_ShowIntNum(X_5,Y_8,servo1.d,2,BLACK,WHITE,16);
			
	  LCD_ShowString(X_9,Y_8,"2P:",BLACK,WHITE,16,0);
		LCD_ShowFloatNum1(X_12,Y_8,servo2.p,3,BLACK,WHITE,16);
		//LCD_ShowIntNum(X_12,Y_8,servo2.p,2,BLACK,WHITE,16);
		
		LCD_ShowString(X_1,Y_9,"S2I:",BLACK,WHITE,16,0);
		LCD_ShowFloatNum1(X_5,Y_9,servo2.i,3,BLACK,WHITE,16);
		//LCD_ShowIntNum(X_4,Y_9,servo2.i,2,BLACK,WHITE,16);
		
		LCD_ShowString(X_9,Y_9,"2D:",BLACK,WHITE,16,0);
		LCD_ShowFloatNum1(X_12,Y_9,servo2.d,3,BLACK,WHITE,16);
		//LCD_ShowIntNum(X_12,Y_9,servo2.d,2,BLACK,WHITE,16);	
			
//	  LCD_ShowString(X_1,Y_9,"e1:",BLACK,WHITE,16,0);
//		LCD_ShowIntNum(X_4,Y_9,servo1.error[0],3,BLACK,WHITE,16);	
//			
//	  LCD_ShowString(X_8,Y_9,"e2:",BLACK,WHITE,16,0);
//		LCD_ShowIntNum(X_12,Y_9,servo2.error[0],3,BLACK,WHITE,16);			
			
			
	  LCD_ShowString(X_1,Y_10,"m:",BLACK,WHITE,16,0);
		LCD_ShowIntNum(X_3,Y_10,mode,1,BLACK,WHITE,16);
			
	  LCD_ShowString(X_5,Y_10,"p:",BLACK,WHITE,16,0);
		LCD_ShowIntNum(X_7,Y_10,page,1,BLACK,WHITE,16);			

	 // LCD_ShowString(X_5,Y_10,"p:",BLACK,WHITE,16,0);
		//LCD_ShowIntNum(X_7,Y_10,page,1,BLACK,WHITE,16);	

			
		}
		
		else if(page==2)
		{
			LCD_Fill(0, 0, 128, 90, WHITE);
			/*AB*/
      //LCD_DrawLine(scale_480_to_128(cal_mid(point_data.A1[0],point_data.A2[0])), scale_480_to_128(cal_mid(point_data.A1[1],point_data.A2[1])),scale_480_to_128(cal_mid( point_data.B1[0],point_data.B2[0])),scale_480_to_128( cal_mid(point_data.B1[1],point_data.B2[1])),RED);
			LCD_DrawLine(point_data.A1[0], point_data.A1[1],point_data.B1[0],point_data.B1[1],RED);

		  /*BC*/
			//LCD_DrawLine(scale_480_to_128(cal_mid(point_data.B1[0],point_data.B2[0])), scale_480_to_128(cal_mid(point_data.B1[1],point_data.B2[1])),scale_480_to_128(cal_mid( point_data.C1[0],point_data.C2[0])),scale_480_to_128( cal_mid(point_data.C1[1],point_data.C2[1])),YELLOW);
			/*CD*/
      //LCD_DrawLine(scale_480_to_128(cal_mid(point_data.C1[0],point_data.C2[0])), scale_480_to_128(cal_mid(point_data.C1[1],point_data.C2[1])),scale_480_to_128(cal_mid( point_data.D1[0],point_data.D1[0])),scale_480_to_128( cal_mid(point_data.D1[1],point_data.D2[1])),BLUE);
      /*AD*/
      //LCD_DrawLine(scale_480_to_128(cal_mid(point_data.A1[0],point_data.A2[0])), scale_480_to_128(cal_mid(point_data.A1[1],point_data.A2[1])),scale_480_to_128(cal_mid( point_data.D1[0],point_data.D1[0])),scale_480_to_128( cal_mid(point_data.D1[1],point_data.D2[1])),GREEN);
			/*激光点*/
			LCD_DrawPoint(scale_480_to_128(point_data.light[0]),scale_480_to_128(point_data.light[1]),RED);
			/*中点*/
      LCD_DrawPoint(scale_480_to_128(point_data.mid[0]),scale_480_to_128(point_data.mid[0]),BROWN);
			
//			LCD_ShowString(X_1,Y_7,"LX:",BLACK,WHITE,16,0);
//		  LCD_ShowIntNum(X_4,Y_7,point_data.light[0],3,BLACK,WHITE,16);
//			
//	    LCD_ShowString(X_8,Y_7,"LY:",BLACK,WHITE,16,0);
//		  LCD_ShowIntNum(X_12,Y_7,point_data.light[0],3,BLACK,WHITE,16);
			
			LCD_ShowString(X_8,Y_7,"AY1:",BLACK,WHITE,16,0);
		  LCD_ShowIntNum(X_12,Y_7,cal_mid(point_data.A1[1],point_data.A2[1]),3,BLACK,WHITE,16);
			
			LCD_ShowString(X_8,Y_8,"AY2:",BLACK,WHITE,16,0);
		  LCD_ShowIntNum(X_12,Y_8,AB_line[cal_mid(point_data.A1[0],point_data.A2[0])],3,BLACK,WHITE,16);
			
			LCD_ShowString(X_1,Y_10,"m:",BLACK,WHITE,16,0);
		  LCD_ShowIntNum(X_3,Y_10,mode,1,BLACK,WHITE,16);
			
	    LCD_ShowString(X_5,Y_10,"p:",BLACK,WHITE,16,0);
		  LCD_ShowIntNum(X_7,Y_10,page,1,BLACK,WHITE,16);		
			//AB_line[cal_mid(point_data.A1[0],point_data.A2[0])]
		}
		
		
		
	
}





