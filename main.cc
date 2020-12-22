// g++ UserspaceI2C_driver.cc -L../ICM20948_driver -L../VL6180X_driver -li2c
#include <cstdio>
#include <unistd.h>

//#include "ICM20948_driver.h"
//#include "VL6180X_driver.h"
#include "UserSpaceI2C_driver.h"


#define WriteByte(addr, data)   i2c.WriteWordData(addr>>8, addr&0x00FF, data)


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

WriteByte(0x0207, 0x01);
WriteByte(0x0208, 0x01);
WriteByte(0x0096, 0x00);
WriteByte(0x0097, 0xfd);
WriteByte(0x00e3, 0x01);
WriteByte(0x00e4, 0x03);
WriteByte(0x00e5, 0x02);
WriteByte(0x00e6, 0x01);
WriteByte(0x00e7, 0x03);
WriteByte(0x00f5, 0x02);
WriteByte(0x00d9, 0x05);
WriteByte(0x00db, 0xce);
WriteByte(0x00dc, 0x03);
WriteByte(0x00dd, 0xf8);
WriteByte(0x009f, 0x00);
WriteByte(0x00a3, 0x3c);
WriteByte(0x00b7, 0x00);
WriteByte(0x00bb, 0x3c);
WriteByte(0x00b2, 0x09);
WriteByte(0x00ca, 0x09);
WriteByte(0x0198, 0x01);
WriteByte(0x01b0, 0x17);
WriteByte(0x01ad, 0x00);
WriteByte(0x00ff, 0x05);
WriteByte(0x0100, 0x05);
WriteByte(0x0199, 0x05);
WriteByte(0x01a6, 0x1b);
WriteByte(0x01ac, 0x3e);
WriteByte(0x01a7, 0x1f);
WriteByte(0x0030, 0x00);

// Recommended : Public registers - See data sheet for more detail
WriteByte(0x0011, 0x10); // Enables polling for ‘New Sample ready’
 // when measurement completes
WriteByte(0x010a, 0x30); // Set the averaging sample period
 // (compromise between lower noise and
 // increased execution time)
WriteByte(0x003f, 0x46); // Sets the light and dark gain (upper
 // nibble). Dark gain should not be
 // changed.
WriteByte(0x0031, 0xFF); // sets the # of range measurements after
 // which auto calibration of system is
 // performed
WriteByte(0x0041, 0x63); // Set ALS integration time to 100ms

WriteByte(0x002e, 0x01); // perform a single temperature calibration
 // of the ranging sensor

WriteByte(0x016, 0x00); //change fresh out of set status to 0

    i2c.WriteByteData(0x00, 0x00);

    // |m|m addr(0x29) |m|s| slave     |m|m|
    // |S|0|1|0|1|0|0|1|R|A| data(7:0) |A|P|
    data = i2c.ReadByte();

#if 1
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
