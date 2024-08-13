#include "mpu6050.h"

// Direccion del MPU6050
static uint8_t mpu6050_addr;
// Bus de I2C
static i2c_inst_t *mpu6050_i2c;

static inline void mpu6050_write(uint8_t reg, uint8_t *src, uint8_t len) {
    // Array completo para enviar
    uint8_t buff[20] = {0};
    // Registro a escribir
    buff[0] = reg;
    // Copio los otros
    for(uint8_t i = 0; i < len; i++) { buff[i + 1] = src[i]; }
    // Inicio la comunicacion
    i2c_write_blocking(mpu6050_i2c, mpu6050_addr, buff, len + 1, false);
}

static inline void mpu6050_read(uint8_t reg, uint8_t *dst, uint8_t len) {
    // Inicio la comunicacion
    i2c_write_blocking(mpu6050_i2c, mpu6050_addr, &reg, 1, true);
    // Leo los bytes
    i2c_read_blocking(mpu6050_i2c, mpu6050_addr, dst, len, false);
}

void mpu6050_init(i2c_inst_t *i2c, uint8_t addr) {
    // Guardo bus de I2C y direccion
    mpu6050_i2c = i2c;
    mpu6050_addr = addr;
    
    // power management register 0X6B we should write all 0's to wake the sensor up
    uint8_t data = 0;
    mpu6050_write(PWR_MGMT_1_REG, &data, 1);
    // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
    data = 0x07;
    mpu6050_write(SMPLRT_DIV_REG, &data, 1);
    // Set accelerometer configuration in ACCEL_CONFIG Register
    // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
    data = 0;
    mpu6050_write(ACCEL_CONFIG_REG, &data, 1);
    // Set Gyroscopic configuration in GYRO_CONFIG Register
    // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
    data = 0;
    mpu6050_write(GYRO_CONFIG_REG, &data, 1);
}

void mpu6050_reset(void) {
    uint8_t rst = 0x80;
    mpu6050_write(PWR_MGMT_1_REG, &rst, 1);
}

uint8_t mpu6050_who_am_i(void) {
    // check device ID WHO_AM_I
    uint8_t who_am_i;
    mpu6050_read(WHO_AM_I_REG, &who_am_i, 1);
    return who_am_i;
}

void mpu6050_read_accel(mpu_accel_t *accel) {
    // Array para guardar valores
	uint8_t dst[6] = {0};
	// Leer 14 bytes del MPU6050
    mpu6050_read(ACCEL_XOUT_H_REG, dst, 6);
    // Ajusto valores de aceleracion
    accel->accel_x = (int16_t)(dst[0] << 8 | dst[1]);
    accel->accel_y = (int16_t)(dst[2] << 8 | dst[3]);
    accel->accel_z = (int16_t)(dst[4] << 8 | dst[5]);
}

void mpu6050_read_gyro(mpu_gyro_t *gyro) {
    // Array para guardar valores
	uint8_t dst[6] = {0};
	// Leer 14 bytes del MPU6050
    mpu6050_read(GYRO_XOUT_H_REG, dst, 6);
    // Ajusto valores de aceleracion
    gyro->gyro_x = (int16_t)(dst[0] << 8 | dst[1]);
    gyro->gyro_y = (int16_t)(dst[2] << 8 | dst[3]);
    gyro->gyro_z = (int16_t)(dst[4] << 8 | dst[5]);
}

void mpu6050_read_temp(int16_t *temp) {
    // Array para guardar valores
	uint8_t dst[2] = {0};
	// Leer 14 bytes del MPU6050
    mpu6050_read(TEMP_OUT_H_REG, dst, 6);
    // Ajusto valores de aceleracion
    *temp = (int16_t)(dst[0] << 8 | dst[1]);
}
