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

#ifndef MILIGHT_H
#define MILIGHT_H

#define DEFAULT_RADIO_TRASMISSION 100

class MiLight
{
  private:
    //CS pin for LYT radio module
    byte _csPin;
    byte ADDR_A, ADDR_B, PAR, PAR2, PAR3, COMMAND, MODE, PACK;
    //Number of radio transmission for LYT
    int radioTransmissionNumber;

    void initRadioModule(void);
    void regWrite16(byte ADDR, byte V1, byte V2, byte WAIT);
    void RGBSendCommand(byte ADDRA, byte ADDRB, byte CMD, byte PAR);
    void RGBSendHeadPacket(void);
    void RGBSendPacket(void);
    void RGBWAndWhiteSendCommand(byte Mode, byte ADDRA, byte ADDRB, byte CMD, byte PAR, byte PAR2, byte PAR3);
    void RGBWAndWhiteSendHeadPacket(void);
    void RGBWAndWhiteSendPacket(int cbit);
    
  public:
    //Init Iotuino board
    void initialize(byte csPin);
    
    //Set radio transmission
    void setRadioTransmissionNumber(int Val);
    int setRadioTransmissionNumber(void);
    void setDefaultRadioTransmissionNumber(void);
    
    //Power On LYT bulb
    void RGBOn(byte ADDRA, byte ADDRB);
    void RGBWOn(byte ADDRA, byte ADDRB);
    void WhiteOn(byte ADDRA, byte ADDRB);
    
    //Power Off LYT bulb
    void RGBOff(byte ADDRA, byte ADDRB);
    void RGBWOff(byte ADDRA, byte ADDRB);
    void WhiteOff(byte ADDRA, byte ADDRB);
    
    //Set the max white intensity
    void RGBSetMaxWhiteValue(byte ADDRA, byte ADDRB);
    void RGBWSetMaxWhiteValue(byte ADDRA, byte ADDRB);
    void WhiteSetMaxWhiteValue(byte ADDRA, byte ADDRB);
    
    //Set LYT address
    void RGBSetAddress(byte ADDRA, byte ADDRB);
    void RGBWSetAddress(byte ADDRA, byte ADDRB);
    void WhiteSetAddress(byte ADDRA, byte ADDRB);
    
    //Clear LYT address
    void RGBClearAddress(byte ADDRA, byte ADDRB);
    void RGBWClearAddress(byte ADDRA, byte ADDRB);
    void WhiteClearAddress(byte ADDRA, byte ADDRB);
    
    //Set color (only for RGB and RGBW LYT)
    void RGBSetRGBValue(byte ADDRA, byte ADDRB, byte VALUE);
    void RGBWSetRGBValue(byte ADDRA, byte ADDRB, byte VALUE);
    
    //Set light temperature (only for White LYT)
    void WhiteSetTemperatureValue(byte ADDRA, byte ADDRB, byte COMMAND);
    
    //Set brightness (only for White LYT)
    void WhiteSetBrightnessValue(byte ADDRA, byte ADDRB, byte COMMAND);
    
    //Active night mode on (only for White LYT)
    void WhiteActiveNightMode(byte ADDRA, byte ADDRB);

    //Set brightness (only for RGBW LYT)
    void RGBWSetBrightnessValue(byte ADDRA, byte ADDRB, byte VALUE);
};

#endif
