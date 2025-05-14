#include "headfile.h"
float test;
flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];  // FLASH ���������ݻ�����
#define FLASH_BUFFER_SIZE 32  // ����ʵ���������

// �� STM32 �в�������
void stm32_flash_erase_page(uint32_t page_num) {
    FLASH_EraseInitTypeDef eraseConfig;
    uint32_t sectorError;

    // �������
    if (!IS_FLASH_PAGE_NUM(page_num)) {
        return; // ��Ч������
    }

    // ���ò�������
    eraseConfig.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseConfig.Sector = page_num;
    eraseConfig.NbSectors = 1;
    eraseConfig.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    HAL_FLASH_Unlock();
    if (HAL_FLASHEx_Erase(&eraseConfig, &sectorError) != HAL_OK) {
        // ����ʧ�ܴ���
        HAL_FLASH_Lock();
        return;
    }
    HAL_FLASH_Lock();
}

// ��ȡFlashҳ��ַ
uint32_t get_flash_page_address(uint32_t page_num) {
    uint32_t address;
    
    // ��STM32F407VGT6��������ַ����ӳ��
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
        default: address = FLASH_SECTOR_11_ADDR; break; // Ĭ��ʹ��Sector 11
    }
    
    return address;
}

/**
  * @brief  ��ȫ�ֻ����� flash_union_buffer ������д��ָ�� Flash ҳ
  * @param  page_num: ҳ��ţ���ȷ������Ч��Χ�ڣ�
  * @retval HAL_StatusTypeDef: ����״̬��HAL_OK �ɹ�������Ϊʧ�ܣ�
  * @note   - ����ǰ��ȷ��Ŀ�������ѱ�����
  *         - ������ flash_union_buffer ���밴 8 �ֽڶ���
  *         - EEPROM_PAGE_LENGTH ����Ϊ 8 �ı���
  */
HAL_StatusTypeDef flash_write_page(uint32_t page_num) {
    // �������
    if (!IS_FLASH_PAGE_NUM(page_num)) {
        return HAL_ERROR; // ��Чҳ��
    }

    // ȷ��ȫ�ֻ������� 8 �ֽڶ��루����ʱ���ԣ�
    typedef char assert_aligned[(uintptr_t)flash_union_buffer % 8 == 0 ? 1 : -1];

    // ����Ŀ�� Flash ��ַ
    uint32_t address = get_flash_page_address(page_num);

    // ��ȡ˫��ָ�루64 λ���룩
    uint64_t *buffer = (uint64_t *)flash_union_buffer;
    uint32_t words_to_write = EEPROM_PAGE_LENGTH / 8; // ����˫����

    // ���� Flash
    HAL_FLASH_Unlock();

    // ��˫��д������
    for (uint32_t i = 0; i < words_to_write; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 
                            address + (i * 8), 
                            buffer[i]) != HAL_OK) {
            HAL_FLASH_Lock(); // ʧ��ʱ���¼���
            return HAL_ERROR;
        }
    }

    // ���¼��� Flash
    HAL_FLASH_Lock();
    return HAL_OK;
}

// ��ջ�����
void flash_buffer_clear(void) {
    memset(flash_union_buffer, 0xFF, EEPROM_PAGE_LENGTH);
}

/**
  * @brief ��ָ�� Flash ҳ��ȡ���ݵ�ȫ�ֻ����� flash_union_buffer
  * @param sector_num      ����������δʹ�ã���Ϊ����ԭ�ӿڣ�
  * @param page_num        ҳ��ţ���Χ�����ʵ�� Flash ��С���壬�� 0~11��
  * @retval void
  */
void flash_read_page_to_buffer(uint32_t sector_num, uint32_t page_num) {
    // �������
    if (!IS_FLASH_PAGE_NUM(page_num)) {
        return; // ��Чҳ��
    }

    // ���� Flash �����ַ
    uint32_t flash_addr = get_flash_page_address(page_num);

    // ֱ���ڴ濽����Flash ��ַ�ռ�ɶ���
    memcpy(flash_union_buffer, (void*)flash_addr, FLASH_PAGE_SIZE);
}

/**
  * @brief ��ͨ�ò���д��Flash
  * @retval None
  */
void flash_set_buffer_PARA(void) {
    // ��һ�����������
    flash_buffer_clear();
    
    // �ڶ��������ݴ浽������
    flash_union_buffer[1].float_type = test;
    
    // ��������FLASH���ݲ���
    stm32_flash_erase_page(FLASH_SECTOR);
    
    // ���Ĳ��浽FLASHָ������
    if (flash_write_page(1) != HAL_OK) {
        // д��ʧ�ܴ���������Ҫ��
    }
}

/**
  * @brief ��Flash��ȡͨ�ò���
  * @retval None
  */
void flash_get_buffer_PARA(void) {
    flash_read_page_to_buffer(FLASH_SECTOR, 1);
    test = flash_union_buffer[1].float_type;
}

/**
  * @brief ��PID����д��Flash
  * @param pid_params : PID��������ָ��
  * @param param_count : �������������ܳ���PID_PARAM_COUNT��
  * @retval HAL_StatusTypeDef ����״̬
  */
HAL_StatusTypeDef flash_set_buffer_PID(float *pid_params, uint16_t param_count) {
    // �������
    if (pid_params == NULL || param_count > PID_PARAM_COUNT) {
        return HAL_ERROR; // ��������
    }

    // 1. ���������
    flash_buffer_clear();

    // 2. ��PID�������뻺����
    for (uint16_t i = 0; i < param_count; i++) {
        flash_union_buffer[i].float_type = pid_params[i];
    }

    // 3. ����Flash����
    stm32_flash_erase_page(PID_FLASH_SECTOR);

    // 4. ֱ��д��Flash����
    return flash_write_page(PID_FLASH_SECTOR);
}

/**
  * @brief ��Flash��ȡPID����
  * @param pid_params : ���ڴ洢��ȡ����������ָ��
  * @param param_count : Ҫ��ȡ�Ĳ������������ܳ���PID_PARAM_COUNT��
  * @retval HAL_StatusTypeDef ����״̬
  */
HAL_StatusTypeDef flash_get_buffer_PID(float *pid_params, uint16_t param_count) {
    // �������
    if (pid_params == NULL || param_count > PID_PARAM_COUNT) {
        return HAL_ERROR; // ��������
    }

    // ��ȡFlash��������������ʹ����д����ͬ�������ţ�
    flash_read_page_to_buffer(PID_FLASH_SECTOR, PID_FLASH_SECTOR);

    // �ӻ�������ȡPID����
    for (uint16_t i = 0; i < param_count; i++) {
        pid_params[i] = flash_union_buffer[i].float_type;
    }
    
    return HAL_OK;
}





