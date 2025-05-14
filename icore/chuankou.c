#include "chuankou.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

point point_data;
char str[200];
//zuobiao dingdian;

// 添加调试状态变量
uint8_t last_receive_mode = 0;  // 0:未知, 1:矩形数据, 2:绿色激光点数据

// 添加初始化函数
void chuankou_init(void)
{
    // 清空接收缓冲区
    memset(str, 0, sizeof(str));
    
    // 初始化坐标数据为0
    memset(&point_data, 0, sizeof(point_data));
    last_receive_mode = RECEIVE_MODE_UNKNOWN; // 初始化模式
    
    // 启动DMA接收
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)str, sizeof(str));
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);  // 禁用半传输中断
}

// 辅助函数：安全地解析整数，避免格式错误
uint16_t safe_parse_int(char **ptr)
{
    // 跳过非数字字符
    while(**ptr != '\0' && (**ptr < '0' || **ptr > '9') && **ptr != '-' /* 允许负号，尽管坐标通常为正 */) 
        (*ptr)++;
    
    char *end;
    long val = strtol(*ptr, &end, 10);
    
    // 如果无法解析或值无效（假设坐标非负且在uint16_t范围内）
    if (*ptr == end || val < 0 || val > 65535) { // 对于坐标，通常 val < 0 也是无效的
        if (*end == ',' || *end == '#') // 只处理预期的分隔符
            *ptr = end + 1; // 跳过分隔符
        else
            *ptr = end;
        return 0; // 或特定的错误码
    }
    
    // 跳过数字和可能的分隔符
    *ptr = end;
    if (**ptr == ',' || **ptr == '#')
        (*ptr)++;
    
    return (uint16_t)val;
}

// 检查字符串中是否包含特定的消息头
uint8_t contains_header(const char* buffer, const char* header) {
    return strstr(buffer, header) != NULL;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart2 && Size > 0)
    {
        // 检查是否是K230发送的特定模式切换确认（如果K230固件会发送这个）
        // 这个消息不由 python 脚本直接发送，但可能作为K230内部逻辑的一部分
        if (strstr(str, "!MODE_CHANGED_TO_GREEN_LASER!") != NULL) {
            // 记录当前模式已经切换为绿色激光模式
            // 注意：这个消息可能与@GD/@RO不同步，依赖于K230的行为
            // last_receive_mode = RECEIVE_MODE_GREEN_LASER; // 根据原代码逻辑保留
            
            // 发送确认消息回K230
            // HAL_UART_Transmit(&huart2, (uint8_t *)"OK_MODE_CHANGED\r\n", strlen("OK_MODE_CHANGED\r\n"), 100);
        }
        // 检查是否为矩形数据 (格式: @RO...)
        else if (strncmp(str, "@RO", 3) == 0) {
            char *data_start = str + 3; // 跳过 "@RO"
            char *end_marker = strchr(data_start, '#');

            if (end_marker != NULL) {
                char *p = data_start;
                
                // 解析外圈四个顶点 (A1, B1, C1, D1)
               // point_data.A1[0] = safe_parse_int(&p); point_data.A1[1] = safe_parse_int(&p);
							  point_data.C1[0] = safe_parse_int(&p); point_data.C1[1] = safe_parse_int(&p);
                //point_data.B1[0] = safe_parse_int(&p); point_data.B1[1] = safe_parse_int(&p);
						  	point_data.B1[0] = safe_parse_int(&p); point_data.B1[1] = safe_parse_int(&p);
                ///point_data.C1[0] = safe_parse_int(&p); point_data.C1[1] = safe_parse_int(&p);
							  point_data.A1[0] = safe_parse_int(&p); point_data.A1[1] = safe_parse_int(&p);
                //point_data.D1[0] = safe_parse_int(&p); point_data.D1[1] = safe_parse_int(&p);
							  point_data.D1[0] = safe_parse_int(&p); point_data.D1[1] = safe_parse_int(&p);
                
                // 解析内圈四个顶点 (A2, B2, C2, D2)
                //point_data.A2[0] = safe_parse_int(&p); point_data.A2[1] = safe_parse_int(&p);
							  point_data.D2[0] = safe_parse_int(&p); point_data.D2[1] = safe_parse_int(&p);
                //point_data.B2[0] = safe_parse_int(&p); point_data.B2[1] = safe_parse_int(&p);
						  	point_data.C2[0] = safe_parse_int(&p); point_data.C2[1] = safe_parse_int(&p);
                //point_data.C2[0] = safe_parse_int(&p); point_data.C2[1] = safe_parse_int(&p);
						  	point_data.B2[0] = safe_parse_int(&p); point_data.B2[1] = safe_parse_int(&p);
                //point_data.D2[0] = safe_parse_int(&p); point_data.D2[1] = safe_parse_int(&p);
                point_data.A2[0] = safe_parse_int(&p); point_data.A2[1] = safe_parse_int(&p);
                // 解析内部矩形中心点 (mid)
                point_data.mid[0] = safe_parse_int(&p);
                point_data.mid[1] = safe_parse_int(&p);
                
                // 清除之前的激光点数据，因为此包仅含矩形数据
                point_data.light[0] = 0;
                point_data.light[1] = 0;
                control_mid();
                last_receive_mode = RECEIVE_MODE_RECTANGLE;
                
                // 可选: 发送接收确认
                // HAL_UART_Transmit(&huart2, (uint8_t *)"RCV_RECT_OK\r\n", strlen("RCV_RECT_OK\r\n"), 100);
            }
        }
        // 检查是否为绿色激光点数据 (格式: @GD...)
        else if (strncmp(str, "@GD", 3) == 0) {
            char *data_start = str + 3; // 跳过 "@GD"
            char *end_marker = strchr(data_start, '#');

            if (end_marker != NULL) {
                char *p = data_start;
                
                // 解析绿色激光点坐标 (light)
                point_data.light[0] = safe_parse_int(&p);
                point_data.light[1] = safe_parse_int(&p);
                
                // 矩形数据 (A1-D2, mid) 保持不变，因为它们是上一次@RO包设置的
                
                last_receive_mode = RECEIVE_MODE_GREEN_LASER;
                
                // 如果在激光点模式下收到有效的激光点坐标，发送确认 (根据原代码逻辑保留)
                if (point_data.light[0] > 0 || point_data.light[1] > 0) { // 简单有效性检查
                    char confirm_msg[50];
                    sprintf(confirm_msg, "RCV_LASER:%d,%d\r\n", point_data.light[0], point_data.light[1]);
                    HAL_UART_Transmit(&huart2, (uint8_t *)confirm_msg, strlen(confirm_msg), 100);
                }
            }
        }
        
        // 重置缓冲区并重新启动DMA接收
        memset(str, 0, sizeof(str)); // 清理整个缓冲区
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)str, sizeof(str));
        __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT); // 重新禁用半传输中断
    }
}

// 获取最后接收的数据类型
uint8_t get_last_receive_mode(void)
{
    return last_receive_mode;
}

