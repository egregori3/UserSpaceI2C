#ifndef USERSPACEI2CDRIVER
#define USERSPACEI2CDRIVER

#include <cstdint>
#include <vector>


// Non I2C sequences - Overload the address
#define    NONI2C             0x80
#define    SLEEP              (NONI2C+0x01)


// Data Structures$
typedef struct
{
    uint8_t addr;
    uint8_t data;
}   WRITE;

typedef struct
{
    int     id;
    uint8_t addr;
}   PORT;

class UserSpaceI2Cdriver
{
    private:
        int     fp;
        bool    initialized;
        std::vector<PORT> ports;

        int WriteRegisters(std::vector<WRITE>);

    public:
        // addr = the address of the MUX
        UserSpaceI2Cdriver(char *, uint8_t addr);
        ~UserSpaceI2Cdriver();
        int GetIntialized(void) { return initialized; }

        //
        // Data functions
        //
        int WriteCommands(std::vector<std::vector<WRITE> >);

        //
        // I2C funcions
        //
        // S Addr Rd [A] [Data] NA P
        uint8_t ReadByte(void);

        // S Addr Wr [A] first_byte [A] S Addr Rd [A] [Data] NA P
        uint8_t ReadByteData(uint8_t first_byte);

        // S Addr Wr [A] first_byte [A] second_byte [A] P
        int WriteByteData(uint8_t first_byte, uint8_t second_byte);

        //
        // MUX control functions
        //
        int AddPort(int port, uint8_t addr);
        int SetPort(int port);
};

#endif
