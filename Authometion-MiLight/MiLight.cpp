/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/*  * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Code by Pietro Moscetta
 http://www.authometion.com
 Version 1.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>
#include "MiLight.h"

/* Initialization function */
void MiLight::initialize(byte csPin)
{
  _csPin=csPin;
  ADDR_A=1;
  ADDR_B=1;
  PAR=1;
  COMMAND=0;
  MODE=0;
  PACK=0;

  // Default radio transmission number for MiLight/Limitlessled bulbs
  radioTransmissionNumber=DEFAULT_RADIO_TRASMISSION;
  
   
  pinMode(_csPin, OUTPUT);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV64);    // UNO and 328
  //SPI.setClockDivider(SPI_CLOCK_DIV128); // MEGA and FASTER
  SPI.setBitOrder(MSBFIRST);
  // Initialize SPI:
  SPI.begin();

  //Initialize PL1167 transceiver
  initRadioModule();   
  delay(50);
  RGBWAndWhiteSendHeadPacket();
  delayMicroseconds(5);
    
  //Reset SPI MODE to default
  SPI.setDataMode(SPI_MODE0);  
}

/**
  * LYT COMMANDS
  *
  */

/**
  * description  Set the number of radio transmission for LYT
  * param        Val           : number of radio transmission repetition
  * retval       none
  */
void MiLight::setRadioTransmissionNumber(int Val)
{
  radioTransmissionNumber=Val;
}

/**
  * description  read the number of radio transmission for LYT
  * param        none
  * retval       intger        : number of actual radio transmission number
  */
int MiLight::setRadioTransmissionNumber(void)
{
  return radioTransmissionNumber;
}

/**
  * description  Set the number of radio transmission for LYT to DEFAULT_RADIO_TRASMISSION
  * param        none
  * retval       none
  */
void MiLight::setDefaultRadioTransmissionNumber(void)
{
  radioTransmissionNumber=DEFAULT_RADIO_TRASMISSION;
}

/**
  * description  Set the two address byte of RGBW bulbs
  * param        ADDRA         : new address A value
  * param        ADDRB         : new address B value
  * retval       none
  */
void MiLight::RGBWSetAddress(byte ADDRA, byte ADDRB)
{
  int radioTransmissionNumberTemp;

  radioTransmissionNumberTemp=radioTransmissionNumber;
  radioTransmissionNumber=500;
  RGBWAndWhiteSendCommand(0, ADDRA, ADDRB, 3, 1, 1, 1);
  radioTransmissionNumber=radioTransmissionNumberTemp;
}

/**
  * description  Set the two address byte of White bulbs
  * param        ADDRA         : new address A value
  * param        ADDRB         : new address A value
  * retval       none
  */
void MiLight::WhiteSetAddress(byte ADDRA, byte ADDRB)
{
  int radioTransmissionNumberTemp;

  radioTransmissionNumberTemp=radioTransmissionNumber;
  radioTransmissionNumber=500;
  RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 8, 1, 1, 1);
  radioTransmissionNumber=radioTransmissionNumberTemp;
}

/**
  * description  Reset the two address byte of RGBW bulbs
  * param        ADDRA         : address A of bulb to be reset 
  * param        ADDRB         : address B of bulb to be reset
  * retval       none
  */
void MiLight::RGBWClearAddress(byte ADDRA, byte ADDRB)
{
  int radioTransmissionNumberTemp;

  radioTransmissionNumberTemp=radioTransmissionNumber;
  radioTransmissionNumber=500;
  //SEND 3 SHORT DEL COMMANDS + 2 LONG DEL COMMANDS
  for(int i=0; i<5; i++)
  {
    if(i>=0 && i<3)
      RGBWAndWhiteSendCommand(0, ADDRA, ADDRB, 3, 1, 1, i+1);
    if(i>2)
      RGBWAndWhiteSendCommand(0, ADDRA, ADDRB, 19, 1, 1, i+1); 
    delay(100);
  }
  radioTransmissionNumber=radioTransmissionNumberTemp;
}

/**
  * description  Reset the two address byte of White bulbs
  * param        ADDRA         : address A of bulb to be reset 
  * param        ADDRB         : address B of bulb to be reset
  * retval       none
  */
void MiLight::WhiteClearAddress(byte ADDRA, byte ADDRB)
{
  int radioTransmissionNumberTemp;

  radioTransmissionNumberTemp=radioTransmissionNumber;
  radioTransmissionNumber=100;
  //SEND 5 SHORT DEL COMMANDS
  for(int i=0; i<5; i++)
  {
    RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 8, 1, i+1, 1);
    delay(100);
  }
  radioTransmissionNumber=radioTransmissionNumberTemp;
}

/**
  * description  Switch ON for RGBW bulbs
  * param        ADDRA         : address A of bulb to be switch ON 
  * param        ADDRB         : address B of bulb to be switch ON
  * retval       none
  */
void MiLight::RGBWOn(byte ADDRA, byte ADDRB)
{ 
  RGBWAndWhiteSendCommand(0, ADDRA, ADDRB, 3, 1, 1, 1);
}

/**
  * description  Switch ON for White bulbs
  * param        ADDRA         : address A of bulb to be switch ON 
  * param        ADDRB         : address B of bulb to be switch ON
  * retval       none
  */
void MiLight::WhiteOn(byte ADDRA, byte ADDRB)
{
  RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 8, 1, 1, 1);
}

/**
  * description  Switch OFF for RGBW bulbs
  * param        ADDRA         : address A of bulb to be switch OFF 
  * param        ADDRB         : address B of bulb to be switch OFF
  * retval       none
  */
void MiLight::RGBWOff(byte ADDRA, byte ADDRB)
{
  RGBWAndWhiteSendCommand(0, ADDRA, ADDRB, 4, 1, 1, 1);
}

/**
  * description  Switch OFF for White bulbs
  * param        ADDRA         : address A of bulb to be switch OFF 
  * param        ADDRB         : address B of bulb to be switch OFF
  * retval       none
  */
void MiLight::WhiteOff(byte ADDRA, byte ADDRB)
{
  RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 11, 1, 1, 1);
}

/**
  * description  Set RBG values for RGBW bulbs
  * param        ADDRA         : address A of bulb to be set 
  * param        ADDRB         : address B of bulb to be set
  * param        VALUE         : color value
  * retval       none
  */
void MiLight::RGBWSetRGBValue(byte ADDRA, byte ADDRB, byte VALUE)
{
  RGBWAndWhiteSendCommand(0, ADDRA, ADDRB, 15, VALUE, 1, 0);
}

/**
  * description  Set RBG max value for RGBW bulbs
  * param        ADDRA         : address A of bulb to be set 
  * param        ADDRB         : address B of bulb to be set
  * retval       none
  */
void MiLight::RGBWSetMaxWhiteValue(byte ADDRA, byte ADDRB)
{
  RGBWAndWhiteSendCommand(0, ADDRA, ADDRB, 19, 1, 1, 1);
}

/**
  * description  Set White max value for White bulbs
  * param        ADDRA         : address A of bulb to be set 
  * param        ADDRB         : address B of bulb to be set
  * retval       none
  */
void MiLight::WhiteSetMaxWhiteValue(byte ADDRA, byte ADDRB)
{
  RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 24, 1, 1, 1);
}

/**
  * description  Dimmer temperature value for White bulbs
  * param        ADDRA         : address A of bulb to be set 
  * param        ADDRB         : address B of bulb to be set
  * param        COOMAD        : specifies type of command
  *                              This parameter can be one of following values:
  *                                [0..1] : 0 for increment, 1 for decrement
  * retval       none
  */
void MiLight::WhiteSetTemperatureValue(byte ADDRA, byte ADDRB, byte COMMAND)
{
  if (COMMAND==0)
    RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 14, 1, 0, 0);
  else
    RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 15, 1, 0, 0);
}

/**
  * description  Dimmer brightness value for White bulbs
  * param        ADDRA         : address A of bulb to be set 
  * param        ADDRB         : address B of bulb to be set
  * param        VALUE         : specifies type of command
  *                              This parameter can be one of following values:
  *                                [0..1] : 0 for increment, 1 for decrement                           
  * retval       none
  */
void MiLight::WhiteSetBrightnessValue(byte ADDRA, byte ADDRB, byte COMMAND)
{
  if (COMMAND==0)
    RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 12, 1, 0, 0);
  else
    RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 4, 1, 0, 0);
}

/**
  * description  Active night mode for White bulbs
  * param        ADDRA         : address A of bulb to be set 
  * param        ADDRB         : address B of bulb to be set
  * retval       none
  */
void MiLight::WhiteActiveNightMode(byte ADDRA, byte ADDRB)
{
  RGBWAndWhiteSendCommand(1, ADDRA, ADDRB, 27, 1, 0, 0);
}

/**
  * description  Set brightness value for RGBW bulbs
  * param        ADDRA         : address A of bulb to be set 
  * param        ADDRB         : address B of bulb to be set
  * param        VALUE         : brightness value
  *                              This parameter can be one of following values:
  *                                [0..17] :
  * retval       none
  */
void MiLight::RGBWSetBrightnessValue(byte ADDRA, byte ADDRB, byte VALUE)
{
  if(VALUE>=0 && VALUE<18)
  {
    if(VALUE!=17)
      VALUE=16-VALUE;
    else
      VALUE=23;
    
    VALUE=1+(VALUE*8);
    RGBWAndWhiteSendCommand(0, ADDRA, ADDRB, 14, 1, VALUE, 1);
  }
}

//****PRIVATE FUNCTIONS*********
//DO NOT MODIFY THIS CODE!!

/**************************************************************************/
/*
    Read a register of PL1167
*/
/**************************************************************************/
void MiLight::regWrite16(byte ADDR, byte V1, byte V2, byte WAIT)
{
  digitalWrite(_csPin, LOW);
  SPI.transfer(ADDR);
  SPI.transfer(V1);
  SPI.transfer(V2);
  digitalWrite(_csPin, HIGH);
  delayMicroseconds(WAIT);
}

/**************************************************************************/
/*
    Send command to RGBW and White bulbs
*/
/**************************************************************************/
void MiLight::RGBWAndWhiteSendCommand(byte Mode, byte ADDRA, byte ADDRB, byte CMD, byte P1, byte P2, byte P3)
{
  int cbit=0;                                  // Radio channel diversity
    
	MODE=Mode;
	ADDR_A=ADDRA;
	ADDR_B=ADDRB;
  COMMAND=CMD;
  PAR=P1;
	PAR2=P2;
	PAR3=P3;
   
	SPI.setDataMode(SPI_MODE1);
  RGBWAndWhiteSendHeadPacket();
  delayMicroseconds(350);
  for(int i=0; i<radioTransmissionNumber; i++) // Radio Transmission of packets
  {
    RGBWAndWhiteSendPacket(cbit);
    delayMicroseconds(350);
    cbit++;
    if(cbit>2)
      cbit=0;
  }
	PACK++;
	if(PACK>254)
		PACK=0;
  SPI.setDataMode(SPI_MODE0);
}

/**************************************************************************/
/*
    Send head and first packet to RGBW and White bulbs
*/
/**************************************************************************/
void MiLight::RGBWAndWhiteSendHeadPacket(void)
{
  regWrite16(7, 0, 0, 1);      // Disable TX/RX and set radio channel to 0
  
  digitalWrite(_csPin, LOW);   // Enable PL1167 SPI transmission
  SPI.transfer(36);            // Write PL1167's SYNCWORD register 0
  if(MODE==0)                  
  {
    SPI.transfer(20);          // RGBW bulbs
    SPI.transfer(122);
  }
  else
  {
    SPI.transfer(5);           // White bulbs
    SPI.transfer(10);
  }
  digitalWrite(_csPin, HIGH);  // Disable PL1167 SPI transmission
  delayMicroseconds(1);

  digitalWrite(_csPin, LOW);   // Enable PL1167 SPI transmission
  SPI.transfer(39);            // Write PL1167's SYNCWORD register 3
  if(MODE==0)
  {
    SPI.transfer(37);          // RGBW bulbs
    SPI.transfer(139);
  }
  else
  {
    SPI.transfer(85);          // White bulbs
    SPI.transfer(170);
  }
  digitalWrite(_csPin, HIGH);  // Disable PL1167 SPI transmission
  delayMicroseconds(5);
  
  regWrite16(7, 0, 0, 1);      // Disable again TX/RX and set radio channel to 0
  regWrite16(52, 128, 128, 1); // Clear TX and RX FIFO data pointer
  
  // Send packet
  digitalWrite(_csPin, LOW);   // Enable PL1167 SPI transmission
  SPI.transfer(50);            // Start writing PL1167's FIFO Data register
  SPI.transfer(7);             // Length of data buffer: 7 bytes
  if(MODE==0)
    SPI.transfer(184);         // RGBW bulbs - command type (0xB8)
  else
    SPI.transfer(90);          // White bulbs - command type (0x5A)
  
  SPI.transfer(ADDR_A);        // Address A of bulb
  SPI.transfer(ADDR_B);        // Address B of bulb
  SPI.transfer(PAR);           // Send PAR parameter
  if(MODE==0)                  // RGBW bulbs
  {
    SPI.transfer(PAR2);        // Send PAR2 parameter
    SPI.transfer(COMMAND);     // Send COMMAND parameter
  }
  else                         // White bulbs
  {
    SPI.transfer(COMMAND);     // Send COMMAND parameter
    SPI.transfer(PACK);        // Send PACK value
  }
  SPI.transfer(PACK);          // Send PACK value
  digitalWrite(_csPin, HIGH);  // Disable PL1167 SPI transmission
  delayMicroseconds(10);
  
  digitalWrite(_csPin, LOW);   // Enable PL1167 SPI transmission
  SPI.transfer(7);             // Write PL1167's TX/RX Enable and Channel Register
  SPI.transfer(1);             // Enable TX
  if(MODE==0)
    SPI.transfer(9);           // RGBW bulbs - select channel 9
  else
    SPI.transfer(4);           // White bulbs - select channel 4
  digitalWrite(_csPin, HIGH);  // Disable PL1167 SPI transmission
}

/**************************************************************************/
/*
    Send packet to RGBW and White bulbs
*/
/**************************************************************************/
void MiLight::RGBWAndWhiteSendPacket(int cbit)
{
  regWrite16(7, 0, 0, 1);          // Disable TX/RX and set radio channel to 0
  regWrite16(52, 128, 128, 1);     // Clear TX and RX FIFO data pointer
  
  digitalWrite(_csPin, LOW);       // Enable PL1167 SPI transmission
  SPI.transfer(50);                // Start writing PL1167's FIFO Data register
  SPI.transfer(7);                 // Length of data buffer: 7 bytes
  if(MODE==0)
    SPI.transfer(184);             // RGBW bulbs - command type (0xB8)
  else
    SPI.transfer(90);              // White bulbs - command type (0x5A)
  SPI.transfer(ADDR_A);            // Address A of bulb
  SPI.transfer(ADDR_B);            // Address B of bulb
  SPI.transfer(PAR);               // Send PACK value
  if(MODE==0)                      // RGBW bulbs 
  {
    SPI.transfer(PAR2);            // Send PAR2 parameter
    SPI.transfer(COMMAND);         // Send COMMAND parameter
  }
  else                             // White bulbs
  {
    SPI.transfer(COMMAND);         // Send COMMAND parameter
    SPI.transfer(PACK);            // Send PACK value
  }
  SPI.transfer(PACK);              // Send PACK value
  digitalWrite(_csPin, HIGH);      // Disable PL1167 SPI transmission
  delayMicroseconds(10);
  
  digitalWrite(_csPin, LOW);       // Enable PL1167 SPI transmission
  SPI.transfer(7);                 // Write PL1167's TX/RX Enable and Channel Register
  SPI.transfer(1);                 // Enable TX
  if(MODE==0)                      // Radio channel diversity for RGBW bulbs
  {
    //SPI.transfer(radioChannel);
    if(cbit==0) SPI.transfer(40);  // RGBW bulbs - select channel 40
    if(cbit==1) SPI.transfer(71);  // RGBW bulbs - select channel 71
    if(cbit==2) SPI.transfer(9);   // RGBW bulbs - select channel 9
  }
  else                             // Radio channel diversity for WW bulbs
  {
    if(cbit==0) SPI.transfer(39);  // White bulbs - select channel 39
    if(cbit==1) SPI.transfer(74);  // White bulbs - select channel 74
    if(cbit==2) SPI.transfer(4);   // White bulbs - select channel 4
  }
  digitalWrite(_csPin, HIGH);      // Disable PL1167 SPI transmission
}

/**************************************************************************/
/*
   Initialize PL1167 radio module
*/
/**************************************************************************/
void MiLight::initRadioModule(void)
{
  regWrite16(0, 111, 224, 7);  // Recommended value by PMmicro
  regWrite16(1, 86, 129, 7);   // Recommended value by PMmicro
  regWrite16(2, 102, 23, 7);   // Recommended value by PMmicro
  regWrite16(4, 156, 201, 7);  // Recommended value by PMmicro
  regWrite16(5, 102, 55, 7);   // Recommended value by PMmicro
  regWrite16(7, 0, 76, 7);     // PL1167's TX/RX Enable and Channel Register
  regWrite16(8, 108, 144, 7);  // Recommended value by PMmicro
  regWrite16(9, 72, 0, 7);     // PL1167's PA Control Register
  regWrite16(10, 127, 253, 7); // Recommended value by PMmicro 
  regWrite16(11, 0, 8, 7);     // PL1167's RSSI OFF Control Register -- ???
  regWrite16(12, 0, 0, 7);     // Recommended value by PMmicro 
  regWrite16(13, 72, 189, 7);  // Recommended value by PMmicro 
  regWrite16(22, 0, 255, 7);   // Recommended value by PMmicro     
  regWrite16(23, 128, 5, 7);   // PL1167's VCO Calibration Enable Register
  regWrite16(24, 0, 103, 7);   // Recommended value by PMmicro
  regWrite16(25, 22, 89, 7);   // Recommended value by PMmicro
  regWrite16(26, 25, 224, 7);  // Recommended value by PMmicro
  regWrite16(27, 19, 0, 7);    // Recommended value by PMmicro
  regWrite16(28, 24, 0, 7);    // Recommended value by PMmicro
  regWrite16(32, 72, 0, 7);    // PL1167's Data Configure Register: LEN_PREAMBLE = 010 -> (0xAAAAAA) 3 bytes, LEN_SYNCWORD = 01 -> 32 bits, LEN_TRAILER = 000 -> (0x05) 4 bits, TYPE_PKT_DAT = 00 -> NRZ law data, TYPE_FEC = 00 -> No FEC
  regWrite16(33, 63, 199, 7);  // PL1167's Delay Time Control Register 0
  regWrite16(34, 32, 0, 7);    // PL1167's Delay Time Control Register 1
  regWrite16(35, 3, 0, 7);     // PL1167's Power Management and Miscellaneous Register
  regWrite16(40, 68, 2, 7);    // PL1167's FIFO and SYNCWORD Threshold Register
  regWrite16(41, 176, 0, 7);   // PL1167's Miscellaneous Register: CRC_ON = 1 -> ON, SCR_ON = 0 -> OFF, EN_PACK_LEN = 1 -> ON, FW_TERM_TX = 1 -> ON, AUTO_ACK = 0 -> OFF, PKT_LEVEL = 0 -> PKT active high, CRC_INIT_DAT = 0
  regWrite16(42, 253, 176, 7); // PL1167's SCAN RSSI Register 0
  regWrite16(43, 0, 15, 7);    // PL1167's SCAN RSSI Register 1
  delay(200);
  regWrite16(128, 0, 0, 7);
  regWrite16(129, 255, 255, 7);
  regWrite16(130, 0, 0, 7);
  regWrite16(132, 0, 0, 7);
  regWrite16(133, 255, 255, 7);
  regWrite16(135, 255, 255, 7);
  regWrite16(136, 0, 0, 7);
  regWrite16(137, 255, 255, 7);
  regWrite16(138, 0, 0, 7);
  regWrite16(139, 255, 255, 7);
  regWrite16(140, 0, 0, 7);
  regWrite16(141, 255, 255, 7);
  regWrite16(150, 0, 0, 7);
  regWrite16(151, 255, 255, 7);
  regWrite16(152, 0, 0, 7);
  regWrite16(153, 255, 255, 7);
  regWrite16(154, 0, 0, 7);
  regWrite16(155, 255, 255, 7);
  regWrite16(156, 0, 0, 7);
  regWrite16(160, 0, 0, 7);
  regWrite16(161, 255, 255, 7);
  regWrite16(162, 0, 0, 7);
  regWrite16(163, 255, 255, 7);
  regWrite16(168, 0, 0, 7);
  regWrite16(169, 255, 255, 7);
  regWrite16(170, 0, 0, 7);
  regWrite16(171, 255, 255, 7);
  regWrite16(7, 0, 0, 7);       // Disable TX/RX and set radio channel to 0
}
