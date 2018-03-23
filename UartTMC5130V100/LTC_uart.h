#ifndef LTC_UART_H
#define	LTC_UART_H

//#include <Wire.h> 
#include <Arduino.h>
#include <stdint.h>



class LTC_Uart{

public:
	#define uart_buffer_len 200		//100
	void uart_init(void);
	void com_process(void);
	unsigned char read_byte(void);
	void Send_Cmd_Set(uint8_t wr,uint8_t devc_temp,uint8_t addr_temp,uint8_t *data);
	void Tx_Buffer_Write(void);
	 
 // void serialEvent(void);
	uint8_t uart_rcv[10];
	uint8_t Rx_buffer[uart_buffer_len];
	uint8_t Tx_buffer[uart_buffer_len];
	//uint8_t uart_rcv[7];
	uint8_t Special[10];
	uint8_t Rx_write_pt;
	uint8_t Rx_Read_pt;
	uint8_t Tx_write_pt;
	uint8_t Tx_Read_pt;

	  
	uint8_t New_CMD;
	uint8_t uart_rcv_pt;
#define	Host 		0x01
#define	Slave		0x02
private:



//RW
#define READ 		0			
#define WRITE		0x80

//Device/Fun
#define	TMC5130	0x02
#define Slave_addr_Motor_Num 0x07	//slaveaddr uartmode
//SubAddr
	//Address of Registers
#define GCONF		0x00
#define GSTAT		0x01

	//Ramp generator registers
#define RAMPMODE	0x20
#define XACTUAL		0x21
#define VACTUAL		0x22
#define VSTART		0x23
#define A1			0x24
#define V1			0x25	
#define AMAX		0x26
#define VMAX		0x27
#define DMAX		0x28

#define D1			0x2A
#define VSTOP		0X2B
#define TZEROWAIT	0x2C
#define XTARGET		0x2D
	//Ramp generator driver feature control regisgers
#define IHOLD_IRUN	0x30
#define VCOOLTHRS	0x31
#define	VHIGH		0x32
#define	VDCMIN		0x33
#define SW_MODE		0x34
#define RAMP_STAT	0x35
#define XLATCH		0x36
	//Motor driver regisgers


#define CHOPCONF	0x6C
#define	COOLCONF	0x6D
#define DCCTRL		0x6E


	//read
#define DRV_STATUS	0x6F		


//#define INPUT 		0x04
//#define OUTPUT 		0x04



#define X_ACTUAL 	0x21
#define X_Target 	0x2D 



void com_execute(void);
uint8_t SUM(uint8_t *x);


struct
{
	uint8_t	Slave_FM;
	uint8_t	Host_FM;
	uint8_t	WR_FM;
	uint8_t	Devc_FM; 	
	uint8_t	ADD_FM;

	union{
		uint8_t   Data_byte[4];
		uint16_t  Data_word;
		
		}Value;
	
}Snd_Cmd_use; 


struct
{
	uint8_t	Host_FM;
	uint8_t	Slave_FM;
	uint8_t	WR_FM;
	uint8_t	Devc_FM;	
	uint8_t	ADD_FM;	
	
	union{
		uint8_t   Data_byte[4];
		uint32_t  Data_word;
		
		}Value;

}Rcv_Cmd_use;	


};


extern LTC_Uart TMC5130_Uart;


#endif
