#include "image.h"

/*����*/
int AB_line[MAX_X];
int CD_line[MAX_X];

/*����*/
int BC_line[MAX_Y];
int AD_line[MAX_Y];

/**?
 * @brief X���������ߺ�����ʵ�ߣ�֧������б�ʣ�
 * @param X1,Y1   �������
 * @param X2,Y2   �յ�����
 * @param x_line  Ŀ�����飬x_line[X]�洢��ӦYֵ
 */
void Fill_XLine(int X1, int Y1, int X2, int Y2, int* x_line) {
    /*----- 1. ���갲ȫ���� -----*/
    X1 = (X1 < 0) ? 0 : (X1 >= MAX_X) ? MAX_X-1 : X1;
    X2 = (X2 < 0) ? 0 : (X2 >= MAX_X) ? MAX_X-1 : X2;
    Y1 = (Y1 < 0) ? 0 : Y1;
    Y2 = (Y2 < 0) ? 0 : Y2;

    /*----- 2. ȷ��X1 <= X2������¼���꽻����־ -----*/
    int swap_flag = 0;
    if (X1 > X2) {
        int tmp = X1; X1 = X2; X2 = tmp;
        tmp = Y1; Y1 = Y2; Y2 = tmp;
        swap_flag = 1;
    }

    /*----- 3. ��ֱ�ߴ���X1 == X2��-----*/
    if (X1 == X2) {
        int start_y = (Y1 < Y2) ? Y1 : Y2;
        int end_y = (Y1 < Y2) ? Y2 : Y1;
        for (int y = start_y; y <= end_y; y++) {
            x_line[X1] = y; // �洢��ǰX��Ӧ��Yֵ
        }
        return;
    }

    /*----- 4. Bresenham�㷨�����߼� -----*/
    int dx = X2 - X1;         // �ѱ�֤dx >= 0
    int dy = abs(Y2 - Y1);
    int sy = (Y1 < Y2) ? 1 : -1; // Y���򲽽�
    int err = dx - dy;        // ����ʼ��

    int current_x = X1;
    int current_y = Y1;

    while (current_x <= X2) {
        // �洢��ǰX��Ӧ��Yֵ���������꽻����
        x_line[current_x] = swap_flag ? (current_y != Y1 ? Y2 - (current_y - Y1) : Y1) : current_y;

        #if DEBUG_MODE
        printf("X=%d: Y=%d (err=%d)\n", current_x, current_y, err);
        #endif

        if (current_x == X2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { // ��ֱ��������ۻ�
            err -= dy;
            current_x++; // ����ˮƽ�ƶ�
        }
        if (e2 < dx) {  // ˮƽ��������
            err += dx;
            current_y += sy;
        }
    }
}


/**
 * @brief Y���������ߺ�����ʵ�ߣ�֧������б�ʣ�
 * @param X1,Y1   �������
 * @param X2,Y2   �յ�����
 * @param y_line  Ŀ�����飬y_line[Y]�洢��ӦXֵ
 */
void Fill_YLine(int X1, int Y1, int X2, int Y2, int* y_line) {
    // ���갲ȫǯλ
    Y1 = (Y1 < 0) ? 0 : (Y1 >= MAX_Y) ? MAX_Y-1 : Y1;
    Y2 = (Y2 < 0) ? 0 : (Y2 >= MAX_Y) ? MAX_Y-1 : Y2;
    X1 = (X1 < 0) ? 0 : X1;
    X2 = (X2 < 0) ? 0 : X2;

    // ȷ�� Y1 <= Y2������������ʱͬ������ X ֵ
    int swap_flag = 0;
    if (Y1 > Y2) {
        int tmp = Y1; Y1 = Y2; Y2 = tmp;
        tmp = X1; X1 = X2; X2 = tmp;
        swap_flag = 1; // ��������ѽ���
    }

    // ����ֱ�ߣ�Y1 == Y2��
    if (Y1 == Y2) {
        int start_x = (X1 < X2) ? X1 : X2;
        int end_x = (X1 < X2) ? X2 : X1;
        for (int x = start_x; x <= end_x; x++) {
            // ��Y����������洢���Xֵ����������ݽṹ
            y_line[Y1] = x; // ������Ҫ��ά����洢��ֵ
        }
        return;
    }

    // Bresenham �㷨����
    int dx = abs(X2 - X1);
    int dy = Y2 - Y1; // �����ѽ�����dy >= 0
    int sx = (X1 < X2) ? 1 : -1; // X ��������
    int err = dx - dy;
    int current_y = Y1;
    int current_x = X1;

    while (current_y <= Y2) {
        // �洢��ǰY��Ӧ��Xֵ���������ѽ����軹ԭ��
        y_line[current_y] = swap_flag ? (current_x != X1 ? X2 - (current_x - X1) : X1) : current_x;
        
        if (current_y == Y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { // �����ƶ�
            err -= dy;
            current_x += sx;
        }
        if (e2 < dx) {  // �����ƶ�
            err += dx;
            current_y++;
        }
    }
}
uint16_t cal_mid(uint16_t point1,uint16_t point2)
{
    uint16_t mid_point;
    mid_point=(point1+point2)/2;
    return mid_point;
}

int scale_480_to_128(int input) {
    // �߽籣��
    if(input <= 0) return 0;
    if(input >= 480) return 128;
    
    // ��ȷ���㣨�������룩
    return (int)(input * 0.2666666667f + 0.5f); // 128/480 �� 0.2666667
}

int scale_320_to_64(int input) {
    // �߽籣��
    if(input <= 0) return 0;
    if(input >= 320) return 64;
    
    // ��ȷ���㣨�������룩
    return (int)(input * 0.2f + 0.5f); // 64/320 = 0.2
}


