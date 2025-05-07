#include "chuankou.h"
#include <string.h>
#include <stdlib.h>
point point_data;
char str[200];
//zuobiao dingdian;

// 辅助函数：安全地解析整数，避免格式错误
uint16_t safe_parse_int(char **ptr)
{
    char *end;
    long val = strtol(*ptr, &end, 10);
    
    // 如果无法解析或值无效，返回0
    if (*ptr == end || val < 0 || val > 65535) {
        *ptr = end;
        return 0;
    }
    
    *ptr = end + 1; // 跳过逗号或其他分隔符
    return (uint16_t)val;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart2 && Size > 0)
    {
		
        // 检查是否收到完整数据（以@开头，以#结尾）
        if (str[0] == '@')
        {
            // 寻找结束符#
            char *endMarker = strchr(str, '#');
            if (endMarker != NULL)
            {
                char *p = str + 1; // 跳过@符号
                
                // 解析8个点的坐标（16个值）
							
                // 外圈四个点
                point_data.A1[0] = safe_parse_int(&p);
                point_data.A1[1] = safe_parse_int(&p);
                
                point_data.B1[0] = safe_parse_int(&p);
                point_data.B1[1] = safe_parse_int(&p);
                
                point_data.C1[0] = safe_parse_int(&p);
                point_data.C1[1] = safe_parse_int(&p);
                
                point_data.D1[0] = safe_parse_int(&p);
                point_data.D1[1]= safe_parse_int(&p);
                
                // 内圈四个点
                point_data.A2[0] = safe_parse_int(&p);
                point_data.A2[1] = safe_parse_int(&p);
                
                point_data.B2[0] = safe_parse_int(&p);
                point_data.B2[1] = safe_parse_int(&p);
                
                point_data.C2[0] = safe_parse_int(&p);
                point_data.C2[1] = safe_parse_int(&p);
                
                point_data.D2[0] = safe_parse_int(&p);
                point_data.D2[1] = safe_parse_int(&p);
                
                // 解析内部矩形中心点
                point_data.mid[0] = safe_parse_int(&p);
                point_data.mid[1] = safe_parse_int(&p);
                
                // 解析绿色激光点坐标
                point_data.light[0] = safe_parse_int(&p);
                point_data.light[1] = safe_parse_int(&p);
                HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)str, sizeof(str));			
            }
        }
        else if (str[0]=='@'&&str[1]=='G')
        {
            char *endMarker = strchr(str, '#');
            if (endMarker != NULL)
            {
                // 跳过"@G,"前缀
                char *p = str + 3; // 跳过@G,
                
                // 确保数据是有效的
                if(*p != '\0' && *p != '#')
                {
                    // 解析绿色激光点坐标
                    point_data.light[0] = safe_parse_int(&p);
                    point_data.light[1] = safe_parse_int(&p);
                }
                else
                {
                    // 无效数据时，将坐标设为0
                    point_data.light[0] = 0;
                    point_data.light[1] = 0;
        }
     }
        // 重新启动DMA接收
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)str, sizeof(str));
    }
}
}
