#ifndef ETHERNET_TASK_THREAD_H
#define ETHERNET_TASK_THREAD_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "config.h"
#include "debug.h"
#include "net_prots.h"
// #include "threadGyro.h"

#define NET_BUFFER_SIZE 600

#define __ASSERT_PACK(pack)     (((pack->m_type == MT_REQUEST) && \
                                  (pack->s_type < ST_END) && \
                                  ((pack->m_type == MT_REQUEST) && (pack->d_type < DT_END))) \
                                  ? (1) : (0))

typedef struct net_packet{
    uint8_t         m_type;
    uint8_t         s_type;
    int16_t         d_type;
    uint8_t         checksum;
}net_packet_t;

void threadEthernet(void const * arg);

#endif