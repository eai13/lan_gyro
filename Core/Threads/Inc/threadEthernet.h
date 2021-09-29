#ifndef ETHERNET_TASK_THREAD_H
#define ETHERNET_TASK_THREAD_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

void threadEthernet(void const * arg);

#endif