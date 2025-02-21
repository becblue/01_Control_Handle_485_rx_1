# STM32F103 Modbus RTU 从机控制器

## 目录



## 1. 项目概述
本项目是一个基于STM32F103C8T6的Modbus RTU通讯系统的B站开发项目。该系统网络拓扑采用主从架构，A为主站，B为从站。A站可主动发起通信；B站只作应答
B站通过RS485接收A站发送的KEY1、KEY2、KEY3按键状态保存至寄存器，同时解析寄存器数据，用来驱动LED1、LED2、LED3的亮灭。并发出应答，应答内包括B站LED1、LED2、LED3以及KEY1,KEY2状态，打包后发送至A站。
同时B站需要将LED1、LED2、LED3、KEY1、KEY2状态显示在OLED显示屏上。

## 2. 硬件配置

### 2.1 OLED显示屏 (I2C)
#### 基本配置
- 参考文献：OLED显示屏规格书，文档路径：.\STM32\ex07_Control_Handle_485_4L_rx\Doc\野火0.96寸_IIC_OLED模块规格手册_20240419 
- 通信接口：I2C1
- 引脚配置：
  - SCL: PB6
  - SDA: PB7
- 分辨率：128x64
- 地址：0x78
- 显示控制器：SSD1306

#### 字库配置
- ASCII字库：6x8点阵
- 字符范围：ASCII 0x20-0x7E
- 字体数据结构：
  ```c
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
  ```
- 字体特点：
  - 每个字符6x8像素
  - 每字节代表一列点阵
  - 1表示点亮，0表示熄灭
  - 支持95个可打印字符
- 显示方式：
  - 垂直扫描
  - 逐列显示
  - MSB在上，LSB在下
- 字体效果：
  - 清晰可辨
  - 适合小尺寸显示
  - 支持基本符号

#### 显示布局
- 第1行（0-7像素）：时间和CPU使用率
- 第2行（16-23像素）：按键状态
- 第3行（24-31像素）：LED状态
- 第4行（32-39像素）：通信状态
- 第5行（40-47像素）：错误计数
- 第6行（48-55像素）：公司信息

#### 显示特性
- 支持双缓冲
- 脏区域刷新
- 动画效果
- 局部更新

### 2.2 RS485接口 (UART3)
#### 基本配置
- 参考文献：OLED显示屏规格书，文档路径：.\STM32\ex07_Control_Handle_485_4L_rx\Doc\野火小智TTL转RS485模块规格手册_20240805.pdf
- 通信接口：USART3
- 引脚配置：
  - TX: PB10
  - RX: PB11
  - DE/RE: PB8 (方向控制，高电平发送/低电平接收)
- 芯片型号：SIT3088E

#### 电气特性
- 工作电压：3.3V
- 差分电压：>2V
- 接线说明：
  - A+: 485差分信号正端
  - B-: 485差分信号负端
  - GND: 信号地
- 终端电阻：
  - 120Ω (在总线两端)
- 通信距离：最大1200米

#### 通信参数
- 波特率：9600
- 数据位：8
- 停止位：1
- 校验位：无
- 通信模式：半双工
- 总线特性：
  - 支持多点通信
  - 抗干扰能力强
  - 传输距离远

### 2.3 调试串口 (UART1)
- 通信接口：USART1
- 引脚配置：
  - TX: PA9
  - RX: PA10
- 通信参数：同RS485

### 2.4 LED指示灯
- LED_R: PA1 (输出模式，推挽输出，低电平点亮)
- LED_G: PA2 (输出模式，推挽输出，低电平点亮)
- LED_B: PA3 (输出模式，推挽输出，低电平点亮)

### 2.5 GPIO配置
- 按键输入：
  - KEY1: PA0 (输入模式，下拉)
  - KEY2: PC13 (输入模式，下拉)
- 状态指示：
  - KEY_STATUS: PA6 (输出模式，推挽输出)
  功能：当任意按键状态发生变化时，该引脚输出一个高电平脉冲,时长20ms



## 3. 软件架构

### 3.1 代码实现过程
#### 系统初始化
- 在main.c中进行系统时钟配置和硬件初始化(GPIO、USART等)
- 调用bsp_init()函数初始化板级支持包,设置必要外设

#### MODBUS协议初始化 
- 在modbus.c中初始化MODBUS协议相关参数(从站地址、波特率等)
- 设置接收缓冲区和发送缓冲区用于数据收发

#### OLED初始化 
- 在oled.c中初始化OLED显示屏,配置为I2C主机模式
- 使用6x8点阵ASCII字库显示字符



#### 主循环逻辑
- main.c主循环中持续检查接收缓冲区是否有数据到达
- 接收到数据时调用modbus_process()函数进行数据解析处理

#### 数据解析
- modbus.c中modbus_process()函数负责解析接收到的MODBUS帧
- 根据MODBUS协议格式提取功能码、数据地址和数据长度等信息
- 根据功能码执行相应操作(读写寄存器等)

#### 响应生成
- 处理完请求后生成相应的MODBUS响应帧
- 将响应数据填充到发送缓冲区并通过USART发送

#### 中断处理
- stm32f4xx_it.c中处理USART接收中断,确保数据及时接收
- 中断服务程序中读取接收数据并存入接收缓冲区

### 3.2 逻辑流程
#### 初始化阶段
- 系统启动时进行硬件和协议初始化,确保外设正常工作

#### 数据接收
- 主循环中不断检查USART接收缓冲区,等待MODBUS请求到来

#### 请求处理
- 接收到数据后解析MODBUS帧,识别请求功能码
- 根据功能码执行相应操作,访问寄存器或执行其他命令

#### 响应发送
- 生成响应帧,将结果填充到发送缓冲区
- 通过USART将响应数据发送回主机

#### 循环执行
- 主循环持续进行,等待下一个请求到来

总结
这个MODBUS RTU案例的代码实现过程和逻辑主要围绕着系统的初始化、数据的接收与解析、响应的生成与发送等核心功能展开。通过合理的模块划分和清晰的逻辑流程，确保了系统的稳定性和可靠性。

### 4.3 关键技术实现
#### 1. OLED显示管理
- 采用I2C通信方式,配置为主机模式
- 使用6x8点阵ASCII字库显示字符
- 显示内容包括:
  - LED1-3状态显示
  - KEY1-2状态显示
  - 通信状态指示
  - 错误计数显示

## 5. 维护记录
- 版本：V1.0.0
- 创建时间：2024-03-21
- 最后更新：2024-03-21
- 更新内容：添加开发计划

## 6. 开发计划

### 6.1 硬件初始化阶段
#### 6.1.1 I2C1配置 (OLED显示)
- SCL: PB6
- SDA: PB7
- 速率: 400kHz
- 地址: 0x78

#### 6.1.2 USART3配置 (RS485通信)
- TX: PB10 
- RX: PB11
- RS485_EN: PB8
- 波特率: 115200
- 数据位: 8
- 停止位: 1
- 无校验

#### 6.1.3 GPIO配置
- LED输出:
  - LED1: PA1 (低电平点亮)
  - LED2: PA2 (低电平点亮) 
  - LED3: PA3 (低电平点亮)
- 按键输入:
  - KEY1: PA0 (下拉输入)
  - KEY2: PC13 (下拉输入)
- 状态指示:
  - KEY_STATUS: PA6 (推挽输出)

### 6.2 软件模块开发
#### 6.2.1 OLED驱动模块
- 初始化OLED
- 显示字符功能
- 清屏功能
- 显示状态更新功能

#### 6.2.2 Modbus RTU从机协议
- 帧接收与解析
- CRC校验
- 响应帧生成
- 寄存器读写功能

#### 6.2.3 按键处理模块
- 按键扫描
- 消抖处理
- 状态变化检测
- 状态指示输出

#### 6.2.4 LED控制模块
- LED初始化
- LED开关控制
- LED状态读取

### 6.3 主程序逻辑实现
#### 6.3.1 系统初始化
- 时钟配置
- 外设初始化
- 变量初始化

#### 6.3.2 主循环
- 按键扫描
- Modbus通信处理
- LED状态更新
- OLED显示更新

#### 6.3.3 中断服务
- USART3接收中断
- 定时器中断(用于按键扫描)

### 6.4 调试与优化
#### 6.4.1 通信测试
- RS485通信测试
- Modbus协议测试
- 响应时序测试

#### 6.4.2 功能测试
- LED控制测试
- 按键功能测试
- OLED显示测试

#### 6.4.3 性能优化
- 中断响应时间优化
- 通信效率优化
- 显示刷新优化

### 6.5 文档完善
#### 6.5.1 更新文档
- 完善项目说明
- 添加使用说明
- 补充注意事项

#### 6.5.2 代码注释完善
- 添加中文注释
- 完善函数说明
- 添加关键算法说明

## 7. 开发进度

### 7.1 硬件初始化阶段
- [x] I2C1配置 (OLED显示)
  - [x] 引脚配置：SCL-PB6, SDA-PB7
  - [x] 速率设置：400kHz
  - [x] 地址设置：0x78

- [x] USART3配置 (RS485通信)
  - [x] 引脚配置：TX-PB10, RX-PB11, RS485_EN-PB8
  - [x] 通信参数：115200-8-1-N
  - [x] 中断接收配置
  - [x] 发送功能实现
  - [x] 回环测试完成

- [x] GPIO配置
  - [x] LED输出配置
  - [x] 按键输入配置
  - [x] 状态指示配置

### 7.2 软件模块开发
- [x] OLED驱动模块
  - [x] 初始化功能
  - [x] 显示字符功能
  - [x] 清屏功能
  - [x] 显示状态更新功能

- [x] Modbus RTU从机协议
  - [x] 帧接收与解析
  - [x] CRC校验
  - [x] 响应帧生成
  - [x] 基本功能实现：
    - [x] 读线圈(0x01)
    - [x] 读离散输入(0x02)
    - [x] 读保持寄存器(0x03)
    - [x] 读输入寄存器(0x04)
    - [x] 写单个线圈(0x05)
    - [x] 写单个保持寄存器(0x06)
    - [x] 写多个保持寄存器(0x10)

### 7.3 功能实现状态
- [x] 按键处理模块
  - [x] 按键扫描
  - [x] 消抖处理
  - [x] 状态变化检测
  - [x] 状态指示输出

- [x] LED控制模块
  - [x] LED初始化
  - [x] LED开关控制
  - [x] LED状态读取

- [ ] 调试与优化
  - [x] OLED显示测试
  - [x] RS485通信测试
  - [x] Modbus协议测试
  - [x] 按键功能测试
  - [x] LED控制测试
  - [x] 响应时序测试
  - [x] 中断响应时间优化
  - [ ] 通信效率优化

- [ ] 文档完善
  - [ ] 更新使用说明
  - [ ] 补充注意事项
  - [ ] 完善代码注释

## 参考资料
1. STM32F103数据手册
2. OLED显示屏规格书
3. MAX485数据手册
4. HAL库使用手册


