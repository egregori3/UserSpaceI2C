// Linux Userspace Driver by Eric Gregori
// g++ -std=c++11 ICM20948_linux_driver.cc -li2c
// By Eric Gregori
#define ONHARDWARE  0

#include <unistd.h> // usleep
#include <stdio.h>  // printf
#include <vector>   // vector


#if ONHARDWARE
extern "C" {
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}
#endif

#include "UserSpaceI2C_driver.h"


using namespace std;


//
// I2C functions
//

//  S Addr Rd [A] [Data] NA P
uint8_t UserSpaceI2Cdriver::ReadByte(void)
{
    int data = 0;
#if ONHARDWARE
    data = i2c_smbus_read_byte(fp);
    if (data < 0)
    {
        perror("I2C Read operation failed.");
    }
#endif
    printf("ReadByte: S Addr Rd [A] %2x NA P\n", data);
    return (uint8_t)data;
}

//  S Addr Wr [A] first_byte [A] S Addr Rd [A] [Data] NA P -> returns Data
uint8_t UserSpaceI2Cdriver::ReadByteData(uint8_t first_byte)
{
    int data = 0;

#if ONHARDWARE
    data = i2c_smbus_read_byte_data(fp, first_byte);
    if (data < 0)
    {
        perror("I2C Read operation failed.");
    }
#endif
    printf("ReadByteData: S Addr Wr [A] %2x [A] S Addr Rd [A] %2x NA P\n", first_byte, data);
    return (uint8_t)data;
}

// S Addr Wr [A] first_byte [A] second_byte [A] P
int UserSpaceI2Cdriver::WriteByteData(uint8_t first_byte, uint8_t second_byte)
{
    int ret_val = 0;

#if ONHARDWARE
    ret_val = i2c_smbus_write_byte_data(fp,
                                        first_byte,
                                        second_byte);
    if (ret_val < 0)
    {
        perror("I2C Write Operation failed.");
    }
#endif
    printf("WriteByteData: S Addr Wr [A] %2x [A] %2x [A] P\n", first_byte, second_byte);
    return ret_val;
}


UserSpaceI2Cdriver::UserSpaceI2Cdriver(char *PathFilename, uint8_t i2c_add)
{
    int ret_val = 0;

    printf("Opening I2C Driver: %s\n", PathFilename);
#if ONHARDWARE
    fp = open(PathFilename, O_RDWR);
    if (fp < 0)
    {
        perror("Unable to open device node.");
    }

    // Set I2C_SLAVE address */
    ret_val = ioctl(fp,I2C_SLAVE,i2c_add);
    if (ret_val < 0)
    {
        perror("Could not set I2C_SLAVE.");
    }
#endif

    initialized = true;
}

UserSpaceI2Cdriver::~UserSpaceI2Cdriver()
{
    close(fp);
}

//
// Data functions
//
// Write a vector of commands
int UserSpaceI2Cdriver::WriteCommands(vector<vector<WRITE> >cmd)
{
    for(int i=0; i<cmd.size(); ++i)
        WriteRegisters(cmd[i]);
    return 0;
}

// Write a vector of registers
int UserSpaceI2Cdriver::WriteRegisters(vector<WRITE> registers)
{
    for(int i=0; i<registers.size(); ++i)
    {
        if((registers[i].addr & NONI2C) == 0)
            WriteByteData(registers[i].addr, registers[i].data);
        else
        {
            switch(registers[i].addr)
            {
                case SLEEP:
                    printf("SLEEP -> %d\n", registers[i].data);
                    usleep(registers[i].data*1000);
                    break;
            }
        }
    }
    return 0;
}

//
// MUX Functions
//
int UserSpaceI2Cdriver::SetPort(int port)
{
    for(int i=0; i<ports.size(); ++i)
    {
        if(ports[i].id == port)
        {
            // Set port
            printf("Set port to %d", ports[i].addr);
            break;
        }
    }
}

int UserSpaceI2Cdriver::AddPort(int port, uint8_t addr)
{
    PORT p = { .id=port, .addr=addr };
    ports.push_back(p);
}




