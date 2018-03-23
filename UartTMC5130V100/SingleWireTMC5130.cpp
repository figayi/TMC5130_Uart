#include "SingleWireTMC5130.h"

#define Debug_SWTMC5130

TMC_SingleWire TMC5130_SW;



void	TMC_SingleWire::delay_n1(unsigned int n)
{
 
	unsigned int i,j;
	for(i=n;i>0;i--)
		{
			for(j=200;j>0;j--);		
		}	
		
}
uint8_t	TMC_SingleWire:: Read5130(uint8_t Slave_Addr,uint8_t Reg,uint8_t *readbuf)
{
		uint8_t CRC = 0, i ;
		unsigned char buf [8];//8
		unsigned long dataBytes ;
		uint8_t CRC_TEMP,CRC_buf;
		Reg|=TMC_Read;

	
		buf [0] = Slave_Sync_TMC5130 ;
		buf [1] = Slave_Addr ;
		buf [2] = Reg ;
		//buf [3] = CRC8_ATM(buf,4) ;
		CRC8_ATM(buf,4) ;
		while(Serial1.read() >= 0);
		Serial1.write (buf , 4); // write datagram
		Serial1.flush (); // wait until all datas are written
		Serial1.readBytes (buf , 4); // clear input buffer
		Serial1.readBytes (buf , 8);
		CRC_buf=buf [7];
		CRC_TEMP=CRC8_ATM(buf,8);

	//	if(CRC_TEMP==CRC_buf||Reg==0x04||Reg==0x6F)	//crc check ok	//fix crc bug for Reg 0x04 
		if(CRC_TEMP==CRC_buf)
			{
				for(i=0;i<4;i++)
					*(readbuf+i)=*(buf+3+i);
				return(Crc_Correct);
				/*
				if(Reg==0x04||Reg==0x6F)
					return(CRC_buf);
				else
					return(Crc_Correct);
				*/
				/*
				if(CRC_TEMP==CRC_buf)
					return(Crc_Correct);
				else
					return(CRC_buf);
				*/
			}
		return(Crc_Error);
}


void	TMC_SingleWire::Write5130(uint8_t Slave_Addr,uint8_t Reg,uint8_t *sendbuf)
{
		uint8_t CRC = 0, i ;
		unsigned char buf [8];//8
		unsigned long dataBytes ;
		uint8_t CRC_TEMP,CRC_buf;
		Reg|=TMC_Write;

	
		buf [0] = Slave_Sync_TMC5130 ;
		buf [1] = Slave_Addr ;
		buf [2] = Reg ;
		//buf [3] = CRC8_ATM(buf,4) ;
		for(i=0;i<4;i++)
			*(buf+3+i)=*(sendbuf+i);
	
		CRC8_ATM(buf,8) ;
		while(Serial1.read() >= 0);
		Serial1.write (buf , 8); // write datagram
		Serial1.flush (); // wait until all datas are written
		Serial1.readBytes (buf , 8);
}



uint8_t TMC_SingleWire::CRC8_ATM(uint8_t* datagram, uint8_t datagramLength)
{
	int i,j;
	uint8_t* crc = datagram + (datagramLength-1); // CRC located in last byte of message
	uint8_t currentByte;
	*crc = 0;
	for (i=0; i<(datagramLength-1); i++) 
	{ // Execute for all bytes of a message
		currentByte = datagram[i]; // Retrieve a byte to be sent from Array
		for (j=0; j<8; j++) 
			{
			if ((*crc >> 7) ^ (currentByte&0x01)) // update CRC based result of XOR operation
				{
				*crc = (*crc << 1) ^ 0x07;
				}
			else
				{
				*crc = (*crc << 1);
				}
			currentByte = currentByte >> 1;
			} // for CRC bit
	} // for message byte
	return(*crc);
}



