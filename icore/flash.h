#ifndef __flash_H
#define __flash_H
#include "headfile.h"

typedef union                                                                   // 固定数据存储缓单元格式
{
    float   float_type; 
                                              // float  类型
    uint32_t  uint32_type;                    // uint32 类型
    int32_t   int32_type;                     // int32  类型
    uint16_t  uint16_type;                    // uint16 类型
    int16_t   int16_type;                     // int16  类型
    uint8_t   uint8_type;                     // uint8  类型
    int8_t    int8_type;                     // int8   类型
}flash_data_union;

// STM32F407VGT6 Flash配置
#define EEPROM_PAGE_LENGTH           (1024)   // 页数据长度
#define FLASH_PAGE_SIZE              (1024)   // 定义页大小为 1 KB
#define FLASH_PAGE_NUM_MAX           (12)     // 最大页数（STM32F407有12个扇区）

// Flash扇区地址定义（STM32F407VGT6）
#define FLASH_BASE_ADDRESS           (0x08000000)  // Flash起始地址
#define FLASH_SECTOR_10_ADDR         (0x080C0000)  // Sector 10起始地址
#define FLASH_SECTOR_11_ADDR         (0x080E0000)  // Sector 11起始地址

// 扇区定义
#define FLASH_SECTOR                 (11)     // 参数存储页位置 Sector 11
#define PID_FLASH_SECTOR             (11)     // PID参数存储页位置，修改为使用Sector 11
#define PID_PARAM_COUNT              (10)     // PID参数最大数量

// 辅助宏定义
#define IS_FLASH_PAGE_NUM(PAGE)      ((PAGE) < FLASH_PAGE_NUM_MAX)

extern float test;
extern flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH]; 

// 函数声明
void flash_set_buffer_PARA(void);
void flash_get_buffer_PARA(void);
HAL_StatusTypeDef flash_set_buffer_PID(float *pid_params, uint16_t param_count);
HAL_StatusTypeDef flash_get_buffer_PID(float *pid_params, uint16_t param_count);
void flash_buffer_clear(void);
HAL_StatusTypeDef flash_write_page(uint32_t page_num);
void stm32_flash_erase_page(uint32_t page_num);
void flash_read_page_to_buffer(uint32_t sector_num, uint32_t page_num);

#endif
