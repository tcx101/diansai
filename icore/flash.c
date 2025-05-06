#include "headfile.h"
float test;
flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];  // FLASH ���������ݻ�����

// �� STM32 ��ģ��ҳ����������ÿҳ��Ӧһ��������
void stm32_flash_erase_page(uint32_t page_num) {
    FLASH_EraseInitTypeDef eraseConfig;
    uint32_t sectorError;

    // ���ò������������� page_num ��Ӧ�����ţ�
    eraseConfig.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseConfig.Sector = page_num;
    eraseConfig.NbSectors = 1;
    eraseConfig.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&eraseConfig, &sectorError);
    HAL_FLASH_Lock();
}

uint32_t get_flash_page_address(uint32_t page_num) {
    // Sector 11 ��ʼ��ַ: 0x080E0000
    return 0x080E0000 + (page_num * FLASH_PAGE_SIZE);
}




///**
//  * @brief  ��ȫ�ֻ����� flash_union_buffer ������д��ָ�� Flash ҳ
//  * @param  page_num: ҳ��ţ���ȷ������Ч��Χ�ڣ�
//  * @retval HAL_StatusTypeDef: ����״̬��HAL_OK �ɹ�������Ϊʧ�ܣ�
//  * @note   - ����ǰ��ȷ��Ŀ�������ѱ�����
//  *         - ������ flash_union_buffer ���밴 8 �ֽڶ���
//  *         - EEPROM_PAGE_LENGTH ����Ϊ 8 �ı���
//  */

HAL_StatusTypeDef flash_write_page(uint32_t page_num) {

    // ȷ��ȫ�ֻ������� 8 �ֽڶ��루����ʱ���ԣ�
    typedef char assert_aligned[(uintptr_t)flash_union_buffer % 8 == 0 ? 1 : -1];

    // ����Ŀ�� Flash ��ַ
    uint32_t address = 0x080E0000 + (page_num * EEPROM_PAGE_LENGTH);

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
////-------------------------------------------------------------------------------------------------------------------
//// �������      ���ָ��ҳ������STM32 HAL�⣩
//// ����˵��      sector_num      ����������STM32��δʹ�ã���Ϊ����ԭ�ӿڣ�
//// ����˵��      page_num        ҳ��ţ���Χ�� FLASH_PAGE_NUM ���壬��0~11��
//// ����˵��      buf             ��д�����ݻ��������谴8�ֽڶ��룩
//// ����˵��      len             ���ݳ��ȣ���λ���ֽڣ���Ϊ8�ı�����
//// ���ز���      HAL_StatusTypeDef ����״̬��HAL_OK�ɹ�������ʧ�ܣ�
//// ʹ��ʾ��      flash_write_page(0, 5, buffer, 512); // д��512�ֽڵ�ҳ5
//// ��ע��Ϣ      - ��ҪԤ����FLASH_PAGE_SIZE��FLASH_PAGE_NUM
////               - ȫ�ֱ���flash_erase_page_flag�����ⲿ����
////-------------------------------------------------------------------------------------------------------------------
//HAL_StatusTypeDef flash_write_page(uint32_t sector_num, uint32_t page_num, const uint32_t *buf, uint16_t len) {
////    // �������
////    if (page_num >= FLASH_PAGE_NUM || len > FLASH_PAGE_SIZE || (len % 8) != 0) {
////        return HAL_ERROR;
////    }

//    // ���������ַ������ҳ�����ֲ���
//    uint32_t flash_addr = FLASH_BASE + (page_num * FLASH_PAGE_SIZE);

//    // ����Ƿ���Ҫ������������ʵ��flash_check������
//    static uint8_t flash_erase_page_flag = 0;
//    if (flash_erase_page_flag == 0 && flash_check(page_num)) { 
//        if (HAL_FLASHEx_Erase(&(FLASH_EraseInitTypeDef){
//                .TypeErase = FLASH_TYPEERASE_SECTORS,
//                .Sector = FLASH_PAGE_TO_SECTOR(page_num), // �趨��ҳ��������ӳ��
//                .NbSectors = 1,
//                .VoltageRange = FLASH_VOLTAGE_RANGE_3
//            }, NULL) != HAL_OK) {
//            return HAL_ERROR;
//        }
//        flash_erase_page_flag = 1;
//    }

//    // ת��Ϊ64λָ�루STM32�谴˫��д�룩
//    uint64_t *src = (uint64_t*)buf;
//    uint32_t words = len / 8;

//    // ����Flash
//    HAL_FLASH_Unlock();

//    // ��˫��д��
//    for (uint32_t i = 0; i < words; i++) {
//        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 
//                             flash_addr + (i * 8), 
//                             src[i]) != HAL_OK) {
//            HAL_FLASH_Lock();
//            return HAL_ERROR;
//        }
//    }

//    // ���¼���
//    HAL_FLASH_Lock();
//    return HAL_OK;
//}

void flash_buffer_clear (void)
{
    memset(flash_union_buffer, 0xFF, EEPROM_PAGE_LENGTH);
	
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ָ�� Flash ҳ��ȡ���ݵ�ȫ�ֻ����� flash_union_buffer
// ����˵��     sector_num      ����������δʹ�ã���Ϊ����ԭ�ӿڣ�
// ����˵��     page_num        ҳ��ţ���Χ�����ʵ�� Flash ��С���壬�� 0~11��
// ���ز���     void
// ʹ��ʾ��     flash_read_page_to_buffer(0, 11); // ��ȡҳ11���ݵ�������
// ��ע��Ϣ     - �����궨��: FLASH_PAGE_SIZE��ҳ��С����FLASH_MAX_PAGE�����ҳ�ţ�
//              - ȫ�ֻ������趨��: extern uint8_t flash_union_buffer[FLASH_PAGE_SIZE];
//-------------------------------------------------------------------------------------------------------------------
void flash_read_page_to_buffer(uint32_t sector_num, uint32_t page_num) {
    // �������Լ�飨���û�ʵ�ֻ��滻ΪHAL���assert��
    assert_param(IS_FLASH_PAGE_NUM(page_num));  // ʾ�����Ժ꣬�����ʵ�ʶ���

    // ���� Flash �����ַ������ҳ��ʼ��ַ�� Flash ����ַ��ʼ�����ֲ���
    uint32_t flash_addr = 0x080E0000 + (page_num * FLASH_PAGE_SIZE);

    // ֱ���ڴ濽����Flash ��ַ�ռ�ɶ���
    memcpy(flash_union_buffer, (void*)flash_addr, FLASH_PAGE_SIZE);
}


void flash_set_buffer_PARA()  
{
	 //��һ�����������
	 flash_buffer_clear();
   //�ڶ��������ݴ浽������
	flash_union_buffer[1].float_type=test;
	
	//��������FLASH���ݲ���/�����ݴ浽������
	stm32_flash_erase_page(FLASH_SECTOR);
	
	//���Ĳ��浽FLASHָ������
	flash_write_page(1);
	
}
void flash_get_buffer_PARA()  
{
	
	flash_read_page_to_buffer(11, 1);
	test=flash_union_buffer[1].float_type;
	
	
}





