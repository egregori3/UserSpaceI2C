// g++ UserspaceI2C_driver.cc -L../ICM20948_driver -L../VL6180X_driver -li2c
#include <cstdio>
#include <unistd.h>

//#include "ICM20948_driver.h"
#include "VL6180X_driver.h"
#include "UserSpaceI2C_driver.h"



int main(int argc, char **argv)
{
    uint8_t data;

    // Initialize I2C driver
    UserSpaceI2Cdriver i2c(argv[1], 0x29);

    // Read VL6180 ID register (0x00, 0x00)
    // |m|m addr(0x29) |m|s| master      |s| master     |s|m|
    // |S|0|1|0|1|0|0|1|R|A| index(15:8) |A| index(7:0) |A|P|
    i2c.WriteByteData(0x00, 0x00);

    // |m|m addr(0x29) |m|s| slave     |m|m|
    // |S|0|1|0|1|0|0|1|R|A| data(7:0) |A|P|
    data = i2c.ReadByte();
    printf("Version: 0x%02x\n", data);

    VL6180driver left_obs(&i2c);
    printf("Version: 0x%02x\n", left_obs.GetVersion());

    while(1)
    {
        int range = left_obs.GetRange();
        if(range < 0)
            perror("VL6180 driver returned an error\n");
        else
            printf("Range: %d\n", range);
        sleep(1);
    }
#if 0
while(1)
{
    uint8_t range;

    usleep(50000);
    WriteByte(0x018,0x01); // Start Range
    usleep(50000);

    i2c.WriteByteData(0x00, 0x62);
    range = i2c.ReadByte();

    printf( "Range: %d\n", range);

    WriteByte(0x015,0x07);
}
#endif
}

#if 0
    i2c.AddPort(L_OBST,0x53); // obstruction sensor
    i2c.AddPort(R_OBST,0x53); // obstruction sensor
    i2c.AddPort(IMU,0x53); // imu

    // Initialize sensor drivers
    i2c.SetPort(IMU);
    ICM20948driver imu(&i2c);
    i2c.SetPort(L_OBST);
    VL6180driver left_obs(&i2c);
    i2c.SetPort(R_OBST);
    VL6180driver right_obs(&i2c);

    // Display sensor data
    DATA x;
    imu.ReadIMU(&x);
    imu.DumpData(&x);
    printf("Left Range: %d",  left_obs.GetRange());
    printf("Right Range: %d", right_obs.GetRange());
#endif
