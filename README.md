# Linux Userspace I2C Driver Using I2C_RDWR and i2c-dev by Eric Gregori

A c++ Linux userspace I2C driver designed for adding sensors to
a PI based robot.

# I2C Bus
[Understanding the I2C Bus](https://www.ti.com/lit/an/slva704/slva704.pdf)

The I2C bus is an addressable 2-wire multi-drop bidirectional master/slave bus.
The two wires are data (SDA) and clock (SCL) with a clock rate up to 400KHz. 
Both wires are pulled-up using bus resistors. 
The master can pull either wire to ground. The master generates the clock (SCL).
Slaves cannot pull-down the SCL wire but they can pull down the SDA wire.

Special attention needs to be paid to the pull-up resistors.
Improper pull-ups is the primary cause of I2C bus issues.

The bus does support multiple masters, but this mode is not used very often.

Transfers are normally done in 8 bit chunks controlled by the master.
Each 8 bit transfer is replied with either an ACK or NACK signal.

A single clock pulse is a L->H->L. The SDA bus transitions while the SCL wire
in low (with the exception of a STOP condition).

## Condition

In addition to data and clock the two wires are used to signal conditions.
The master sends a START signal to take ownership of the bus and sends a STOP
signal to give up ownership of the bus. When a master takes ownership of the 
bus it is a signal to other masters to be quiet.

### START (S)

Master only
The master initiates communication by sending a START condition by holding
SCL high and transitioning SDA from high to low (the idle state of the bus is high).
SCL = H, SDA = H->L

### STOP (P)

Master only
The master signals a stop condition by holding SCL high and transitioning SDA
from low to high.
SCL = H, SDA = L->H

### ACK (A)

Master or Slave
If after 8 clock pulses (L->H->L), the SDA line is held low during the 9th clock,
this is interpreted as an ACK. 

### NACK (N)

Master or Slave
If after 8 clock pulses (L->H->L), the SDA line is held high during the 9the clock,
this is interpreted as a NACK.

## Addressing

Master only
The master addresses a device with the first 8 clock pulses after sending a START
condition. The first 7 pulses represent the address with the 8th pulse representing
a read/write signal to the slave. The address is 7 bits. Each slave must have a unique
address.
Read = 1, Write = 0

## Example SMBUS protocol
https://www.kernel.org/doc/html/latest/i2c/smbus-protocol.html

### Write
|m|  master     |m|s|  master              |s| master   |s|m|
|S|7 bit address|W|A|8 bit register address|A|8 bit data|A|P|

### Read (Repeated Start)
The master will continue sending out the clock pulses, but will release the SDA line, so that the slave can
transmit data. At the end of every byte of data, the master will send an ACK to the slave, letting the slave
know that it is ready for more data. Once the master has received the number of bytes it is expecting, it
will send a NACK, signaling to the slave to halt communications and release the bus. The master will
follow this up with a STOP condition.

|m| master      |m|s| master               |s|m| master      |m|s| slave    |m|m|
|S|7 bit address|W|A|8 bit register address|A|S|7 bit address|R|A|8 bit data|N|P|

##  i2c_smbus_write_word_data()

|S|7 bit address|W|A|8 bit command|8 bit data low|A|8 bit data high|A|P|

## i2c_smbus_write_byte_data()

|S|7 bit address|W|A|8 bit command|A|8 bit data|A|P|

## i2c_smbus_read_byte()

|S|7 bit address|R|A|8 bit data from device|N|P|

## Example VL6180 protocol (2 byte address)
https://www.st.com/resource/en/datasheet/vl6180x.pdf

### Write
|m|m addr(0x29) |m|s| master      |s| master     |s| master    |s|m|
|S|0|1|0|1|0|0|1|W|A| index(15:8) |A| index(7:0) |A| data(7:0) |A|P|

i2c_smbus_write_word_data(command=index(15:8), dlow=index(7:0), dhigh=data)

### Read
|m|m addr(0x29) |m|s| master      |s| master     |s|m|
|S|0|1|0|1|0|0|1|R|A| index(15:8) |A| index(7:0) |A|P|

i2c_smbus_write_byte_data(command=index(15:8), data=index(7:0))

|m|m addr(0x29) |m|s| slave     |m|m|
|S|0|1|0|1|0|0|1|R|A| data(7:0) |A|P|

i2c_smbus_read_byte()

## Example TCA9548A I2C Mux
https://www.ti.com/lit/ds/symlink/tca9548a.pdf

### Write
|m|  master     |m|s|  master              |s| master   |s|m|
|S|7 bit address|W|A|8 bit register address|A|8 bit data|A|P|

i2c_smbus_write_byte_data(command=register address, data=data)


# I2C

https://www.kernel.org/doc/Documentation/i2c/dev-interface



# Installation

sudo apt-get install -y libi2c-dev


# Building

When building C++ apps, the i2c headers need to be wrapped by extern "C".
extern "C" {
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

Of-course you need to include -li2c in your g++ command line.
g++ -std=c++11 UserspaceI2C_driver.cc -li2c
g++ UserspaceI2C_driver.cc -I./ -I../ICM20948_driver -I../VL6180X_driver
g++ -std=c++11 UserSpaceI2C_driver.cc main.cc -li2c

# Testing

```
ls /dev/ | grep "i2c"
   i2c-1
i2cdetect -l
i2c-1    unknown   bcm2835 (i2c@7e804000)          N/A
```



