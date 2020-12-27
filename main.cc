// g++ UserspaceI2C_driver.cc -L../ICM20948_driver -L../VL6180X_driver -li2c
#include <cstdio>
#include <unistd.h>

#include "ICM20948_driver.h"
#include "VL6180X_driver.h"
#include "UserSpaceI2C_driver.h"

#define MUX_ADDRESS     0x70
#define VL6180_ADDR     0x29
#define MAG_ADDR        0x0C
#define IMU_ADDR        0x69

#define FORMATING       "| %06d "

int main(int argc, char **argv)
{
    int range_left;
    int range_right;
    IMUDATA imudata;
    MAGDATA magdata;

    // Initialize I2C driver
    UserSpaceI2Cdriver i2c(argv[1], MUX_ADDRESS);

    i2c.AddPort(0,VL6180_ADDR);
    i2c.AddPort(7,VL6180_ADDR);
    i2c.AddPort(4,IMU_ADDR);     // The IMU and magnetometer 
    i2c.AddPort(0x14,MAG_ADDR);  // are connected to the same port
    i2c.SetPort(0);              // Select port 0
    VL6180driver left_obs(&i2c);
    printf("Version: 0x%02x\n", left_obs.GetVersion());
    i2c.SetPort(7);              // Select port 7
    VL6180driver right_obs(&i2c);
    printf("Version: 0x%02x\n", right_obs.GetVersion());
    i2c.SetPort(4);              // Select port 4 device 0 (IMU)
    ICM20948driver imu(&i2c);
    printf("IMU Version: 0x%02x\n", imu.GetIMUVersion());
    i2c.SetPort(0x14);           // Select port 4 device 1 (MAG)
    printf("MAG Version: 0x%02x\n", imu.GetMAGVersion());

    for(int i=0; true; ++i)
    {
        printf("\n|   AX   |   AY   |   AZ   |   GX   |   GY   |   GZ   |   MX   |   MY   |   MZ   |  LTOF  |  RTOF  |\n");
        // Read left VL6180
        i2c.SetPort(0);
        range_left = left_obs.GetRange();
        // Read right VL6180
        i2c.SetPort(7);
        range_right = right_obs.GetRange();
        // Read IMU
        i2c.SetPort(4);              // Select port 4 device 0 (IMU)
        imu.ReadIMU(&imudata);
        // Read MAG
        i2c.SetPort(0x14);           // Select port 4 device 1 (MAG)
        imu.ReadMAG(&magdata);

        printf(FORMATING, (int16_t)imudata.ax);
        printf(FORMATING, (int16_t)imudata.ay);
        printf(FORMATING, (int16_t)imudata.az);
        printf(FORMATING, (int16_t)imudata.gx);
        printf(FORMATING, (int16_t)imudata.gy);
        printf(FORMATING, (int16_t)imudata.gz);
        printf(FORMATING, (int16_t)magdata.mx);
        printf(FORMATING, (int16_t)magdata.my);
        printf(FORMATING, (int16_t)magdata.mz);
        printf(FORMATING, range_left);
        printf(FORMATING, range_right);
        printf("\n");
    }
}


