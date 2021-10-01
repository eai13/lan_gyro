#ifndef LAN_CONFIG_H
#define LAN_CONFIG_H

typedef enum sensor_type{
    ST_ERROR       = 0x00,
    ST_GYRO_1      = 0x01,
    ST_GYRO_2      = 0x02,
    ST_TEMP_1      = 0x03,
    ST_TEMP_2      = 0x04,
    ST_END
}sensor_type_t;

typedef enum data_type{
    DT_ERROR     = 0x0100,
    DT_RAW_DATA  = 0x0101,
    DT_CONV_DATA = 0x0102,
    DT_END
}data_type_t;

typedef enum msg_type{
    MT_REQUEST = 0x01,
    MT_ANSWER  = 0x02,
    MT_END
}msg_type_t;

#endif