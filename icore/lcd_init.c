#include "lcd_init.h"
#include "lcd.h"

// 删除extern声明，改用HAL库延时函数
// HAL库延时函数
void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能GPIOC和GPIOD时钟，因为LCD引脚分布在这两个GPIO上
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    // 设置所有引脚为输出模式，并初始化为高电平
    // GPIOD上的引脚：SCL(PD5)、SDA(PD7)、RES(PD3)、DC(PD2)、BLK(PD1)、CS(PD4)
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_4, GPIO_PIN_SET);
    
    // 初始化GPIOD控制引脚
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;         // 上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
    
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    // 初始状态设置
    LCD_CS_Set();   // CS默认高电平
    LCD_SCLK_Set(); // SCK默认高电平
    LCD_MOSI_Set(); // MOSI默认高电平
}

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
    uint8_t i;
    LCD_CS_Clr();
    
    // 降低通信速度，增加稳定性
    for(i=0; i<8; i++)
    {			  
        LCD_SCLK_Clr();
        // 增加短延时，提高通信稳定性
        for(volatile uint8_t j=0; j<2; j++);
        
        if(dat & 0x80)
        {
           LCD_MOSI_Set();
        }
        else
        {
           LCD_MOSI_Clr();
        }
        
        // 增加短延时，提高通信稳定性
        for(volatile uint8_t j=0; j<2; j++);
        
        LCD_SCLK_Set();
        // 增加短延时，提高通信稳定性
        for(volatile uint8_t j=0; j<2; j++);
        
        dat <<= 1;
    }	
    LCD_CS_Set();	
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_Writ_Bus(dat);
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
    LCD_Writ_Bus(dat>>8);
    LCD_Writ_Bus(dat);
}

/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
    LCD_DC_Clr();//写命令
    LCD_Writ_Bus(dat);
    LCD_DC_Set();//写数据
}

void LCD_Init(void)
{
    LCD_GPIO_Init(); // 初始化GPIO
    
    // 确保所有控制信号处于非活动状态
    LCD_CS_Set();  // CS高电平
    LCD_DC_Set();  // DC高电平
    LCD_BLK_Clr(); // 背光关闭
    
    // 加强复位序列
    LCD_RES_Clr();
    delay_ms(120);  // 增加复位时间
    LCD_RES_Set();
    delay_ms(120);  // 增加复位恢复时间
    
    // 发送软复位命令
    LCD_WR_REG(0x01); // 软件复位
    delay_ms(150);    // 增加复位后等待时间
    
    // ST7735屏幕标准初始化序列
    LCD_WR_REG(0x11); // 退出睡眠模式
    delay_ms(150);    // 增加等待时间
    
    // 设置像素格式为16位/像素 - 提前设置
    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);
    
    // 帧率控制 - 降低帧率以提高稳定性
    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x02); // 降低帧率，原值为0x01
    LCD_WR_DATA8(0x35); // 增加行周期，原值为0x2C
    LCD_WR_DATA8(0x36); // 增加行周期，原值为0x2D
    
    // 帧率控制（空闲模式/8色）
    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x02); // 降低帧率
    LCD_WR_DATA8(0x35);
    LCD_WR_DATA8(0x36);
    
    // 帧率控制（部分模式）
    LCD_WR_REG(0xB3);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x35);
    LCD_WR_DATA8(0x36);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x35);
    LCD_WR_DATA8(0x36);
    
    // 反向控制
    LCD_WR_REG(0xB4);
    LCD_WR_DATA8(0x03); // 修改反相设置，原值为0x07
    
    // 电源控制1 - 增强稳定性
    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0xA2);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x84);
    
    // 电源控制2 - 增强稳定性
    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0xC5);
    
    // 电源控制3 (普通模式)
    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x0D); // 增加电压值，原值为0x0A
    LCD_WR_DATA8(0x00);
    
    // 电源控制4 (空闲模式)
    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x8D); // 增加电压值，原值为0x8A
    LCD_WR_DATA8(0x2A);
    
    // 电源控制5 (部分模式)
    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x8D); // 增加电压值，原值为0x8A
    LCD_WR_DATA8(0xEE);
    
    // VCOM控制1 - 调整VCOM电压
    LCD_WR_REG(0xC5);
    LCD_WR_DATA8(0x12); // 增加VCOM电压，原值为0x0E
    
    // 显示反转控制 - 可能有助于解决花屏
    LCD_WR_REG(0xB0);
    LCD_WR_DATA8(0x00); // 正常扫描顺序
    
    // 设置显示方向 - 确保正确方向
    LCD_WR_REG(0x36);
    if (USE_HORIZONTAL == 0)
        LCD_WR_DATA8(0xC8); // 竖屏模式1
    else if (USE_HORIZONTAL == 1)
        LCD_WR_DATA8(0x08); // 竖屏模式2
    else if (USE_HORIZONTAL == 2)
        LCD_WR_DATA8(0xA8); // 横屏模式1
    else
        LCD_WR_DATA8(0x68); // 横屏模式2
    
    // ST7735S Gamma序列 - 优化以减少花屏
    LCD_WR_REG(0xE0); // 正Gamma
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x22);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x30);
    LCD_WR_DATA8(0x25);
    LCD_WR_DATA8(0x2A);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x3A);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x13);
    
    LCD_WR_REG(0xE1); // 负Gamma
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x16);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x2D);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x23);
    LCD_WR_DATA8(0x27);
    LCD_WR_DATA8(0x27);
    LCD_WR_DATA8(0x25);
    LCD_WR_DATA8(0x2D);
    LCD_WR_DATA8(0x3B);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x13);
    
    // 设置窗口大小 - 使用完整屏幕范围
    // 列地址设置
    LCD_WR_REG(0x2A);
    LCD_WR_DATA8(0x00); // 起始列高8位
    LCD_WR_DATA8(0x00); // 起始列低8位
    LCD_WR_DATA8(0x00); // 结束列高8位
    LCD_WR_DATA8(0x7F); // 结束列低8位 (对应128-1像素)
    
    // 行地址设置
    LCD_WR_REG(0x2B);
    LCD_WR_DATA8(0x00); // 起始行高8位
    LCD_WR_DATA8(0x00); // 起始行低8位
    LCD_WR_DATA8(0x00); // 结束行高8位
    LCD_WR_DATA8(0x9F); // 结束行低8位 (对应160-1像素)
    
    // 内存数据访问控制 - 尝试不同的扫描方向
    LCD_WR_REG(0x36);
    if (USE_HORIZONTAL == 0)
        LCD_WR_DATA8(0xC8); // 竖屏模式1: MY=1, MX=1, MV=0, BGR=0
    else if (USE_HORIZONTAL == 1)
        LCD_WR_DATA8(0x08); // 竖屏模式2: MY=0, MX=0, MV=0, BGR=0
    else if (USE_HORIZONTAL == 2)
        LCD_WR_DATA8(0xA8); // 横屏模式1: MY=1, MX=0, MV=1, BGR=0
    else
        LCD_WR_DATA8(0x68); // 横屏模式2: MY=0, MX=1, MV=1, BGR=0
    
    // 关闭色彩反转
    LCD_WR_REG(0x21);
    
    // 开启显示
    LCD_WR_REG(0x29);
    delay_ms(20);
    
    // 准备写入RAM
    LCD_WR_REG(0x2C);
    
    // 填充黑色清屏 - 多次清屏以确保稳定
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    delay_ms(20);
    
    // 再次清屏
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    delay_ms(20);
    
    // 第三次清屏
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    delay_ms(20);
    
    // 再次设置写入RAM
    LCD_WR_REG(0x2C);
    
    // 打开背光前等待稳定
    delay_ms(100);
    LCD_BLK_Set();
    
    // 最后一次调整显示窗口，确保稳定
    LCD_WR_REG(0x2A);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x7F);
    
    LCD_WR_REG(0x2B);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x9F);
    
    // 准备最终写入
    LCD_WR_REG(0x2C);
} 








