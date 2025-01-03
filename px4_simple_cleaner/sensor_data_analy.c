#include "sensor_data_analy.h"
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

// 定义全局共享数据
SensorData sensor_data;
KEYSTATUS key_status;
CHARGESTATUS charge_status;
POWERPRECENT precent;
FAN_TANK_STATUS fan_tank_status;
WARNING warning_status;

// 用于保护共享数据的信号量
sem_t data_mutex;



int parse_key_status(const uint8_t *received_data, KEYSTATUS *new_data) {
    if (received_data[1] < 5) {
        fprintf(stderr, "Data length is insufficient.\n");
        return 1;
    }
    new_data->key_cmd = received_data[4];
    return 0;
}

int parse_charge_status(const uint8_t *received_data, CHARGESTATUS *new_data) {
    if (received_data[1] < 6) {
        fprintf(stderr, "Data length is insufficient.\n");
        return 1;
    }
    new_data->charge_status = received_data[4];
    new_data->charge_type = received_data[5];
    return 0;
}

int parse_power_precent(const uint8_t *received_data, POWERPRECENT *new_data) {
    if (received_data[1] < 5) {
        fprintf(stderr, "Data length is insufficient.\n");
        return 1;
    }
    new_data->precent = received_data[4];
    return 0;
}

int parse_fan_tank_status(const uint8_t *received_data, FAN_TANK_STATUS *new_data) {
    if (received_data[1] < 5) {
        fprintf(stderr, "Data length is insufficient.\n");
        return 1;
    }
    new_data->fan_tank_status = received_data[4];
    return 0;
}

int parse_warning_status(const uint8_t *received_data, WARNING *new_data) {
    if (received_data[1] < 5) {
        fprintf(stderr, "Data length is insufficient.\n");
        return 1;
    }
    new_data->warning_value = received_data[4];
    return 0;
}




// 将网络字节序转换为主机字节序（16位）
static inline uint16_t my_ntohs(uint16_t net_short) {
    return ((net_short & 0x00FF) << 8) | ((net_short & 0xFF00) >> 8);
}

// 将网络字节序转换为主机字节序（32位）
static inline uint32_t my_ntohl(uint32_t net_long) {
    return ((net_long & 0x000000FF) << 24) |
           ((net_long & 0x0000FF00) << 8) |
           ((net_long & 0x00FF0000) >> 8) |
           ((net_long & 0xFF000000) >> 24);
}

int parse_sensor_data(const uint8_t *received_data, SensorData *new_data) {
    if (received_data[1] < 60) {
        fprintf(stderr, "Received data is too short!\n");
        return -1;
    }

    uint16_t frame_len;
    memcpy(&frame_len, received_data, sizeof(uint16_t));
    new_data->frame_len = my_ntohs(frame_len);

    new_data->ick = received_data[2];
    new_data->cmd = received_data[3];

    uint32_t left_encoder, right_encoder, pose_X, pose_Y, pose_theta;
    memcpy(&left_encoder, received_data + 4, sizeof(uint32_t));
    memcpy(&right_encoder, received_data + 8, sizeof(uint32_t));
    memcpy(&pose_X, received_data + 20, sizeof(uint32_t));
    memcpy(&pose_Y, received_data + 24, sizeof(uint32_t));
    memcpy(&pose_theta, received_data + 28, sizeof(uint32_t));

    new_data->left_encoder = my_ntohl(left_encoder);
    new_data->right_encoder = my_ntohl(right_encoder);
    new_data->pose_X = my_ntohl(pose_X);
    new_data->pose_Y = my_ntohl(pose_Y);
    new_data->pose_theta = my_ntohl(pose_theta);

    return 0;
}






SensorData get_sensor_data(void) {
    sem_wait(&data_mutex);
    SensorData data_copy = sensor_data;
    sem_post(&data_mutex);
    return data_copy;
}

KEYSTATUS get_key_status(void) {
    sem_wait(&data_mutex);
    KEYSTATUS data_copy = key_status;
    sem_post(&data_mutex);
    return data_copy;
}

CHARGESTATUS get_charge_status(void) {
    sem_wait(&data_mutex);
    CHARGESTATUS data_copy = charge_status;
    sem_post(&data_mutex);
    return data_copy;
}

POWERPRECENT get_power_precent(void) {
    sem_wait(&data_mutex);
    POWERPRECENT data_copy = precent;
    sem_post(&data_mutex);
    return data_copy;
}

FAN_TANK_STATUS get_fan_tank_status(void) {
    sem_wait(&data_mutex);
    FAN_TANK_STATUS data_copy = fan_tank_status;
    sem_post(&data_mutex);
    return data_copy;
}

WARNING get_warning_status(void) {
    sem_wait(&data_mutex);
    WARNING data_copy = warning_status;
    sem_post(&data_mutex);
    return data_copy;
}

void update_sensor_data(const SensorData *new_data) {
    sem_wait(&data_mutex);
    sensor_data = *new_data;
    sem_post(&data_mutex);
}

void update_key_status(const KEYSTATUS *new_status) {
    sem_wait(&data_mutex);
    key_status = *new_status;
    sem_post(&data_mutex);
}

void update_charge_status(const CHARGESTATUS *new_status) {
    sem_wait(&data_mutex);
    charge_status = *new_status;
    sem_post(&data_mutex);
}

void update_power_precent(const POWERPRECENT *new_status) {
    sem_wait(&data_mutex);
    precent = *new_status;
    sem_post(&data_mutex);
}

void update_fan_tank_status(const FAN_TANK_STATUS *new_status) {
    sem_wait(&data_mutex);
    fan_tank_status = *new_status;
    sem_post(&data_mutex);
}

void update_warning_status(const WARNING *new_status) {
    sem_wait(&data_mutex);
    warning_status = *new_status;
    sem_post(&data_mutex);
}
