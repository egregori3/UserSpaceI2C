// g++ UserspaceI2C_driver.cc -L../ICM20948_driver -L../VL6180X_driver -li2c
#include <cstdio>
#include <unistd.h>

//#include "ICM20948_driver.h"
#include "VL6180X_driver.h"
#include "UserSpaceI2C_driver.h"

#define MUX_ADDRESS     0x70
#define VL6180_ADDR     0x29

int main(int argc, char **argv)
{
    uint8_t data;

    // Initialize I2C driver
    UserSpaceI2Cdriver i2c(argv[1], MUX_ADDRESS);

    i2c.AddPort(0,VL6180_ADDR);
    i2c.AddPort(7,VL6180_ADDR);
    i2c.SetPort(0);
    VL6180driver left_obs(&i2c);
    printf("Version: 0x%02x\n", left_obs.GetVersion());
    i2c.SetPort(7);
    VL6180driver right_obs(&i2c);
    printf("Version: 0x%02x\n", right_obs.GetVersion());


    while(1)
    {
        int range_left, range_right;

        i2c.SetPort(0);
        range_left = left_obs.GetRange();
        i2c.SetPort(7);
        range_right = right_obs.GetRange();
        printf("Range: %d, %d\n", range_left, range_right);
    }
}


