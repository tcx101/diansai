#include "headfile.h"
float test;
flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];  // FLASH 操作的数据缓冲区
#define FLASH_BUFFER_SIZE 32  // 根据实际需求调整

// 在 STM32 中擦除扇区
void stm32_flash_erase_page(uint32_t page_num) {
    FLASH_EraseInitTypeDef eraseConfig;
    uint32_t sectorError;

    // 参数检查
    if (!IS_FLASH_PAGE_NUM(page_num)) {
        return; // 无效扇区号
    }

    // 配置擦除参数
    eraseConfig.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseConfig.Sector = page_num;
    eraseConfig.NbSectors = 1;
    eraseConfig.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    HAL_FLASH_Unlock();
    if (HAL_FLASHEx_Erase(&eraseConfig, &sectorError) != HAL_OK) {
        // 擦除失败处理
        HAL_FLASH_Lock();
        return;
    }
    HAL_FLASH_Lock();
}

// 获取Flash页地址
uint32_t get_flash_page_address(uint32_t page_num) {
    uint32_t address;
    
    // 对STM32F407VGT6的扇区地址进行映射
    switch (page_num) {
        case 0:  address = 0x08000000; break; // Sector 0, 16KB
        case 1:  address = 0x08004000; break; // Sector 1, 16KB
        case 2:  address = 0x08008000; break; // Sector 2, 16KB
        case 3:  address = 0x0800C000; break; // Sector 3, 16KB
        case 4:  address = 0x08010000; break; // Sector 4, 64KB
        case 5:  address = 0x08020000; break; // Sector 5, 128KB
        case 6:  address = 0x08040000; break; // Sector 6, 128KB
        case 7:  address = 0x08060000; break; // Sector 7, 128KB
        case 8:  address = 0x08080000; break; // Sector 8, 128KB
        case 9:  address = 0x080A0000; break; // Sector 9, 128KB
        case 10: address = FLASH_SECTOR_10_ADDR; break; // Sector 10, 128KB
        case 11: address = FLASH_SECTOR_11_ADDR; break; // Sector 11, 128KB
        default: address = FLASH_SECTOR_11_ADDR; break; // 默认使用Sector 11
    }
    
    return address;
}

/**
  * @brief  将全局缓冲区 flash_union_buffer 的数据写入指定 Flash 页
  * @param  page_num: 页编号（需确保在有效范围内）
  * @retval HAL_StatusTypeDef: 操作状态（HAL_OK 成功，其他为失败）
  * @note   - 调用前需确保目标扇区已被擦除
  *         - 缓冲区 flash_union_buffer 必须按 8 字节对齐
  *         - EEPROM_PAGE_LENGTH 必须为 8 的倍数
  */
HAL_StatusTypeDef flash_write_page(uint32_t page_num) {
    // 参数检查
    if (!IS_FLASH_PAGE_NUM(page_num)) {
        return HAL_ERROR; // 无效页号
    }

    // 确保全局缓冲区按 8 字节对齐（编译时断言）
    typedef char assert_aligned[(uintptr_t)flash_union_buffer % 8 == 0 ? 1 : -1];

    // 计算目标 Flash 地址
    uint32_t address = get_flash_page_address(page_num);

    // 获取双字指针（64 位对齐）
    uint64_t *buffer = (uint64_t *)flash_union_buffer;
    uint32_t words_to_write = EEPROM_PAGE_LENGTH / 8; // 计算双字数

    // 解锁 Flash
    HAL_FLASH_Unlock();

    // 按双字写入数据
    for (uint32_t i = 0; i < words_to_write; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 
                            address + (i * 8), 
                            buffer[i]) != HAL_OK) {
            HAL_FLASH_Lock(); // 失败时重新加锁
            return HAL_ERROR;
        }
    }

    // 重新加锁 Flash
    HAL_FLASH_Lock();
    return HAL_OK;
}

// 清空缓冲区
void flash_buffer_clear(void) {
    memset(flash_union_buffer, 0xFF, EEPROM_PAGE_LENGTH);
}

/**
  * @brief 从指定 Flash 页读取数据到全局缓冲区 flash_union_buffer
  * @param sector_num      保留参数（未使用，仅为兼容原接口）
  * @param page_num        页编号（范围需根据实际 Flash 大小定义，如 0~11）
  * @retval void
  */
void flash_read_page_to_buffer(uint32_t sector_num, uint32_t page_num) {
    // 参数检查
    if (!IS_FLASH_PAGE_NUM(page_num)) {
        return; // 无效页号
    }

    // 计算 Flash 物理地址
    uint32_t flash_addr = get_flash_page_address(page_num);

    // 直接内存拷贝（Flash 地址空间可读）
    memcpy(flash_union_buffer, (void*)flash_addr, FLASH_PAGE_SIZE);
}

/**
  * @brief 将通用参数写入Flash
  * @retval None
  */
void flash_set_buffer_PARA(void) {
    // 第一步清除缓冲区
    flash_buffer_clear();
    
    // 第二步把数据存到缓冲区
    flash_union_buffer[1].float_type = test;
    
    // 第三步把FLASH数据擦除
    stm32_flash_erase_page(FLASH_SECTOR);
    
    // 第四步存到FLASH指定扇区
    if (flash_write_page(1) != HAL_OK) {
        // 写入失败处理（如有需要）
    }
}

/**
  * @brief 从Flash读取通用参数
  * @retval None
  */
void flash_get_buffer_PARA(void) {
    flash_read_page_to_buffer(FLASH_SECTOR, 1);
    test = flash_union_buffer[1].float_type;
}

/**
  * @brief 将PID参数写入Flash
  * @param pid_params : PID参数数组指针
  * @param param_count : 参数数量（不能超过PID_PARAM_COUNT）
  * @retval HAL_StatusTypeDef 操作状态
  */
HAL_StatusTypeDef flash_set_buffer_PID(float *pid_params, uint16_t param_count) {
    // 参数检查
    if (pid_params == NULL || param_count > PID_PARAM_COUNT) {
        return HAL_ERROR; // 参数错误
    }

    // 1. 清除缓冲区
    flash_buffer_clear();

    // 2. 将PID参数存入缓冲区
    for (uint16_t i = 0; i < param_count; i++) {
        flash_union_buffer[i].float_type = pid_params[i];
    }

    // 3. 擦除Flash扇区
    stm32_flash_erase_page(PID_FLASH_SECTOR);

    // 4. 写入Flash
    uint32_t address = get_flash_page_address(PID_FLASH_SECTOR);

    // 获取双字指针（64位对齐）
    uint64_t *buffer = (uint64_t *)flash_union_buffer;
    uint32_t words_to_write = EEPROM_PAGE_LENGTH / 8; // 计算双字数

    // 解锁Flash
    HAL_FLASH_Unlock();

    // 按双字写入数据
    for (uint32_t i = 0; i < words_to_write; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 
                            address + (i * 8), 
                            buffer[i]) != HAL_OK) {
            HAL_FLASH_Lock();
            return HAL_ERROR; // 写入失败
        }
    }

    // 重新加锁Flash
    HAL_FLASH_Lock();
    return HAL_OK;
}

/**
  * @brief 从Flash读取PID参数
  * @param pid_params : 用于存储读取参数的数组指针
  * @param param_count : 要读取的参数数量（不能超过PID_PARAM_COUNT）
  * @retval HAL_StatusTypeDef 操作状态
  */
HAL_StatusTypeDef flash_get_buffer_PID(float *pid_params, uint16_t param_count) {
    // 参数检查
    if (pid_params == NULL || param_count > PID_PARAM_COUNT) {
        return HAL_ERROR; // 参数错误
    }

    // 读取Flash页到缓冲区
    flash_read_page_to_buffer(PID_FLASH_SECTOR, 0);

    // 从缓冲区获取PID参数
    for (uint16_t i = 0; i < param_count; i++) {
        pid_params[i] = flash_union_buffer[i].float_type;
    }
    
    return HAL_OK;
}





