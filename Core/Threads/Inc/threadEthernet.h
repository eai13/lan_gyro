#ifndef ETHERNET_TASK_THREAD_H
#define ETHERNET_TASK_THREAD_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#define NET_BUFFER_SIZE 600

typedef enum sensor_type{
    GYRO_1 = 0x00,
    GYRO_2 = 0x01,
    TEMP_1 = 0x02,
    TEMP_2 = 0x03
}sensor_type_t;

typedef enum data_type{
    RAW_DATA  = 0x0100,
    CONV_DATA = 0x0200
}data_type_t;

typedef enum msg_type{
    REQUEST = 0x01,
    ANSWER  = 0x02
}msg_type_t;

typedef struct net_packet{
    msg_type_t      m_type   : 8;
    sensor_type_t   s_type   : 8;
    data_type_t     d_type   : 16;
    uint8_t         checksum : 8;
}net_packet_t;

void threadEthernet(void const * arg);

#endif