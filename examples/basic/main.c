#include <stdio.h>
#include "pico/stdlib.h"

#include "mpu6050.h"

#define SDA_GPIO    4
#define SCL_GPIO    5

int main(void)
{  
    stdio_init_all();
    sleep_ms(1000);

    printf("Inicializando I2C...\n");

    // Inicializacion de I2C
    i2c_init(i2c0, 400000);
    gpio_set_function(SDA_GPIO, GPIO_FUNC_I2C);
    gpio_set_function(SCL_GPIO, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_GPIO);
    gpio_pull_up(SCL_GPIO);

    printf("Inicializando MPU6050...\n");

    // Inicializacion de MPU6050
    mpu6050_init(i2c0, 0x68);
    sleep_ms(2000);

    printf("Who am I = 0x%2x\n", mpu6050_who_am_i());

    while (true) {
        // Estructura para datos de aceleracion
        mpu_accel_t accel = {0};
        mpu_gyro_t gyro = {0};
        int16_t temp = 0;
        // Leo MPU
        mpu6050_read_accel(&accel);
        mpu6050_read_gyro(&gyro);
        mpu6050_read_temp(&temp);
        // Muestro
        printf("Ax = %d, Ay = %d, Az = %d\n", accel.accel_x, accel.accel_y, accel.accel_z);
        printf("Gx = %d, Gy = %d, Gz = %d\n", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
        printf("Temp = %d\n", temp);
        sleep_ms(500);
    }
}
