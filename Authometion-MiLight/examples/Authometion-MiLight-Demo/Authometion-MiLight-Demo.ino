/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 TO USE AND COMPILE THIS SKETCH YOU NEED TO 
 DOWNLOAD FROM OUR WEBSITE ALSO THE MESSENGER LIBRARY
 Code by Pietro Moscetta
 http://www.authometion.com
 Version 1.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <SPI.h>
#include <MiLight.h>
#include <Messenger.h>

//PL1167 Chip Select pin
#define CS_PIN 10

byte MODE=0;
byte ADDR_A=0;
byte ADDR_B=1;
byte COMMAND=0;
byte PAR=1;
byte PAR2;
byte PAR3;

// Define an object MiLight
MiLight myMiMiLight;

// Instantiate Messenger object with the message function and the default separator (the comma character)
Messenger ser_HW=Messenger(',');

// Define messenger function
void messageVirt() 
{
  int i;

  // POWER ON LYT: PON,MODE,ADDR_A,ADDR_B
  if (ser_HW.checkString("PON"))
  { 
    MODE=ser_HW.readInt();
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();
   
    // MODE LYT WHITYE    
    if(MODE==1)
      myMiMiLight.WhiteOn(ADDR_A, ADDR_B);
    // MODE NEW LYT RGBW  
    else if(MODE==2)
      myMiMiLight.RGBWOn(ADDR_A, ADDR_B);  
     
    Serial.println("PON!");
  }
  // POWER OFF LYT: POF,MODE,ADDR_A,ADDR_B
  else if (ser_HW.checkString("POF"))
  { 
    MODE=ser_HW.readInt();
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();
    
    // MODE LYT WHITYE    
    if(MODE==1)
      myMiMiLight.WhiteOff(ADDR_A, ADDR_B);
    // MODE NEW LYT RGBW  
    else if(MODE==2)
      myMiMiLight.RGBWOff(ADDR_A, ADDR_B);  
     
    Serial.println("POF!");
  }
  // SET RADIO TRANSMISSION NUMBER: SRT,PAR
  else if (ser_HW.checkString("SRT"))
  {
    myMiMiLight.setRadioTransmissionNumber(ser_HW.readInt());
    
    Serial.println("SRT!");
  }
  // SET MAX WHYTE: SMW,MODE,ADDR_A,ADDR_B
  else if (ser_HW.checkString("SMW"))
  { 
    MODE=ser_HW.readInt();
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();
    
    // MODE LYT WHITYE    
    if(MODE==1)
      myMiMiLight.WhiteSetMaxWhiteValue(ADDR_A, ADDR_B);
    // MODE NEW LYT RGBW  
    else if(MODE==2)
      myMiMiLight.RGBWSetMaxWhiteValue(ADDR_A, ADDR_B);   
    
    Serial.println("SMW!");
  }
  // SET NEW ADDRESS: SEA,MODE,ADDR_A,ADDR_B
  else if (ser_HW.checkString("SEA"))
  { 
    MODE=ser_HW.readInt();
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();

    // MODE LYT WHITYE    
    if(MODE==1)
      myMiMiLight.WhiteSetAddress(ADDR_A, ADDR_B);
    // MODE NEW LYT RGBW  
    else if(MODE==2)
      myMiMiLight.RGBWSetAddress(ADDR_A, ADDR_B);  

    Serial.println("SEA!");
  }
  // CLEAR ADDRESS: CLA,MODE,ADDR_A,ADDR_B
  else if (ser_HW.checkString("CLA"))
  { 
    MODE=ser_HW.readInt();
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();
 
    // MODE LYT WHITYE    
    if(MODE==1)
      myMiMiLight.WhiteClearAddress(ADDR_A, ADDR_B);
    // MODE NEW LYT RGBW  
    else if(MODE==2)
      myMiMiLight.RGBWClearAddress(ADDR_A, ADDR_B);
    
    Serial.println("CLA!");
  }
  // SET TEMPERATURE: STE,ADDR_A,ADDR_B,COMMAND [COMMAND=0 -> Increment, COMMAND=1 -> Decrement]
  else if (ser_HW.checkString("STE"))
  { 
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();
    COMMAND=ser_HW.readInt(); 
    
    myMiMiLight.WhiteSetTemperatureValue(ADDR_A, ADDR_B, COMMAND);
  
    Serial.println("STE!");
  }
  // SET BRIGHTNESS: SBR,MODE,ADDA,ADDB,COMMAND [COMMAND=0 -> Increment, COMMAND=1 -> Decrement]
  else if (ser_HW.checkString("SBR"))
  { 
    MODE=ser_HW.readInt();
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();
    COMMAND=ser_HW.readInt(); 
   
    //MODE LYT WHITYE
    if(MODE==1)
      myMiMiLight.WhiteSetBrightnessValue(ADDR_A, ADDR_B, COMMAND);
    //MODE NEW LYT RGBW  
    else if(MODE==2)
      myMiMiLight.RGBWSetBrightnessValue(ADDR_A, ADDR_B, COMMAND);
      
    Serial.println("SBR!");
  }
  // SET RGB VALUE: RGB,ADDA,ADDB,COL
  else if (ser_HW.checkString("RGB"))
  { 
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();
    PAR=ser_HW.readInt();
    
    myMiMiLight.RGBWSetRGBValue(ADDR_A, ADDR_B, PAR);
    
    Serial.println("RGB!");
  }
  // ACTIVE NIGHT MODE: NIM,ADDA,ADDB,COL
  else if (ser_HW.checkString("RGB"))
  {
    ADDR_A=ser_HW.readInt();
    ADDR_B=ser_HW.readInt();
    
    myMiMiLight.WhiteActiveNightMode(ADDR_A, ADDR_B);
    
    Serial.println("NIM!");
  }
  // CHECK FREE RAM
  else if (ser_HW.checkString("RAM")) 
  { 
    Serial.println(freeRam());
  }
  // CONFIGURE IO PIN: CIO,COMMAND,PAR
  else if (ser_HW.checkString("CIO")) 
  { 
    COMMAND=ser_HW.readInt();
    PAR=ser_HW.readInt(); 
    if(PAR==0) 
    {
      pinMode(COMMAND, OUTPUT);
      Serial.print(COMMAND);
      Serial.println("-OUT");
    }
    else  
    {
      pinMode(COMMAND, INPUT); 
      Serial.print(COMMAND);
      Serial.println("-INP");
    }  
  }
  // SET IO PIN: SIO,COMMAND,PAR
  else if (ser_HW.checkString("SIO")) 
  { 
    COMMAND=ser_HW.readInt();
    PAR=ser_HW.readInt();

    digitalWrite(COMMAND, PAR);
    Serial.println("SIO!");
  }
  // GET IO PIN: GIO,COMMAND
  else if (ser_HW.checkString("GIO")) 
  { 
    COMMAND=ser_HW.readInt();
   
    Serial.print("GIO="); 
    Serial.println(digitalRead(COMMAND));
  }
  // SET PWM VALUE: PWM,PIN,VALUE
  else if (ser_HW.checkString("PWM")) 
  { 
    COMMAND=ser_HW.readInt();
    PAR=ser_HW.readInt(); 
    if(COMMAND==6 || COMMAND==9)
      digitalWrite(COMMAND, PAR);
    
    Serial.println("PWM!");
  }
  // Read ADC CH 0/7,NREAD
  else if (ser_HW.checkString("ADC")) 
  { 
    long adc_val=0;

    COMMAND=ser_HW.readInt();
    PAR=ser_HW.readInt();
    
    if(PAR==0)
      PAR=1;
    if(PAR>99)
      PAR=100;
    
    if(COMMAND>=0 & COMMAND<8)
    {
      for(i=0;i<PAR;i++)
        adc_val=adc_val+analogRead(COMMAND);
      
      adc_val=adc_val/PAR;
      
      Serial.print("ADC=");
      Serial.println(adc_val);
    }  
  }
  //Change Serial port baud rate 
  else if (ser_HW.checkString("UART")) 
  { 
    long baud;

    baud=ser_HW.readLong();
    Serial.end();
    delay(200);
    Serial.begin(baud);
  }
  else
  {
    Serial.println("UNK!");
  }
}

// Return Available free ram
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v;

  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(3000);

  //Initialize Arduino Shield LYT-WiFi 
  myMiMiLight.initialize(CS_PIN);
    
  ser_HW.attach(messageVirt);  
}

void loop() 
{
  while (Serial.available()) 
    ser_HW.process(Serial.read());
}
