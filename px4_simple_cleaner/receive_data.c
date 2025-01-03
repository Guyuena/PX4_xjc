#include <nuttx/config.h>
#include <nuttx/semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "receive_data.h"
#include "sensor_data_analy.h"

#define BUFFER_SIZE 256
#define QUEUE_SIZE 10

static int serial_fd = -1; // 串口文件描述符，初始化为无效值
static pthread_mutex_t serial_mutex = PTHREAD_MUTEX_INITIALIZER; // 互斥锁，用于保护串口操作

// 循环队列结构体
typedef struct {
    uint8_t data[QUEUE_SIZE][BUFFER_SIZE];
    int head;
    int tail;
    sem_t mutex;
    sem_t data_ready;
} CircularQueue;

static CircularQueue data_queue = {
    .head = 0,
    .tail = 0
};

// 初始化循环队列
void init_queue(CircularQueue *queue) {
    sem_init(&queue->mutex, 0, 1);
    sem_init(&queue->data_ready, 0, 0);
}

// 循环队列：插入数据
void enqueue(CircularQueue *queue, const uint8_t *data, int length) {
    sem_wait(&queue->mutex);
    if ((queue->head + 1) % QUEUE_SIZE == queue->tail) {
        // 队列已满，丢弃最旧的数据
        queue->tail = (queue->tail + 1) % QUEUE_SIZE;
    }
    memcpy(queue->data[queue->head], data, length);
    queue->head = (queue->head + 1) % QUEUE_SIZE;
    sem_post(&queue->mutex);
    sem_post(&queue->data_ready);
}

// 循环队列：取出数据
int dequeue(CircularQueue *queue, uint8_t *buffer) {
    sem_wait(&queue->data_ready);
    sem_wait(&queue->mutex);
    memcpy(buffer, queue->data[queue->tail], BUFFER_SIZE);
    queue->tail = (queue->tail + 1) % QUEUE_SIZE;
    sem_post(&queue->mutex);
    return 1;
}

// 打开串口设备
int open_serial_port(const char *serial_port) {
    int fd = open(serial_port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd == -1) {
        perror("Failed to open serial port");
    }
    return fd;
}

// 串口配置函数
int configure_serial_port(int fd, speed_t baudrate) {
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    // 获取当前串口设置
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error from tcgetattr");
        return -1;
    }

    // 设置波特率
    cfsetospeed(&tty, baudrate);
    cfsetispeed(&tty, baudrate);

    // 8N1 模式: 8 数据位，无校验，1 停止位
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 设置为8数据位
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
        return -1;
    }

    return 0;  // 配置成功
}

// 初始化串口
int uart_init(const char *serial_port) {
    // 打开串口
    serial_fd = open_serial_port(serial_port);
    if (serial_fd == -1) {
        fprintf(stderr, "Failed to open serial port %s\n", serial_port);
        return -1;
    }

    // 配置串口波特率和参数
    if (configure_serial_port(serial_fd, B115200) == -1) {
        fprintf(stderr, "Failed to configure serial port\n");
        close(serial_fd);
        return -1;
    }

    // 配置成功，返回串口文件描述符
    return serial_fd;
}

// 使用 select() 替代轮询
void *uart_read_thread(void *arg) {
    uint8_t buffer[BUFFER_SIZE];
    int buffer_index = 0;

    fd_set read_fds;
    struct timeval timeout;

    while (1) {
        pthread_mutex_lock(&serial_mutex);  // 加锁，确保串口描述符不被多个线程同时操作

        if (serial_fd < 0) {
            perror("Invalid serial file descriptor");
            pthread_mutex_unlock(&serial_mutex);  // 解锁
            break;  // 如果文件描述符无效，直接退出
        }

        FD_ZERO(&read_fds);
        FD_SET(serial_fd, &read_fds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ret = select(serial_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (ret < 0) {
            if (errno == EBADF) {
                perror("Select error: Bad file number");
                pthread_mutex_unlock(&serial_mutex);  // 解锁
                break;  // 如果是无效文件描述符错误，退出
            }
            perror("Select error");
            pthread_mutex_unlock(&serial_mutex);  // 解锁
            break;
        } else if (ret == 0) {
            // 超时，没有数据可读
            pthread_mutex_unlock(&serial_mutex);  // 解锁
            continue;
        }

        if (FD_ISSET(serial_fd, &read_fds)) {
            ssize_t num_bytes = read(serial_fd, buffer + buffer_index, 1);
            if (num_bytes > 0) {
                buffer_index += num_bytes;

                // 检查是否检测到帧头 (0xB5 0x62)
                if (buffer_index >= 2 && buffer[buffer_index - 2] == 0xB5 && buffer[buffer_index - 1] == 0x62) {
                    // 打印完整的数据帧
                    // printf("Complete frame received: ");
                    // for (int i = 0; i < buffer_index; i++) {
                    //     printf("0x%02X ", buffer[i]);
                    // }
                    // printf("\n");

                    // 将数据帧放入队列
                    enqueue(&data_queue, buffer, buffer_index);
                    buffer_index = 0;
                }
            } else if (num_bytes < 0) {
                perror("Error reading from serial port");
                pthread_mutex_unlock(&serial_mutex);  // 解锁
                break;
            }
        }

        pthread_mutex_unlock(&serial_mutex);  // 解锁
    }
    return NULL;
}

// 数据处理线程
// void *data_processing_thread(void *arg) {
//     uint8_t received_data[BUFFER_SIZE];
//     while (1) {
//         dequeue(&data_queue, received_data);
//         printf("0x%02X ", received_data[3]);
//         // 根据不同的 cmd 字段处理数据
//         switch (received_data[3]) {
//         case 0x01:
//             printf("Processing Sensor Data\n");
//             break;
//         case 0x04:
//             printf("Processing Key Status\n");
//             break;
//         case 0x05:
//             printf("Processing Charge Status\n");
//             break;
//         case 0x06:
//             printf("Processing Power Percent\n");
//             break;
//         case 0x07:
//             printf("Processing Fan Tank Status\n");
//             break;
//         case 0x08:
//             printf("Processing Warning\n");
//             break;
//         default:
//             printf("Unknown Command: 0x%02X\n", received_data[3]);
//         }
//     }
//     return NULL;
// }

// 数据处理线程
// void *data_processing_thread(void *arg) {
//     uint8_t received_data[BUFFER_SIZE];

//     while (1) {
//         // 等待队列中有数据，使用信号量进行同步
//         sem_wait(&data_queue.data_ready);  // 等待数据准备好

//         // 从队列中取出数据
//         dequeue(&data_queue, received_data);

//         printf("Processing Command: 0x%02X\n", received_data[3]);

//         // 根据不同的 cmd 字段处理数据
//         switch (received_data[3]) {
//         case 0x01:
//             printf("Processing Sensor Data\n");
//             {
//                 SensorData new_data;
//                 int result = parse_sensor_data(received_data, &new_data);
//                 if (result == 0) {
//                     update_sensor_data(&new_data);
//                 }
//             }
//             break;
//         case 0x04:
//             printf("Processing Key Status\n");
//             {
//                 KEYSTATUS new_key_status;
//                 int result = parse_key_status(received_data, &new_key_status);
//                 if (result == 0) {
//                     update_key_status(&new_key_status);
//                 }
//             }
//             break;
//         case 0x05:
//             printf("Processing Charge Status\n");
//             {
//                 CHARGESTATUS new_charge_status;
//                 int result = parse_charge_status(received_data, &new_charge_status);
//                 if (result == 0) {
//                     update_charge_status(&new_charge_status);
//                 }
//             }
//             break;
//         case 0x06:
//             printf("Processing Power Percent\n");
//             {
//                 POWERPRECENT new_power_precent;
//                 int result = parse_power_precent(received_data, &new_power_precent);
//                 if (result == 0) {
//                     update_power_precent(&new_power_precent);
//                 }
//             }
//             break;
//         case 0x07:
//             printf("Processing Fan Tank Status\n");
//             {
//                 FAN_TANK_STATUS new_fan_tank_status;
//                 int result = parse_fan_tank_status(received_data, &new_fan_tank_status);
//                 if (result == 0) {
//                     update_fan_tank_status(&new_fan_tank_status);
//                 }
//             }
//             break;
//         case 0x08:
//             printf("Processing Warning\n");
//             {
//                 WARNING new_warning;
//                 int result = parse_warning_status(received_data, &new_warning);
//                 if (result == 0) {
//                     update_warning_status(&new_warning);
//                 }
//             }
//             break;
//         default:
//             printf("Unknown Command: 0x%02X\n", received_data[3]);
//             break;
//         }
//     }
//     return NULL;
// }

void *data_processing_thread(void *arg) {
    uint8_t received_data[BUFFER_SIZE];

    while (1) {
        // 等待数据准备好
        sem_wait(&data_queue.data_ready);  // 等待数据准备好

        // 从队列中取出数据
        int ret = dequeue(&data_queue, received_data);
        if (ret != 1) {
            printf("Failed to dequeue data, retrying...\n");
            continue;  // 如果没有数据或者数据无效，跳过并继续循环
        }

        printf("Processing Command: 0x%02X\n", received_data[3]);

        // 根据不同的 cmd 字段处理数据
        switch (received_data[3]) {
        case 0x01:
            printf("Processing Sensor Data\n");
            // {
            //     SensorData new_data;
            //     int result = parse_sensor_data(received_data, &new_data);
            //     if (result == 0) {
            //         update_sensor_data(&new_data);
            //     }
            // }
            break;
        case 0x04:
            printf("Processing Key Status\n");
            // {
            //     KEYSTATUS new_key_status;
            //     int result = parse_key_status(received_data, &new_key_status);
            //     if (result == 0) {
            //         update_key_status(&new_key_status);
            //     }
            // }
            break;
        case 0x05:
            printf("Processing Charge Status\n");
            // {
            //     CHARGESTATUS new_charge_status;
            //     int result = parse_charge_status(received_data, &new_charge_status);
            //     if (result == 0) {
            //         update_charge_status(&new_charge_status);
            //     }
            // }
            break;
        case 0x06:
            printf("Processing Power Percent\n");
            // {
            //     POWERPRECENT new_power_precent;
            //     int result = parse_power_precent(received_data, &new_power_precent);
            //     if (result == 0) {
            //         update_power_precent(&new_power_precent);
            //     }
            // }
            break;
        case 0x07:
            printf("Processing Fan Tank Status\n");
            // {
            //     FAN_TANK_STATUS new_fan_tank_status;
            //     int result = parse_fan_tank_status(received_data, &new_fan_tank_status);
            //     if (result == 0) {
            //         update_fan_tank_status(&new_fan_tank_status);
            //     }
            // }
            break;
        case 0x08:
            printf("Processing Warning\n");
            // {
            //     WARNING new_warning;
            //     int result = parse_warning_status(received_data, &new_warning);
            //     if (result == 0) {
            //         update_warning_status(&new_warning);
            //     }
            // }
            break;
        default:
            printf("Unknown Command: 0x%02X\n", received_data[3]);
            break;
        }
    }
    return NULL;
}




// 启动串口读取和数据处理的线程
void start_uart_processing(void) {
    init_queue(&data_queue);

    pthread_t uart_read_tid, data_process_tid;

    // 创建 UART 读取线程
    if (pthread_create(&uart_read_tid, NULL, uart_read_thread, NULL) != 0) {
        perror("Failed to create uart_read_thread");
    }

    // 创建数据处理线程
    if (pthread_create(&data_process_tid, NULL, data_processing_thread, NULL) != 0) {
        perror("Failed to create data_processing_thread");
    }

    // 等待线程结束
    pthread_join(uart_read_tid, NULL);
    pthread_join(data_process_tid, NULL);


        // 将线程设置为分离状态，变为异步执行
    // pthread_detach(uart_read_tid);
    // pthread_detach(data_process_tid);
}
