#include <Wire.h>



#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <stdint.h>


/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library

#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;


#include "logo.h"
//#include "LTClogo256.h"
//#include "Linear_logo2.h"
//#include "Linear_logo2_16bit.h"
#include "Trinamic_Logo3.h"





#include "LTC_UART.h"



//I2C_EEProm EE_I2C;



// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0xC638    // 0x18 << 11 | 0x31 << 5 | 0x18

//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#include <Wire.h>

const int CONVST_PIN = 75;  //PA26
const int CS0_PIN_OUT = 10;    //PC27
const int CS0_PIN_IN = 77;    //PA28
//const int TEST_PIN_OUT = 21;   //PB13
unsigned int loop_count;
unsigned long systicks, pretms;
unsigned char bupdate = 0;
unsigned char LedState = 0;


const unsigned char adiComponent[7] =
{
  0xFE, 0xFE, 0x82, 0xC6, 0xEE, 0xFE, 0xFE
};

// the setup function runs once when you press reset or power the board
char test_n;
char RSense_R_EE;
uint16_t ACCR_bottom_EE;

//#define Status		
#define	ERROR		1
#define CORRECT		0


#include "SingleWireTMC5130.h"


void setup() {
  // initialize digital pin 13 as an output.

  Serial.begin(9600);


	Serial1.begin (19200);
// Serial1.begin (9600);
//	Serial1.begin (38400);
 // Serial1.begin (4800);

  delay(500);

 // pinMode(21, OUTPUT);
  //pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  
 //	digitalWrite(10, HIGH);
//	digitalWrite(21, HIGH);
//	SPI.begin();															 /////////
//	SPI.setDataMode(SPI_MODE3);
//	SPI.setClockDivider(250);//84





  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);
  tft.fillScreen(BLACK);

  int i, j;
  for(i = 0; i< 80; i ++){
    for(j = 0; j < 240; j ++){
      tft.drawPixel(j, i, excelpoint_logo_rgb_5[j + i*240]);
    }
  }


  for(i = 0; i< 22; i ++){
    for(j = 0; j < 30; j ++){

		loop_count = Trinamic_Logo3[i*30+j];
			tft.drawPixel(j, 290+i, loop_count);
    }
  }
  
  tft.setCursor(40,300);
  tft.setTextColor(GRAY);
  tft.print("TMC5130 SUB Board");
  
  tft.setCursor(0,100);
  tft.fillRect(0,100,240,110,BLACK);
  tft.setTextColor(BLUE);    tft.setTextSize(2);
  //tft.println("Tempture [^C]:");
  tft.println("Charge Time:");
  tft.println();

//  tft.println("Voltage [V]:");
   tft.println("Backup Time:");
  tft.println();

//  tft.println("Current [A]:");
   tft.println("To Be Defined");
  tft.println();

 // tft.println("Coulombs [C]:");
   tft.println("Motor_Num");
	tft.println();
	tft.println();

  tft.print("Running ");

  loop_count = 0;
  pretms = millis();

}

byte x,x1,temp_eew_times = 0;
boolean stringComplete = false;  // whether the string is complete
String inputString = "";         // a string to hold incoming data

void loop() {
  unsigned int tmp;
  uint8_t tmp_Rx_buffer[20];
  uint8_t status,i;
  systicks = millis();

  TMC5130_Uart.com_process();			//uart usr
  if((systicks - pretms) >= 1000){
    char strbuf[128];
//	LTC_U8_to_Flt(); 			// READ ALL LTC REG & PRE_CAL TO FLOAT DATAS
	


	tft.setCursor(170,100);
		  tft.fillRect(170,100,60,20,BLACK);	
		  tft.setTextColor(CYAN);	
//		  tft.println(LTC_C_Degree.flt_C_Degree);
		tft.println("TBD");

	
	tft.setCursor(160,130);
		  tft.fillRect(160,130,60,20,BLACK);	
		  tft.setTextColor(CYAN);	
//		  tft.println(LTC_Voltage.flt_Voltage);
		tft.println("TBD");

	
	  tft.setCursor(160,160);
      tft.fillRect(160,160,60,20,BLACK);  
      tft.setTextColor(CYAN);	
//      tft.println(LTC_Current.flt_Current);

	tft.setCursor(0,220);
      tft.fillRect(0,220,100,20,BLACK);  
      tft.setTextColor(CYAN);	
  //    tft.println(TMC5130_Uart.Motor_Num_Sel);		//TBD slaveaddr
  tft.println(TMC5130_SW.Slave_addr);
	


	
    pretms = systicks;    
    tft.setCursor(100,250);
    tft.fillRect(100,250,100,20,BLACK);
    tft.setTextColor(WHITE);
    tmp = loop_count/3600;
    if(tmp > 0){
      sprintf(strbuf, "%d:%.2d:%.2d",tmp, (loop_count/60)%60, loop_count%60);
    }
    else{
      sprintf(strbuf, "%.2d:%.2d",(loop_count/60)%60, loop_count%60);
    }
    tft.println(strbuf);
    loop_count ++;
  }
  
  if(((systicks - pretms)%700) > 300){
    if(bupdate == 1){
  }
      bupdate = 0;
      digitalWrite(13, LOW);
	//digitalWrite(10, LOW);
	//digitalWrite(21, LOW);
    }
  
  else{
    if(bupdate == 0){

  bupdate = 1;
  digitalWrite(13, HIGH);

  }



	//digitalWrite(10, HIGH);
	//digitalWrite(21, HIGH);
//	delay(5);
//  	delay(5);


  }
    

}


void serialEvent(void) {
	  unsigned char temp;
	while(Serial.available())
	  {   
	  
		  temp=(unsigned char)Serial.read();
		  TMC5130_Uart.Rx_buffer[TMC5130_Uart.Rx_write_pt++]=temp;
		  if(TMC5130_Uart.Rx_write_pt==uart_buffer_len) 	  //uart_buffer_len 100
				  TMC5130_Uart.Rx_write_pt=0;
	  }

}


