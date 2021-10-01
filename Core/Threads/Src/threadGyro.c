#include "threadGyro.h"
#include "config.h"

int16_t GetGyroData(uint8_t st, uint16_t dt){
    if (st == ST_GYRO_1){
        if (dt == DT_RAW_DATA)  return gyro1_raw;
        else                    return BITS_TO_RATE(gyro1_raw);
    }
    else if (st == ST_GYRO_2){
        if (dt == DT_RAW_DATA)  return gyro2_raw;
        else                    return BITS_TO_RATE(gyro2_raw);
    }
    else if (st == ST_TEMP_1){
        if (dt == DT_RAW_DATA)  return temp1_raw;
        else                    return BITS_TO_TEMP(temp1_raw);
    }
    else if (st == ST_TEMP_2){
        if (dt == DT_RAW_DATA)  return temp2_raw;
        else                    return BITS_TO_TEMP(temp2_raw);
    }
    else return DT_ERROR;
}

void GyroOverheatCallback(void *arg){
    return;
}

void threadGyro(void){
    crg20_default_t def_frame;
    crg20_temp_t    temp_frame;
    GyroMutex = osMutexNew(NULL);
    osMutexRelease(GyroMutex);
    GyroOverheatTimer = osTimerNew(GyroOverheatCallback, osTimerPeriodic, (void *)0, NULL);
    osTimerStart(GyroOverheatTimer, 1000);
    while(1){
        if (CRG20_GetDefault(&gyro1, &def_frame) == CRG20_OK){
            gyro1_raw = def_frame.rate;
        }
        else gyro1_raw = 0xffff;
        if (CRG20_GetDefault(&gyro2, &def_frame) == CRG20_OK){
            gyro2_raw = def_frame.rate;
        }
        else gyro2_raw = 0xffff;
        if (CRG20_GetTemperature(&gyro1, &temp_frame) == CRG20_OK){
            temp1_raw = temp_frame.data;
        }
        else temp1_raw = 0xffff;
        if (CRG20_GetTemperature(&gyro2, &temp_frame) == CRG20_OK){
            temp2_raw = temp_frame.data;
        }
        else temp2_raw = 0xffff;
        osDelay(50);
    }
}