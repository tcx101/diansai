#ifndef __flash_H
#define __flash_H
#include "headfile.h"

typedef union                                                                   // �̶������ݻ��嵥Ԫ��ʽ
{
    float   float_type; 
                                              // float  ����
    uint32_t  uint32_type;                    // uint32 ����
    int32_t   int32_type;                     // int32  ����
    uint16_t  uint16_type;                    // uint16 ����
    int16_t   int16_type;                     // int16  ����
    uint8_t   uint8_type;                     // uint8  ����
    int8_t    int8_type;                     // int8   ����
}flash_data_union;

#define EEPROM_PAGE_LENGTH           (1024)  
#define FLASH_PAGE_SIZE    1024    // ����ҳ��СΪ 1 KB
#define FLASH_PAGE_NUM_MAX 11      // ���ҳ��
#define FLASH_SECTOR       11      // ��������ҳλ�� Sector 11
extern float test;
extern flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH]; 
void flash_set_buffer_PARA();
void flash_get_buffer_PARA();
void flash_set_buffer_PID(uint16_t page_offset) ;
#endif
