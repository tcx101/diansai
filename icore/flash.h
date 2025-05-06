#ifndef __flash_H
#define __flash_H
#include "headfile.h"

typedef union                                                                   // 固定的数据缓冲单元格式
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
#define EEPROM_PAGE_LENGTH           (1024)  
#define FLASH_PAGE_SIZE    1024    // 假设页大小为 1 KB
#define FLASH_PAGE_NUM_MAX 11      // 最大页号
#define FLASH_SECTOR       11      // 假设所有页位于 Sector 11
extern float test;
extern flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH]; 
void flash_set_buffer_PARA();
void flash_get_buffer_PARA();

#endif
