#include "oled.h"

// 6x8字体库数组，每个字符6字节
const uint8_t F6x8[][6] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00 },   // $
    { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14, 0x00 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00, 0x00 },   // ,
    { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00 },   // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03, 0x00 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00 },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 },   // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E, 0x00 },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00 },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00 },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00 },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00 },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00 },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31, 0x00 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00 },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00 },   // [
    { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 },   // "\"
    { 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00 },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E, 0x00 },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78, 0x00 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00 },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00 },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00 },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00 },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00 },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00 },   // z
    { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 },   // {
    { 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00 },   // |
    { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 },   // }
    { 0x10, 0x08, 0x08, 0x10, 0x08, 0x00 },   // ~
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }    // DEL
};

// 在显示函数之前添加数学函数
uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--) result *= m;
    return result;
}

// 显示缓冲区
uint8_t OLED_GRAM[OLED_PAGE_NUM][OLED_WIDTH];

// 写命令
static HAL_StatusTypeDef OLED_Write_Command(uint8_t cmd)
{
    uint8_t data[2] = {0x00, cmd};  // 0x00表示写命令
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, data, 2, 100);
    return status;
}

// 写数据
static HAL_StatusTypeDef OLED_Write_Data(uint8_t data)
{
    uint8_t temp[2] = {0x40, data};  // 0x40表示写数据
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, temp, 2, 100);
    return status;
}

// OLED初始化
HAL_StatusTypeDef OLED_Init(void)
{
    HAL_StatusTypeDef status;
    
    HAL_Delay(100);  // 等待OLED上电完成
    
    // 初始化命令序列
    if((status = OLED_Write_Command(0xAE)) != HAL_OK) return status; // 关闭显示
    if((status = OLED_Write_Command(0x20)) != HAL_OK) return status; // 设置内存寻址模式
    if((status = OLED_Write_Command(0x10)) != HAL_OK) return status; // 00：水平寻址模式；01：垂直寻址模式；10：页寻址模式
    if((status = OLED_Write_Command(0xB0)) != HAL_OK) return status; // 设置页起始地址（0~7）
    if((status = OLED_Write_Command(0xC8)) != HAL_OK) return status; // 设置COM扫描方向
    if((status = OLED_Write_Command(0x00)) != HAL_OK) return status; // 设置低列起始地址
    if((status = OLED_Write_Command(0x10)) != HAL_OK) return status; // 设置高列起始地址
    if((status = OLED_Write_Command(0x40)) != HAL_OK) return status; // 设置显示起始行
    if((status = OLED_Write_Command(0x81)) != HAL_OK) return status; // 设置对比度控制
    if((status = OLED_Write_Command(0xFF)) != HAL_OK) return status; // 对比度值
    if((status = OLED_Write_Command(0xA1)) != HAL_OK) return status; // 设置段重映射
    if((status = OLED_Write_Command(0xA6)) != HAL_OK) return status; // 正常显示
    if((status = OLED_Write_Command(0xA8)) != HAL_OK) return status; // 设置多路复用率
    if((status = OLED_Write_Command(0x3F)) != HAL_OK) return status; // 1/64 duty
    if((status = OLED_Write_Command(0xD3)) != HAL_OK) return status; // 设置显示偏移
    if((status = OLED_Write_Command(0x00)) != HAL_OK) return status; // 无偏移
    if((status = OLED_Write_Command(0xD5)) != HAL_OK) return status; // 设置显示时钟分频/振荡器频率
    if((status = OLED_Write_Command(0x80)) != HAL_OK) return status; // 设置分频比和振荡器频率
    if((status = OLED_Write_Command(0xD9)) != HAL_OK) return status; // 设置预充电周期
    if((status = OLED_Write_Command(0xF1)) != HAL_OK) return status; // 设置预充电周期时间
    if((status = OLED_Write_Command(0xDA)) != HAL_OK) return status; // 设置COM引脚硬件配置
    if((status = OLED_Write_Command(0x12)) != HAL_OK) return status; // 设置COM引脚配置
    if((status = OLED_Write_Command(0xDB)) != HAL_OK) return status; // 设置VCOMH取消选择级别
    if((status = OLED_Write_Command(0x40)) != HAL_OK) return status; // 设置VCOMH取消选择级别
    if((status = OLED_Write_Command(0xAF)) != HAL_OK) return status; // 开启显示
    
    OLED_Clear();  // 清屏
    OLED_Set_Pos(0, 0);  // 设置光标位置到原点
    
    return HAL_OK;
}

// 清屏
void OLED_Clear(void)
{
    uint8_t i, j;
    for(i = 0; i < OLED_PAGE_NUM; i++) {
        for(j = 0; j < OLED_WIDTH; j++) {
            OLED_GRAM[i][j] = 0x00;
        }
    }
    OLED_Refresh();
}

// 开启显示
void OLED_Display_On(void)
{
    OLED_Write_Command(0xAF);
}

// 关闭显示
void OLED_Display_Off(void)
{
    OLED_Write_Command(0xAE);
}

// 设置光标位置
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    OLED_Write_Command(0xB0 + y);
    OLED_Write_Command(((x & 0xF0) >> 4) | 0x10);
    OLED_Write_Command(x & 0x0F);
}

// 显示一个字符
void OLED_ShowChar(uint8_t x, uint8_t y, char chr)
{
    uint8_t c = chr - ' ';  // 得到偏移后的值
    uint8_t i = 0;
    
    if(x > OLED_WIDTH - 6 || y > OLED_PAGE_NUM - 1) return;
    
    for(i = 0; i < 6; i++) {
        OLED_GRAM[y][x + i] = F6x8[c][i];
    }
}

// 显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, const char *str)
{
    while(*str != '\0') {
        OLED_ShowChar(x, y, *str);
        x += 6;
        if(x > OLED_WIDTH - 6) {
            x = 0;
            y++;
            if(y > OLED_PAGE_NUM - 1) {
                y = 0;
            }
        }
        str++;
    }
}

// 刷新显示
void OLED_Refresh(void)
{
    uint8_t i, j;
    for(i = 0; i < OLED_PAGE_NUM; i++) {
        OLED_Set_Pos(0, i);
        for(j = 0; j < OLED_WIDTH; j++) {
            OLED_Write_Data(OLED_GRAM[i][j]);
        }
    }
}

// 显示数字
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for(t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1)) {
            if(temp == 0) {
                OLED_ShowChar(x + t * 6, y, ' ');
                continue;
            } else {
                enshow = 1;
            }
        }
        OLED_ShowChar(x + t * 6, y, temp + '0');
    }
}

// 显示16进制数
void OLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len)
{
    uint8_t t, temp;
    for(t = 0; t < len; t++) {
        temp = (num >> (4 * (len - t - 1))) & 0x0F;
        if(temp < 10) {
            OLED_ShowChar(x + t * 6, y, temp + '0');
        } else {
            OLED_ShowChar(x + t * 6, y, temp - 10 + 'A');
        }
    }
}

// 显示时间
void OLED_ShowTime(uint8_t x, uint8_t y, uint8_t hour, uint8_t min, uint8_t sec)
{
    OLED_ShowNum(x, y, hour, 2);
    OLED_ShowChar(x + 12, y, ':');
    OLED_ShowNum(x + 18, y, min, 2);
    OLED_ShowChar(x + 30, y, ':');
    OLED_ShowNum(x + 36, y, sec, 2);
}

// 显示CPU使用率
void OLED_ShowCPU(uint8_t x, uint8_t y, uint8_t usage)
{
    OLED_ShowString(x, y, "CPU:");
    OLED_ShowNum(x + 24, y, usage, 3);
    OLED_ShowChar(x + 42, y, '%');
}

// 显示按键状态
void OLED_ShowKeys(uint8_t x, uint8_t y, uint8_t keys)
{
    OLED_ShowString(x, y, "KEY:");
    for(uint8_t i = 0; i < 3; i++) {
        if(keys & (1 << i)) {
            OLED_ShowChar(x + 24 + i * 12, y, '1');
        } else {
            OLED_ShowChar(x + 24 + i * 12, y, '0');
        }
    }
}

// 显示LED状态
void OLED_ShowLEDs(uint8_t x, uint8_t y, uint8_t leds)
{
    OLED_ShowString(x, y, "LED:");
    OLED_ShowChar(x + 24, y, (leds & 0x01) ? 'R' : '-');
    OLED_ShowChar(x + 36, y, (leds & 0x02) ? 'G' : '-');
    OLED_ShowChar(x + 48, y, (leds & 0x04) ? 'B' : '-');
}

// 显示通信状态
void OLED_ShowComm(uint8_t x, uint8_t y, uint8_t status)
{
    OLED_ShowString(x, y, "COMM:");
    switch(status) {
        case 0:
            OLED_ShowString(x + 30, y, "OK   ");
            break;
        case 1:
            OLED_ShowString(x + 30, y, "ERROR");
            break;
        case 2:
            OLED_ShowString(x + 30, y, "WAIT ");
            break;
        default:
            OLED_ShowString(x + 30, y, "----");
    }
}

// 显示错误计数
void OLED_ShowError(uint8_t x, uint8_t y, uint16_t count)
{
    OLED_ShowString(x, y, "ERR:");
    OLED_ShowNum(x + 24, y, count, 5);
}

// 更新所有显示内容
void OLED_UpdateDisplay(void)
{
    static uint32_t last_update = 0;
    uint32_t current_time = HAL_GetTick();
    
    // 每100ms更新一次显示
    if(current_time - last_update >= 100) {
        last_update = current_time;
        
        // 获取系统时间（需要实现）
        RTC_TimeTypeDef time;
        HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
        
        // 第1行：时间和CPU使用率
        OLED_ShowTime(0, 0, time.Hours, time.Minutes, time.Seconds);
        OLED_ShowCPU(72, 0, Get_CPU_Usage()); // 需要实现CPU使用率获取函数
        
        // 第2行：按键状态
        OLED_ShowKeys(0, 2, Get_Key_Status()); // 需要实现按键状态获取函数
        
        // 第3行：LED状态
        OLED_ShowLEDs(0, 3, Get_LED_Status()); // 需要实现LED状态获取函数
        
        // 第4行：通信状态
        OLED_ShowComm(0, 4, Get_Comm_Status()); // 需要实现通信状态获取函数
        
        // 第5行：错误计数
        OLED_ShowError(0, 5, Get_Error_Count()); // 需要实现错误计数获取函数
        
        // 第6行：公司信息
        OLED_ShowString(0, 6, "Company Info");
        
        // 刷新显示
        OLED_Refresh();
    }
} 

