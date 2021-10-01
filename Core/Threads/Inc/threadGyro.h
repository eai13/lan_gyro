#ifndef GYRO_TASK_THREAD_H
#define GYRO_TASK_THREAD_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "config.h"
#include "crg20.h"

extern crg20_module_t gyro1;
extern crg20_module_t gyro2;

static int16_t gyro1_raw;
static int16_t gyro2_raw;
static int16_t temp1_raw;
static int16_t temp2_raw;

osTimerId_t GyroOverheatTimer;
osMutexId_t GyroMutex;

void GyroOverheatCallback(void * arg);

int16_t GetGyroData(uint8_t st, uint16_t dt);

void threadGyro(void);

#endif