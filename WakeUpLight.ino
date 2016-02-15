/*===================================================PINOUT=====================================================
 |   Arduino	12V	RTC	LCD	LED	Input                                                           |
 |   -------------------------------------------------                                                           |
 |   Vin	        12v				                                                                |
 |   5V		        VCC			                                                                |
 |   GND		        GND	LED-	Source, Gate	  enc common on ground                                                      |
 |   D2			        LCD D7		                                                                |
 |   D3~			        LCD D6		                                                                |
 |   D4			        LCD D5		                                                                |
 |   D5~			        LCD D4		                                                                |
 |   D6~				        GR	                                                                |
 |   D7					      Bok                                                               |
 |   D8					      Besc                                                              |
 |   D9~				        GG	                                                                |
 |   D10~				      GB	                                                        |
 |   D11~			LED+		                                                                |
 |   D12			        RS		                                                                |
 |   D13			        Enable		                                                                |
 |   A0					      EncA                                                              |
 |   A1					      EncB                                                              |
 |   A2					      Benc                                                              |
 |   A3					                                                                        |
 |   A4 (SDA)		SDA			                                                                |
 |   A5 (SCL)		SCL			                                                                |
 |   A6					                                                                        |
 |   A7					                                                                        |
 |                                                                                                               |
 ================================================================================================================*/

/*===================================================ToDo=======================================================
 |  V Safety temperature based                                                                                   |
 |  X LCD_LED fade out/in                                                                                        |
 |  V Wekker&brightness settings in EEprom !!actieve wekkerin ds3231 (kan niet, dagen niet mogelijk)!!           |
 |  X function blink cursor var                                                                                  |
 |  V Brightness, display while setting                                                                          |
 |  V Auto zomer/wintertijd ahv algoritme(laatste weekend vd maand)                                              |
 |  X Resetknop                                                                                                  |
 |    ActiveAlarm binary                                                                                         |
 |    pas in menu bij vbEnc = true                                                                               |
 |    In homescreen, timer als enc++, bevestigen met vbEncLong, countdown op scherm                              |
 |    Lichten uit met button                                                                                     |
 |    Verjaardagen/events
 ================================================================================================================*/

// Libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>
#define ENCODER_OPTIMIZE_INTERRUPTS    //Encoder lib from http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>

// Setup RTC
#define DS3231_I2C_ADDRESS          0x68
#define DS3231_TEMPERATURE_MSB      0x11
#define DS3231_TEMPERATURE_LSB      0x12

byte temp_msb;
byte temp_lsb;
byte DST;                //save in eeprom

// Setup LCD
#define LCD_D7      A0
#define LCD_D6      A1
#define LCD_D5      4
#define LCD_D4      5
#define LCD_Enable  13
#define LCD_RS      12
#define LCD_LED     11
byte vBrightnessHigh = 50; // brightness lcd
byte vBrightnessLow = 5;   // brightness lcd
byte BrightnessLow;

LiquidCrystal lcd(LCD_RS, LCD_Enable, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Setup LED-strip
#define pRed         6
#define pGreen       9
#define pBlue        10
byte vR;
byte vG;
byte vB;
byte vBrightness;
byte fade = 0;

// Setup IR receiver
#include <IRremote.h>
#define RECV_PIN A6
byte I = 1;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Setup UI
#define bEsc        7         //8  A3
#define bEnc        8         //A2
#define ENC_A       2
#define ENC_B       3
#define ENC_PORT PINC
Encoder myEnc(ENC_A, ENC_B);
int eNewPosition = 0;
int eOldPosition = 0;
boolean eChangeUp = false;
boolean eChangeDown = false;
boolean skipbEnc = false;

// Setup menus
#define main        0
#define wekker      1
#define vrij        2
#define licht       3
#define tijd        4
#define misc        5

byte menuMain = 0;
byte submenu = 0;
byte menuWekker = 0;
byte menuVrij = 0;
byte menuLicht = 0;
byte menuTijd = 0;
byte menuMisc = 0;
byte subsubmenu = 0;
//byte mVrij = 0;
//byte mSafety = 0;
//boolean mChange = false;

// Setup misc
byte bOkState = 0;
byte bEscState = 0;
byte bEncState = 0;
boolean vbEnc = false;
boolean vbEncLong = false;
boolean vbEsc = false;
int cbEnc = 0;
byte lastbOkState = 0;
byte lastbEscState = 0;
byte lastbEncState = 0;
int tDelay = 100;
int cHome = 0;
byte tHome = 5;            //Time till menu structure resets to homescreen
byte vDaysoff = 0;
byte intvDaysoff = 0;
byte vTempMsb;
byte vTempLsb;
byte Ack = 0;
byte Ok = 0;
byte cutoffTemp;
byte intCutoffTemp;
byte nWekker = 1;
byte uur;
byte minuut;
byte dag;
boolean actief;
boolean dagActive = false;
byte cDag = 0;


//EEprom addresses
#define eaActiveAlarm 0
#define eaW1H 1 
#define eaW2H 2 
#define eaW3H 3 
#define eaW4H 4 
#define eaW5H 5 
#define eaW1M 11 
#define eaW2M 12 
#define eaW3M 13 
#define eaW4M 14 
#define eaW5M 15 
#define eaW1D 21 
#define eaW2D 22 
#define eaW3D 23 
#define eaW4D 24 
#define eaW5D 25
#define eaDST 6
#define eaCutoffTemp 7

// Setup alarm
byte W1H;
byte W1M;
byte W1D;
byte W2H;
byte W2M;
byte W2D;
byte W3H;
byte W3M;
byte W3D;
byte W4H;
byte W4M;
byte W4D;
byte W5H;
byte W5M;
byte W5D;
byte activeAlarm;

void setup() 
{
  //Pinmodes
  pinMode(bEnc, INPUT);
  pinMode(bEsc, INPUT);

  //Start communictions
  Wire.begin();              // Start I2C
  lcd.begin(16, 2);          // Set up the 16x2 LCD
  //  Serial.begin(9600);

  //  LEDtest();             // Test LEDs at startup

  //ONLY FIRST TIME:
  //  setDS3231time(00, 15, 18, 7, 2, 1, 16);
  //  EEPROM.write(eaCutoffTemp,55);
  //  EEPROM.write(eaDST,0);
  //  setAlarm(1,6,0,B0111110,1);
  readSettings();
}

void loop() 
{
  readInput();
  showMenu();
  safety();
  delay(tDelay);
}


/*===============================================================================================================
 | This safety function is being called from the RTC function. If ds3231 's temperature rises above 55°C, LED's  |
 | will be shut off. Can be reset with long press from encoder button.                                           |
 ================================================================================================================*/

void safety(){
  boolean safetyCutout;  
  do{
    getTemp();
    if(vTempMsb <= cutoffTemp){
      break;
    }
    readInput();
    analogWrite(pRed, 0);
    analogWrite(pGreen, 0);
    analogWrite(pBlue, 0);
    safetyCutout = true;
    if(vTempMsb == 255){
      lcd.setCursor(0,0);
      lcd.print("!!Safety cutout!");
      lcd.setCursor(0,1);
      lcd.print("RTC disconnected");
    }
    else{
      lcd.setCursor(0,0);
      lcd.print("!!Safety cutout!");
      lcd.setCursor(0,1);
      lcd.print("    Temp ");
      lcd.print(vTempMsb);
      lcd.print(" C   ");
    }
    if(vbEncLong == true){
      safetyCutout = false;
      lcd.clear();
    }
    delay(tDelay);
  } 
  while(safetyCutout == true);
}
















