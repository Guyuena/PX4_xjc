#ifndef SENSOR_DATA_ANALY_H
#define SENSOR_DATA_ANALY_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>

#define BUFFER_SIZE 256

// 传感器数据结构体
typedef struct {
    int16_t frame_len;             // 索引+2
    uint8_t ick;                   // 索引+4
    uint8_t cmd;                   // 索引+5
    int32_t left_encoder;          // 索引+6
    int32_t right_encoder;         // 索引+10
    int16_t zero_0;                // 置0 索引+14
    int16_t pose_theta_0;          // 索引+16
    int16_t zero_1;                // 置 0 索引+18
    int16_t zero_2;                // 置 0 索引+20
    int32_t pose_X;                // 索引+22
    int32_t pose_Y;                // 索引+26
    int32_t pose_theta;            // 索引+30
    uint16_t Pix_DX;               // 置 0 索引+34
    uint16_t Pix_DY;               // 置 0 索引+36
    uint16_t IrWall_left;          // 沿墙 索引+38
    uint16_t IrWall_right;         // 索引+40
    uint8_t Bumps_triggered;       // 碰撞检测 索引+42
    uint8_t Cliff_triggered;       // 地检 索引+43
    uint8_t magnet_triggered;      // 左右地磁检测 索引+44
    uint8_t zero_3;                // 置 0 索引+45
    uint8_t dustbox_installed;     // 水箱尘盒 索引+46
    uint16_t IrBump_Left;          // 置 0 索引+47
    uint16_t IrBump_Middle;        // 置 0 索引+49
    uint16_t IrBump_Right;         // 置 0 索引+51
    uint8_t nec_right_front_dock;  // 红外右前 索引+53
    uint8_t nec_left_front_dock;   // 红外左前 索引+54
    uint8_t nec_left_dock;         // 红外左 索引+55
    uint8_t nec_right_dock;        // 红外右 索引+56
    uint8_t nec_left_back_dock;    // 置 0 索引+57 暂未使用
    uint8_t nec_right_back_dock;   // 置 0 索引+58 暂未使用
    uint8_t charge_access;         // 充电接触 索引+59
    uint8_t material;              // 地面材质 索引+60
    uint32_t Time_tick;            // now_time 索引+61
} SensorData;

// 按键状态结构体
typedef struct {
    uint8_t key_cmd; // 按键命令
} KEYSTATUS;

// 充电状态结构体
typedef struct {
    uint8_t charge_status; // 充电状态
    uint8_t charge_type;   // 充电类型
} CHARGESTATUS;

// 电量百分比结构体
typedef struct {
    uint8_t precent; // 电量百分比
} POWERPRECENT;

// 风机水箱状态结构体
typedef struct {
    uint8_t fan_tank_status; // 风机水箱状态
} FAN_TANK_STATUS;

// 警告信息结构体
typedef struct {
    int32_t warning_value; // 警告值
} WARNING;

// 全局共享数据
extern SensorData sensor_data;
extern KEYSTATUS key_status;
extern CHARGESTATUS charge_status;
extern POWERPRECENT power_precent;
extern FAN_TANK_STATUS fan_tank_status;
extern WARNING warning_status;

// 信号量保护共享数据
extern sem_t data_mutex;

// 函数声明
int parse_sensor_data(const uint8_t *received_data, SensorData *new_data);
int parse_key_status(const uint8_t *received_data, KEYSTATUS *new_data);
int parse_charge_status(const uint8_t *received_data, CHARGESTATUS *new_data);
int parse_power_precent(const uint8_t *received_data, POWERPRECENT *new_data);
int parse_fan_tank_status(const uint8_t *received_data, FAN_TANK_STATUS *new_data);
int parse_warning_status(const uint8_t *received_data, WARNING *new_data);

SensorData get_sensor_data(void);
KEYSTATUS get_key_status(void);
CHARGESTATUS get_charge_status(void);
POWERPRECENT get_power_precent(void);
FAN_TANK_STATUS get_fan_tank_status(void);
WARNING get_warning_status(void);

void update_sensor_data(const SensorData *new_data);
void update_key_status(const KEYSTATUS *new_status);
void update_charge_status(const CHARGESTATUS *new_status);
void update_power_precent(const POWERPRECENT *new_status);
void update_fan_tank_status(const FAN_TANK_STATUS *new_status);
void update_warning_status(const WARNING *new_status);

#endif
