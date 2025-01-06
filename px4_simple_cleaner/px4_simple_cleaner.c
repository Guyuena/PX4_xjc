

/*
#include <px4_platform_common/posix.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

// 串口配置函数
void configure_serial_port(int fd) {
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    // 获取当前串口设置
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error from tcgetattr");
        return;
    }

    // 设置波特率为115200
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // 8N1 模式: 8 数据位，无校验，1 停止位
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~PARENB;  // 无校验
    tty.c_cflag &= ~CSTOPB;  // 1 停止位
    tty.c_cflag &= ~CRTSCTS; // 禁用硬件流控制
    tty.c_cflag |= CREAD | CLOCAL;  // 启用读取，忽略调制解调器控制线

    // 设置为非规范模式（原始输入）
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // 禁用规范模式，回显和信号
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);          // 禁用软件流控制
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);  // 禁用特殊字节处理
    tty.c_oflag &= ~OPOST;  // 禁用输出处理

    // 应用设置
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error from tcsetattr");
    }
}

// 打印接收到的数据（16进制格式）
void print_hex_data(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

extern "C" __EXPORT int serial_test_diy_main(int argc, char **argv);

int serial_test_diy_main(int argc, char **argv) {
    const char *uart_name = "/dev/ttyS3";  // 这里使用 ttyS3 作为串口设备

    // 打开串口
    int fd = open(uart_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    printf("************  fd = %d ************\n", fd);
    if (fd < 0) {
        perror("Failed to open UART");
        return -1;
    }

    // 配置串口
    configure_serial_port(fd);

    // 接收数据
    uint8_t buffer[256];  // 数据缓冲区
    int n;

    while (1) {  // 不停接收数据
        n = read(fd, buffer, sizeof(buffer));
        if (n < 0) {
            if (errno == EAGAIN) {
                usleep(10000);  // 没有数据时等待10ms再试
                continue;
            } else {
                perror("Read error");
                break;
            }
        } else if (n == 0) {
            // 没有数据
            usleep(10000);  // 没有数据时等待10ms
        } else {
            // 打印接收到的数据（16进制格式）
            printf("Received data (%d bytes):\n", n);
            print_hex_data(buffer, n);
        }
    }

    // 关闭串口
    close(fd);
    return 0;
}

*/

/*

#include <px4_platform_common/posix.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

// 串口配置函数
void configure_serial_port(int fd) {
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    // 获取当前串口设置
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error from tcgetattr");
        return;
    }

    // 设置波特率为115200
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // 8N1 模式: 8 数据位，无校验，1 停止位
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~PARENB;  // 无校验
    tty.c_cflag &= ~CSTOPB;  // 1 停止位
    tty.c_cflag &= ~CRTSCTS; // 禁用硬件流控制
    tty.c_cflag |= CREAD | CLOCAL;  // 启用读取，忽略调制解调器控制线

    // 设置为非规范模式（原始输入）
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // 禁用规范模式，回显和信号
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);          // 禁用软件流控制
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);  // 禁用特殊字节处理
    tty.c_oflag &= ~OPOST;  // 禁用输出处理

    // 应用设置
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error from tcsetattr");
    }
}

// 打印接收到的数据（16进制格式）
void print_hex_data(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

// 提取完整帧
void extract_frame(uint8_t *buffer, size_t n) {
    size_t i = 0;
    while (i < n) {
        // 查找帧头 B5 62
        if (buffer[i] == 0xB5 && buffer[i + 1] == 0x62) {
            // 获取第三、第四字节表示的剩余数据长度
            uint16_t remaining_len = (buffer[i + 2] << 8) | buffer[i + 3];  // 剩余数据长度（包括帧头）

            // 计算完整帧的总长度
            size_t total_len = 4 + remaining_len;  // 4字节是帧头加第三、第四字节

            // 确保接收到的数据包含完整的帧
            if (i + total_len <= n) {
                // 打印完整帧数据
                printf("Received a full frame of length %zu: \n", total_len);
                print_hex_data(&buffer[i], total_len);
                // 跳过已处理的完整帧
                i += total_len;
            } else {
                // 如果接收到的数据不完整，退出循环（等待更多数据）
                break;
            }
        } else {
            // 如果当前字节不是帧头，继续查找
            i++;
        }
    }
}

extern "C" __EXPORT int serial_test_diy_main(int argc, char **argv);

int serial_test_diy_main(int argc, char **argv) {
    const char *uart_name = "/dev/ttyS3";  // 这里使用 ttyS3 作为串口设备

    // 打开串口
    int fd = open(uart_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    printf("************  fd = %d ************\n", fd);
    if (fd < 0) {
        perror("Failed to open UART");
        return -1;
    }

    // 配置串口
    configure_serial_port(fd);

    // 接收数据
    uint8_t buffer[256];  // 数据缓冲区
    int n;

    while (1) {  // 不停接收数据
        n = read(fd, buffer, sizeof(buffer));
        if (n < 0) {
            if (errno == EAGAIN) {
                usleep(10000);  // 没有数据时等待10ms再试
                continue;
            } else {
                perror("Read error");
                break;
            }
        } else if (n == 0) {
            // 没有数据
            usleep(10000);  // 没有数据时等待10ms
        } else {
            // 提取完整数据帧
            extract_frame(buffer, n);
        }
    }

    // 关闭串口
    close(fd);
    return 0;
}*/

/*


#include <px4_platform_common/posix.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

// 数据缓冲区
uint8_t buffer[256];
// 互斥锁
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

// 串口配置函数
void configure_serial_port(int fd) {
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    // 获取当前串口设置
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error from tcgetattr");
        return;
    }

    // 设置波特率为115200
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // 8N1 模式: 8 数据位，无校验，1 停止位
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~PARENB;  // 无校验
    tty.c_cflag &= ~CSTOPB;  // 1 停止位
    tty.c_cflag &= ~CRTSCTS; // 禁用硬件流控制
    tty.c_cflag |= CREAD | CLOCAL;  // 启用读取，忽略调制解调器控制线

    // 设置为非规范模式（原始输入）
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // 禁用规范模式，回显和信号
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);          // 禁用软件流控制
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);  // 禁用特殊字节处理
    tty.c_oflag &= ~OPOST;  // 禁用输出处理

    // 应用设置
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error from tcsetattr");
    }
}

// 打印接收到的数据（16进制格式）
void print_hex_data(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

// 提取完整帧
void extract_frame(uint8_t *frame_buffer, size_t n) {
    size_t i = 0;
    while (i < n) {
        // 查找帧头 B5 62
        if (frame_buffer[i] == 0xB5 && frame_buffer[i + 1] == 0x62) {
            // 获取第三、第四字节表示的剩余数据长度
            uint16_t remaining_len = (frame_buffer[i + 2] << 8) | frame_buffer[i + 3];  // 剩余数据长度（包括帧头）

            // 计算完整帧的总长度
            size_t total_len = 4 + remaining_len;  // 4字节是帧头加第三、第四字节

            // 确保接收到的数据包含完整的帧
            if (i + total_len <= n) {
                // 打印完整帧数据
                printf("Received a full frame of length %zu: \n", total_len);
                print_hex_data(&frame_buffer[i], total_len);
                // 跳过已处理的完整帧
                i += total_len;
            } else {
                // 如果接收到的数据不完整，退出循环（等待更多数据）
                break;
            }
        } else {
            // 如果当前字节不是帧头，继续查找
            i++;
        }
    }
}

// 数据接收线程
void* data_receive_thread(void* arg) {
    const char *uart_name = "/dev/ttyS3";  // 串口设备

    // 打开串口
    int fd = open(uart_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        perror("Failed to open UART");
        return NULL;
    }

    // 配置串口
    configure_serial_port(fd);

    int n;
    while (1) {
        // 读取串口数据
        n = read(fd, buffer, sizeof(buffer));
        if (n < 0) {
            if (errno == EAGAIN) {
                usleep(10000);  // 没有数据时等待
                continue;
            } else {
                perror("Read error");
                break;
            }
        } else if (n == 0) {
            usleep(10000);  // 没有数据时等待
        } else {
            // 在处理 buffer 前加锁
            pthread_mutex_lock(&buffer_mutex);

            // 提取完整帧（只读数据）
            extract_frame(buffer, n);

            // 释放锁后，允许其他线程更新 buffer
            pthread_mutex_unlock(&buffer_mutex);
        }
    }

    // 关闭串口
    close(fd);
    return NULL;
}



extern "C" __EXPORT int serial_test_diy_main(int argc, char **argv);

int serial_test_diy_main(int argc, char **argv) {
    pthread_t receive_thread;

    // 创建接收数据的线程
    if (pthread_create(&receive_thread, NULL, data_receive_thread, NULL) != 0) {
        perror("Failed to create receive thread");
        return -1;
    }

    // 等待接收线程结束
    pthread_join(receive_thread, NULL);

    return 0;
}

*/


#include <px4_platform_common/posix.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "sensor_data_analy.h"


// 数据缓冲区
uint8_t buffer[256];
// 数据帧长度
size_t buffer_len = 0;
// 互斥锁
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
// 条件变量
pthread_cond_t data_ready_cond = PTHREAD_COND_INITIALIZER;

// 串口配置函数
void configure_serial_port(int fd) {
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    // 获取当前串口设置
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error from tcgetattr");
        return;
    }

    // 设置波特率为115200
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // 8N1 模式: 8 数据位，无校验，1 停止位
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~PARENB;  // 无校验
    tty.c_cflag &= ~CSTOPB;  // 1 停止位
    tty.c_cflag &= ~CRTSCTS; // 禁用硬件流控制
    tty.c_cflag |= CREAD | CLOCAL;  // 启用读取，忽略调制解调器控制线

    // 设置为非规范模式（原始输入）
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // 禁用规范模式，回显和信号
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);          // 禁用软件流控制
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);  // 禁用特殊字节处理
    tty.c_oflag &= ~OPOST;  // 禁用输出处理

    // 应用设置
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error from tcsetattr");
    }
}

// 打印接收到的数据（16进制格式）
void print_hex_data(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

// 提取完整帧
void extract_frame(uint8_t *frame_buffer, size_t n) {
    size_t i = 0;
    while (i < n) {
        // 查找帧头 B5 62
        if (frame_buffer[i] == 0xB5 && frame_buffer[i + 1] == 0x62) {
            // 获取第三、第四字节表示的剩余数据长度
            uint16_t remaining_len = (frame_buffer[i + 2] << 8) | frame_buffer[i + 3];  // 剩余数据长度（包括帧头）

            // 计算完整帧的总长度
            size_t total_len = 4 + remaining_len;  // 4字节是帧头加第三、第四字节

            // 确保接收到的数据包含完整的帧
            if (i + total_len <= n) {
                // 加锁保护共享数据
                pthread_mutex_lock(&buffer_mutex);

                // 存储接收到的数据帧
                buffer_len = total_len;
                memcpy(buffer, &frame_buffer[i], total_len);

                // 发送通知，通知子线程数据已经准备好
                pthread_cond_signal(&data_ready_cond);

                // 释放锁
                pthread_mutex_unlock(&buffer_mutex);

                // 跳过已处理的完整帧
                i += total_len;
            } else {
                // 如果接收到的数据不完整，退出循环（等待更多数据）
                break;
            }
        } else {
            // 如果当前字节不是帧头，继续查找
            i++;
        }
    }
}







// 数据处理线程
void* data_processing_thread(void* arg) {
    while (1) {
        // 等待接收线程的通知
        pthread_mutex_lock(&buffer_mutex);
        while (buffer_len == 0) {
            // 如果没有数据，等待
            pthread_cond_wait(&data_ready_cond, &buffer_mutex);
        }

        // 打印数据帧
        // printf("Processing received data frame:\n");


        // 根据不同的 cmd 字段来处理不同的数据
        if (buffer[5] == 0x01) {
            // printf("Processing received data frame:  0x01 \n");
            SensorData new_data;
            parse_sensor_data(buffer, &new_data);
        }
        if (buffer[5] == 0x04) {
            // printf("Processing received data frame:  0x04 \n");
            KEYSTATUS new_key_status;
            parse_key_status(buffer, &new_key_status);

        }
        if (buffer[5] == 0x05) {
            CHARGESTATUS new_charge_status;
            parse_charge_status(buffer, &new_charge_status);
        }

        if (buffer[5] == 0x06) {

            POWERPRECENT new_power_precent;
            parse_power_precent(buffer, &new_power_precent);
        }
        if (buffer[5] == 0x07) {
            FAN_TANK_STATUS new_fan_tank_status;
            parse_fan_tank_status(buffer, &new_fan_tank_status);

        }
        if (buffer[5] == 0x08) {
            WARNING new_warning;
            parse_warning_status(buffer, &new_warning);

        }



        // print_hex_data(buffer, buffer_len);

        // 处理完毕后重置数据
        buffer_len = 0;

        // 释放锁
        pthread_mutex_unlock(&buffer_mutex);
    }
    return NULL;
}


// 数据接收线程
void* data_receive_thread(void* arg) {
    const char *uart_name = "/dev/ttyS3";  // 串口设备

    // 打开串口
    int fd = open(uart_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        perror("Failed to open UART");
        return NULL;
    }

    // 配置串口
    configure_serial_port(fd);

    uint8_t local_buffer[256];
    int n;

    while (1) {
        // 读取串口数据
        n = read(fd, local_buffer, sizeof(local_buffer));
        if (n < 0) {
            if (errno == EAGAIN) {
                usleep(10000);  // 没有数据时等待
                continue;
            } else {
                perror("Read error");
                break;
            }
        } else if (n == 0) {
            usleep(10000);  // 没有数据时等待
        } else {
            // 提取帧
            extract_frame(local_buffer, n);
        }
    }

    // 关闭串口
    close(fd);
    return NULL;
}


extern "C" __EXPORT int px4_simple_cleaner_main(int argc, char **argv);

int px4_simple_cleaner_main(int argc, char **argv) {
    pthread_t receive_thread, processing_thread;

    // 创建接收数据的线程
    if (pthread_create(&receive_thread, NULL, data_receive_thread, NULL) != 0) {
        perror("Failed to create receive thread");
        return -1;
    }

    // 创建处理数据的线程
    if (pthread_create(&processing_thread, NULL, data_processing_thread, NULL) != 0) {
        perror("Failed to create processing thread");
        return -1;
    }

    // 等待接收线程结束
    pthread_join(receive_thread, NULL);
    pthread_join(processing_thread, NULL);

    return 0;
}




