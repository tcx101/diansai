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

#define EEPROM_PAGE_LENGTH           (1024)  
#define FLASH_PAGE_SIZE    1024    // 定义页大小为 1 KB
#define FLASH_PAGE_NUM_MAX 11      // 最大页数
#define FLASH_SECTOR       11      // 参数存储页位置 Sector 11
#define PID_FLASH_SECTOR   10      // PID参数存储页位置 Sector 10
#define PID_PARAM_COUNT    10      // PID参数最大数量

extern float test;
extern flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH]; 
void flash_set_buffer_PARA();
void flash_get_buffer_PARA();
void flash_set_buffer_PID(float *pid_params, uint16_t param_count);
void flash_get_buffer_PID(float *pid_params, uint16_t param_count);
void flash_buffer_clear(void);
#endif
