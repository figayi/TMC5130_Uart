#ifndef __SINGLEWIRETMC5130_H
#define	__SINGLEWIRETMC5130_H


#include <Arduino.h>
#include <stdint.h>


class TMC_SingleWire{
public:

	uint8_t Slave_addr;
	void ReadWrite_5130(uint8_t RW, uint8_t Slave_Addr,uint8_t Reg,uint8_t *sendbuf,uint8_t *readbuf); 
	
	uint8_t Read5130(uint8_t Slave_Addr,uint8_t Reg,uint8_t *readbuf);
	void Write5130(uint8_t Slave_Addr,uint8_t Reg,uint8_t *sendbuf);
#define CRC8_GEN  0x07
#define Slave_Sync_TMC5130 0x05

enum CRC_Error
{
	Crc_Correct,
	Crc_Error=0xff,
};
	
private:


	void delay_n1(unsigned int n);
	uint8_t CRC8_ATM(uint8_t* datagram, uint8_t datagramLength);
	

#define uint8 uint8_t

uint8 NextCRCSingle ( uint8 Crc , uint8 Data , uint8 Gen , uint8 Bit );
uint8 NextCRC ( uint8 Crc , uint8 Data , uint8 Gen );


#define TMC_Write  	0X80
#define TMC_Read	0


};

extern TMC_SingleWire TMC5130_SW;

#endif
