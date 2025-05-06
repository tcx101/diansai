#include "headfile.h"
float test;
flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];  // FLASH 操作的数据缓冲区

// 在 STM32 中模拟页擦除（假设每页对应一个扇区）
void stm32_flash_erase_page(uint32_t page_num) {
    FLASH_EraseInitTypeDef eraseConfig;
    uint32_t sectorError;

    // 配置擦除参数（假设 page_num 对应扇区号）
    eraseConfig.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseConfig.Sector = page_num;
    eraseConfig.NbSectors = 1;
    eraseConfig.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&eraseConfig, &sectorError);
    HAL_FLASH_Lock();
}

uint32_t get_flash_page_address(uint32_t page_num) {
    // Sector 11 起始地址: 0x080E0000
    return 0x080E0000 + (page_num * FLASH_PAGE_SIZE);
}




///**
//  * @brief  将全局缓冲区 flash_union_buffer 的数据写入指定 Flash 页
//  * @param  page_num: 页编号（需确保在有效范围内）
//  * @retval HAL_StatusTypeDef: 操作状态（HAL_OK 成功，其他为失败）
//  * @note   - 调用前需确保目标扇区已被擦除
//  *         - 缓冲区 flash_union_buffer 必须按 8 字节对齐
//  *         - EEPROM_PAGE_LENGTH 必须为 8 的倍数
//  */

HAL_StatusTypeDef flash_write_page(uint32_t page_num) {

    // 确保全局缓冲区按 8 字节对齐（编译时断言）
    typedef char assert_aligned[(uintptr_t)flash_union_buffer % 8 == 0 ? 1 : -1];

    // 计算目标 Flash 地址
    uint32_t address = 0x080E0000 + (page_num * EEPROM_PAGE_LENGTH);

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
////-------------------------------------------------------------------------------------------------------------------
//// 函数简介      编程指定页（适配STM32 HAL库）
//// 参数说明      sector_num      保留参数（STM32中未使用，仅为兼容原接口）
//// 参数说明      page_num        页编号（范围由 FLASH_PAGE_NUM 定义，如0~11）
//// 参数说明      buf             待写入数据缓冲区（需按8字节对齐）
//// 参数说明      len             数据长度（单位：字节，需为8的倍数）
//// 返回参数      HAL_StatusTypeDef 操作状态（HAL_OK成功，其他失败）
//// 使用示例      flash_write_page(0, 5, buffer, 512); // 写入512字节到页5
//// 备注信息      - 需要预定义FLASH_PAGE_SIZE、FLASH_PAGE_NUM
////               - 全局变量flash_erase_page_flag需在外部定义
////-------------------------------------------------------------------------------------------------------------------
//HAL_StatusTypeDef flash_write_page(uint32_t sector_num, uint32_t page_num, const uint32_t *buf, uint16_t len) {
////    // 参数检查
////    if (page_num >= FLASH_PAGE_NUM || len > FLASH_PAGE_SIZE || (len % 8) != 0) {
////        return HAL_ERROR;
////    }

//    // 计算物理地址（假设页连续分布）
//    uint32_t flash_addr = FLASH_BASE + (page_num * FLASH_PAGE_SIZE);

//    // 检查是否需要擦除（需自行实现flash_check函数）
//    static uint8_t flash_erase_page_flag = 0;
//    if (flash_erase_page_flag == 0 && flash_check(page_num)) { 
//        if (HAL_FLASHEx_Erase(&(FLASH_EraseInitTypeDef){
//                .TypeErase = FLASH_TYPEERASE_SECTORS,
//                .Sector = FLASH_PAGE_TO_SECTOR(page_num), // 需定义页到扇区的映射
//                .NbSectors = 1,
//                .VoltageRange = FLASH_VOLTAGE_RANGE_3
//            }, NULL) != HAL_OK) {
//            return HAL_ERROR;
//        }
//        flash_erase_page_flag = 1;
//    }

//    // 转换为64位指针（STM32需按双字写入）
//    uint64_t *src = (uint64_t*)buf;
//    uint32_t words = len / 8;

//    // 解锁Flash
//    HAL_FLASH_Unlock();

//    // 逐双字写入
//    for (uint32_t i = 0; i < words; i++) {
//        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 
//                             flash_addr + (i * 8), 
//                             src[i]) != HAL_OK) {
//            HAL_FLASH_Lock();
//            return HAL_ERROR;
//        }
//    }

//    // 重新加锁
//    HAL_FLASH_Lock();
//    return HAL_OK;
//}

void flash_buffer_clear (void)
{
    memset(flash_union_buffer, 0xFF, EEPROM_PAGE_LENGTH);
	
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从指定 Flash 页读取数据到全局缓冲区 flash_union_buffer
// 参数说明     sector_num      保留参数（未使用，仅为兼容原接口）
// 参数说明     page_num        页编号（范围需根据实际 Flash 大小定义，如 0~11）
// 返回参数     void
// 使用示例     flash_read_page_to_buffer(0, 11); // 读取页11数据到缓冲区
// 备注信息     - 依赖宏定义: FLASH_PAGE_SIZE（页大小）、FLASH_MAX_PAGE（最大页号）
//              - 全局缓冲区需定义: extern uint8_t flash_union_buffer[FLASH_PAGE_SIZE];
//-------------------------------------------------------------------------------------------------------------------
void flash_read_page_to_buffer(uint32_t sector_num, uint32_t page_num) {
    // 参数断言检查（需用户实现或替换为HAL库的assert）
    assert_param(IS_FLASH_PAGE_NUM(page_num));  // 示例断言宏，需根据实际定义

    // 计算 Flash 物理地址（假设页起始地址从 Flash 基地址开始连续分布）
    uint32_t flash_addr = 0x080E0000 + (page_num * FLASH_PAGE_SIZE);

    // 直接内存拷贝（Flash 地址空间可读）
    memcpy(flash_union_buffer, (void*)flash_addr, FLASH_PAGE_SIZE);
}


void flash_set_buffer_PARA()  
{
	 //第一步清除缓冲区
	 flash_buffer_clear();
   //第二步把数据存到缓冲区
	flash_union_buffer[1].float_type=test;
	
	//第三步把FLASH数据擦除/把数据存到缓冲区
	stm32_flash_erase_page(FLASH_SECTOR);
	
	//第四步存到FLASH指定扇区
	flash_write_page(1);
	
}
void flash_get_buffer_PARA()  
{
	
	flash_read_page_to_buffer(11, 1);
	test=flash_union_buffer[1].float_type;
	
	
}





