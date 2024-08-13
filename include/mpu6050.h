#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SMPLRT_DIV_REG      0x19
#define GYRO_CONFIG_REG     0x1B
#define ACCEL_CONFIG_REG    0x1C
#define ACCEL_XOUT_H_REG    0x3B
#define TEMP_OUT_H_REG      0x41
#define GYRO_XOUT_H_REG     0x43
#define PWR_MGMT_1_REG      0x6B
#define WHO_AM_I_REG        0x75

typedef struct {
	int16_t accel_x;
	int16_t accel_y;
    int16_t accel_z;
} mpu_accel_t;

typedef struct {
	int16_t gyro_x;
	int16_t gyro_y;
    int16_t gyro_z;
} mpu_gyro_t;

void mpu6050_init(i2c_inst_t *i2c, uint8_t addr);
void mpu6050_reset(void);
uint8_t mpu6050_who_am_i(void);
void mpu6050_read_accel(mpu_accel_t *accel);
void mpu6050_read_gyro(mpu_gyro_t *gyro);
void mpu6050_read_temp(int16_t *temp);

#endif /* INC_MPU6050_H_ */
