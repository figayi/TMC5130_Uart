#include "LTC_UART.h"
#include "SingleWireTMC5130.h"



LTC_Uart TMC5130_Uart;


void LTC_Uart::uart_init(void)
{
	

	Tx_write_pt	=	0;
	Tx_Read_pt	=	0;
	Rx_write_pt	=0;
	Rx_Read_pt	=0;
	uart_rcv_pt=0;

 
	New_CMD=0;

}

unsigned char LTC_Uart::read_byte(void)
{
	unsigned char temp;
	temp= Rx_buffer[Rx_Read_pt++];
	if(Rx_Read_pt>=uart_buffer_len)		//uart_buffer_len 100
		Rx_Read_pt=0;
	return(temp);
	
}
 


uint8_t LTC_Uart::SUM(uint8_t *x)
{
	uint8_t temp_sum,i;
	temp_sum=0;
	for(i=0;i<9;i++)
	{
		temp_sum+=*(x+i);
	}
	return(temp_sum);
}
void LTC_Uart::com_process(void)
{

	unsigned char temp;
 	
	if(Rx_Read_pt!=Rx_write_pt)
	{	
//		Uart_Timeout_cmd_Flag_Clear();	
		if(uart_rcv_pt==0)
		{
			temp=read_byte();
			if(temp==Host)
				{  
				uart_rcv[uart_rcv_pt++]=temp;
				}
		}
		else if(uart_rcv_pt<10)
			{
				uart_rcv[uart_rcv_pt++]=read_byte();
			}
		if(uart_rcv_pt==10)
			{
			uart_rcv_pt=0;
				if((uart_rcv[1]==Slave)&&
					(uart_rcv[9]==SUM(&uart_rcv[0])))
					{
						com_execute();		
					}
			}
	}

}
void LTC_Uart::com_execute(void)
{
	uint8_t i;
	uint8_t Reg;
	uint8_t buffer_temp[5];
	uint8_t crc_temp;
 
//	New_CMD=0;
	Rcv_Cmd_use.WR_FM	=	uart_rcv[2];

	Rcv_Cmd_use.Devc_FM		=	uart_rcv[3];
	Rcv_Cmd_use.ADD_FM	=	uart_rcv[4];
	Rcv_Cmd_use.Value.Data_byte[0]=	uart_rcv[5];	//MSB?
	Rcv_Cmd_use.Value.Data_byte[1]=	uart_rcv[6];
	Rcv_Cmd_use.Value.Data_byte[2]=	uart_rcv[7];
	Rcv_Cmd_use.Value.Data_byte[3]=	uart_rcv[8];
	//void Send_Cmd_Set(unsigned char devc_temp,unsigned char addr_temp,unsigned char wr,unsigned char data)
	Send_Cmd_Set(Rcv_Cmd_use.Devc_FM,Rcv_Cmd_use.ADD_FM,Rcv_Cmd_use.WR_FM,&Rcv_Cmd_use.Value.Data_byte[0]);

	switch(Rcv_Cmd_use.Devc_FM)
	{	
		case Slave_addr_Motor_Num:
			{
	//		TMC5130_Uart.Motor_Num_Sel=Rcv_Cmd_use.Value.Data_byte[3];
			TMC5130_SW.Slave_addr=Rcv_Cmd_use.Value.Data_byte[3];
			Send_Cmd_Set((Rcv_Cmd_use.ADD_FM&0x7F),Slave_addr_Motor_Num,Rcv_Cmd_use.WR_FM,&Rcv_Cmd_use.Value.Data_byte[0]);
			
			Tx_Buffer_Write();
			
			}break;
		case TMC5130:
			{
				
			Reg=Rcv_Cmd_use.ADD_FM;
			
	//		TMC5130_Uart.ReadWrite_5130(Rcv_Cmd_use.WR_FM,Rcv_Cmd_use.ADD_FM, \
	//			&(Rcv_Cmd_use.Value.Data_byte[0]),buffer_temp);
			if((Reg&WRITE)==READ)
				{
				//Read
					crc_temp=TMC5130_SW.Read5130(TMC5130_SW.Slave_addr,Reg,buffer_temp);
				//if(0==TMC5130_SW.Read5130(0x01,Rcv_Cmd_use.ADD_FM,buffer_temp))//0x01 could be replace by Motor_Num
				if(TMC5130_SW.Crc_Error!=crc_temp)
					{
					Send_Cmd_Set((Rcv_Cmd_use.ADD_FM&0x7F),TMC5130,crc_temp,buffer_temp);	//0xab=>0x00
				
					Tx_Buffer_Write();
					}
				}
			else if((Reg&WRITE)==WRITE)
				{
				//Write
				TMC5130_SW.Write5130(TMC5130_SW.Slave_addr,Reg,&(Rcv_Cmd_use.Value.Data_byte[0]));
				}
			}break;
	
		default:	 				//read Main_adc
		{
		}break;
	}
			
}	

void LTC_Uart::Send_Cmd_Set(uint8_t wr,uint8_t devc_temp,uint8_t addr_temp,uint8_t *data)
{

	unsigned char i;
	
	Snd_Cmd_use.Slave_FM=		Slave;
	Snd_Cmd_use.Host_FM= 		Host;
	Snd_Cmd_use.WR_FM= 			wr;
	Snd_Cmd_use.Devc_FM= 		devc_temp;
	Snd_Cmd_use.ADD_FM= 		addr_temp;
	
	Snd_Cmd_use.Value.Data_byte[0]=		*data;
	Snd_Cmd_use.Value.Data_byte[1]=		*(data+1);
	Snd_Cmd_use.Value.Data_byte[2]=		*(data+2);
	Snd_Cmd_use.Value.Data_byte[3]=		*(data+3);

}
void LTC_Uart::Tx_Buffer_Write(void)
{
	uint8_t i,sum;
	sum=0;
	Tx_write_pt=0;
	Tx_buffer[Tx_write_pt++]=	Snd_Cmd_use.Slave_FM;
	Tx_buffer[Tx_write_pt++]=	Snd_Cmd_use.Host_FM;
	Tx_buffer[Tx_write_pt++]=	Snd_Cmd_use.WR_FM;
	Tx_buffer[Tx_write_pt++]=	Snd_Cmd_use.Devc_FM;
	Tx_buffer[Tx_write_pt++]=	Snd_Cmd_use.ADD_FM;	
	Tx_buffer[Tx_write_pt++]=  Snd_Cmd_use.Value.Data_byte[0];
  	Tx_buffer[Tx_write_pt++]= Snd_Cmd_use.Value.Data_byte[1];
	Tx_buffer[Tx_write_pt++]= Snd_Cmd_use.Value.Data_byte[2];
	Tx_buffer[Tx_write_pt++]= Snd_Cmd_use.Value.Data_byte[3];
	sum= Snd_Cmd_use.Slave_FM+Snd_Cmd_use.Host_FM+Snd_Cmd_use.Devc_FM+Snd_Cmd_use.ADD_FM+Snd_Cmd_use.WR_FM+\
		Snd_Cmd_use.Value.Data_byte[0]+Snd_Cmd_use.Value.Data_byte[1]+Snd_Cmd_use.Value.Data_byte[2]+Snd_Cmd_use.Value.Data_byte[3];
	Tx_buffer[Tx_write_pt++]=sum;
	for(i=0;i<10;i++)
//		Serial.print(Tx_buffer[i],HEX);
//		Serial.print(Tx_buffer[i]);
   		Serial.write(Tx_buffer[i]);

}





