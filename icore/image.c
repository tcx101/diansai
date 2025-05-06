#include "image.h"

/*横线*/
int AB_line[MAX_X];
int CD_line[MAX_X];

/*竖线*/
int BC_line[MAX_Y];
int AD_line[MAX_Y];

/**?
 * @brief X轴索引补线函数（实线，支持任意斜率）
 * @param X1,Y1   起点坐标
 * @param X2,Y2   终点坐标
 * @param x_line  目标数组，x_line[X]存储对应Y值
 */
void Fill_XLine(int X1, int Y1, int X2, int Y2, int* x_line) {
    /*----- 1. 坐标安全限制 -----*/
    X1 = (X1 < 0) ? 0 : (X1 >= MAX_X) ? MAX_X-1 : X1;
    X2 = (X2 < 0) ? 0 : (X2 >= MAX_X) ? MAX_X-1 : X2;
    Y1 = (Y1 < 0) ? 0 : Y1;
    Y2 = (Y2 < 0) ? 0 : Y2;

    /*----- 2. 确保X1 <= X2，并记录坐标交换标志 -----*/
    int swap_flag = 0;
    if (X1 > X2) {
        int tmp = X1; X1 = X2; X2 = tmp;
        tmp = Y1; Y1 = Y2; Y2 = tmp;
        swap_flag = 1;
    }

    /*----- 3. 垂直线处理（X1 == X2）-----*/
    if (X1 == X2) {
        int start_y = (Y1 < Y2) ? Y1 : Y2;
        int end_y = (Y1 < Y2) ? Y2 : Y1;
        for (int y = start_y; y <= end_y; y++) {
            x_line[X1] = y; // 存储当前X对应的Y值
        }
        return;
    }

    /*----- 4. Bresenham算法核心逻辑 -----*/
    int dx = X2 - X1;         // 已保证dx >= 0
    int dy = abs(Y2 - Y1);
    int sy = (Y1 < Y2) ? 1 : -1; // Y方向步进
    int err = dx - dy;        // 误差初始化

    int current_x = X1;
    int current_y = Y1;

    while (current_x <= X2) {
        // 存储当前X对应的Y值（考虑坐标交换）
        x_line[current_x] = swap_flag ? (current_y != Y1 ? Y2 - (current_y - Y1) : Y1) : current_y;

        #if DEBUG_MODE
        printf("X=%d: Y=%d (err=%d)\n", current_x, current_y, err);
        #endif

        if (current_x == X2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { // 垂直方向误差累积
            err -= dy;
            current_x++; // 优先水平移动
        }
        if (e2 < dx) {  // 水平方向误差补偿
            err += dx;
            current_y += sy;
        }
    }
}


/**
 * @brief Y轴索引补线函数（实线，支持任意斜率）
 * @param X1,Y1   起点坐标
 * @param X2,Y2   终点坐标
 * @param y_line  目标数组，y_line[Y]存储对应X值
 */
void Fill_YLine(int X1, int Y1, int X2, int Y2, int* y_line) {
    // 坐标安全钳位
    Y1 = (Y1 < 0) ? 0 : (Y1 >= MAX_Y) ? MAX_Y-1 : Y1;
    Y2 = (Y2 < 0) ? 0 : (Y2 >= MAX_Y) ? MAX_Y-1 : Y2;
    X1 = (X1 < 0) ? 0 : X1;
    X2 = (X2 < 0) ? 0 : X2;

    // 确保 Y1 <= Y2，并交换坐标时同步交换 X 值
    int swap_flag = 0;
    if (Y1 > Y2) {
        int tmp = Y1; Y1 = Y2; Y2 = tmp;
        tmp = X1; X1 = X2; X2 = tmp;
        swap_flag = 1; // 标记坐标已交换
    }

    // 处理垂直线（Y1 == Y2）
    if (Y1 == Y2) {
        int start_x = (X1 < X2) ? X1 : X2;
        int end_x = (X1 < X2) ? X2 : X1;
        for (int x = start_x; x <= end_x; x++) {
            // 若Y轴索引允许存储多个X值，需调整数据结构
            y_line[Y1] = x; // 可能需要二维数组存储多值
        }
        return;
    }

    // Bresenham 算法修正
    int dx = abs(X2 - X1);
    int dy = Y2 - Y1; // 由于已交换，dy >= 0
    int sx = (X1 < X2) ? 1 : -1; // X 步进方向
    int err = dx - dy;
    int current_y = Y1;
    int current_x = X1;

    while (current_y <= Y2) {
        // 存储当前Y对应的X值（若坐标已交换需还原）
        y_line[current_y] = swap_flag ? (current_x != X1 ? X2 - (current_x - X1) : X1) : current_x;
        
        if (current_y == Y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { // 横向移动
            err -= dy;
            current_x += sx;
        }
        if (e2 < dx) {  // 纵向移动
            err += dx;
            current_y++;
        }
    }
}