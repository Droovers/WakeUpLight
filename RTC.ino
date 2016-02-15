// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}


void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}


void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  if(vDaysoff != 0){
    lcd.print(vDaysoff);
  }
  else{
    lcd.print(" ");
  }

  lcd.setCursor(4,0);
  // send it to the serial monitor
  if (hour<10)
  {
    lcd.print("0");
  }

  lcd.print(hour, DEC);
  //lcd.print(cEnc);
  // convert the byte variable to a decimal number when displayed
  lcd.print(":");
  if (minute<10)
  {
    lcd.print("0");
  }
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second<10)
  {
    lcd.print("0");
  }
  lcd.print(second, DEC);

  lcd.setCursor(3,1);
  if(dayOfWeek==1){
    lcd.print("Zo");
  }
  if(dayOfWeek==2){
    lcd.print("Ma");
  }
  if(dayOfWeek==3){
    lcd.print("Di");
  }
  if(dayOfWeek==4){
    lcd.print("Wo");
  }
  if(dayOfWeek==5){
    lcd.print("Do");
  }
  if(dayOfWeek==6){
    lcd.print("Vr");
  }
  if(dayOfWeek==7){
    lcd.print("Za");
  }

  lcd.print(", ");
  lcd.print(dayOfMonth);

  if(month==1){
    lcd.print(" jan");
  }
  if(month==2){
    lcd.print(" feb");
  }
  if(month==3){
    lcd.print(" mrt");
  }
  if(month==4){
    lcd.print(" apr");
  }
  if(month==5){
    lcd.print(" mei");
  }
  if(month==6){
    lcd.print(" jun");
  }
  if(month==7){
    lcd.print(" jul");
  }
  if(month==8){
    lcd.print(" aug");
  }
  if(month==9){
    lcd.print(" sep");
  }
  if(month==10){
    lcd.print(" okt");
  }
  if(month==11){
    lcd.print(" nov");
  }
  if(month==12){
    lcd.print(" dec");
  }
  lcd.print("   ");

  //Turn on lcd backlight on low setting between 22:00 and 6:59. Shut off backlight between 7:00 and 21:59  (lcd backlight at homescreen)
  if(hour < 7 || hour >= 22){
    BrightnessLow = vBrightnessLow;
  }
  else{
    BrightnessLow = 0;
  }

  //activate DST
  if (dayOfWeek >= 1 && month >= 3 && month <= 9 && dayOfMonth >= 25 && dayOfMonth <=31 && hour >= 2 && DST == 0)
  {
    hour = 3;
    setDS3231time(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
    EEPROM.write(eaDST,1);
    DST = 1;
  }
  //Deactivate DST
  if (dayOfWeek >= 1 && month >= 10 && dayOfMonth >= 25 && dayOfMonth <=31 && hour >= 3 && DST == 1)
  {
    hour = 2;
    setDS3231time(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
    EEPROM.write(eaDST,0);
    DST = 0;
  }

  //Clear lcd every new day
  if(second==00 && minute==00 && hour == 00){
    lcd.clear();
  }

  //Call safetyfunction every 30 seconds
  if(second==00 || second == 30){
    safety();
  }
}


void getTemp(){
  temp_msb = DS3231_get_MSB();
  temp_lsb = DS3231_get_LSB();

  vTempMsb = temp_msb;

  switch(temp_lsb){
  case 0:
    vTempLsb = 00;
    break;
  case 1 :
    vTempLsb = 25;
    break;
  case 2:
    vTempLsb = 50;
    break;
  case 3:
    vTempLsb = 75;
    break;
  }
}

byte DS3231_get_MSB(){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(DS3231_TEMPERATURE_MSB);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDRESS, 1);
  temp_msb = Wire.read();

}

byte DS3231_get_LSB(){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(DS3231_TEMPERATURE_LSB);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDRESS, 1);
  temp_lsb = Wire.read() >> 6;
}

void setAlarm(byte nr, byte HH, byte MM, byte DDD, boolean active){
  if(HH != EEPROM.read(nr)){
    EEPROM.write(nr,HH);    
  }
  if(MM != EEPROM.read(nr+10)){
    EEPROM.write(nr+10,MM);    
  }
  if(DDD != EEPROM.read(nr+20)){
    EEPROM.write(nr+20,DDD);    
  }
  if(active != bitRead(EEPROM.read(eaActiveAlarm),nr-1)){
    byte activeAlarmInt = EEPROM.read(eaActiveAlarm); 
    bitWrite(activeAlarmInt, nr-1, active);
    EEPROM.write(eaActiveAlarm, activeAlarmInt);   
  }  
  readSettings();
}

void activateAlarm(byte alarm, boolean active){
  if(active != bitRead(EEPROM.read(eaActiveAlarm),alarm-1)){
    byte activeAlarmInt = EEPROM.read(eaActiveAlarm); 
    bitWrite(activeAlarmInt, alarm-1, active);
    EEPROM.write(eaActiveAlarm, activeAlarmInt);   
  }
  readSettings();
}

void displayAlarm(byte nr, byte x, byte y){        
  lcd.setCursor(x,y);
  if(nr==1){
    if(W1H<10){
      lcd.print("0");
    }
    lcd.print(W1H);
    lcd.print(":");
    if(W1M<10){
      lcd.print("0");
    }
    lcd.print(W1M);
    lcd.print(" ZMDWDVZ");
    for(byte n=0; n<7; n++){
      if(bitRead(W1D,n)==0){
        lcd.setCursor(x+6+n,y);
        lcd.print(" ");
      }
    }
  }
  if(nr==2){
    if(W2H<10){
      lcd.print("0");
    }
    lcd.print(W2H);
    lcd.print(":");
    if(W2M<10){
      lcd.print("0");
    }
    lcd.print(W2M);
    lcd.print(" ZMDWDVZ");
    for(byte n=0; n<7; n++){
      if(bitRead(W2D,n)==0){
        lcd.setCursor(x+6+n,y);
        lcd.print(" ");
      }
    }
  }
  if(nr==3){
    if(W3H<10){
      lcd.print("0");
    }
    lcd.print(W3H);
    lcd.print(":");
    if(W3M<10){
      lcd.print("0");
    }
    lcd.print(W3M);
    lcd.print(" ZMDWDVZ");
    for(byte n=0; n<7; n++){
      if(bitRead(W3D,n)==0){
        lcd.setCursor(x+6+n,y);
        lcd.print(" ");
      }
    }
  }
  if(nr==4){
    if(W4H<10){
      lcd.print("0");
    }
    lcd.print(W4H);
    lcd.print(":");
    if(W4M<10){
      lcd.print("0");
    }
    lcd.print(W4M);
    lcd.print(" ZMDWDVZ");
    for(byte n=0; n<7; n++){
      if(bitRead(W4D,n)==0){
        lcd.setCursor(x+6+n,y);
        lcd.print(" ");
      }
    }
  }
  if(nr==5){
    if(W5H<10){
      lcd.print("0");
    }
    lcd.print(W5H);
    lcd.print(":");
    if(W5M<10){
      lcd.print("0");
    }
    lcd.print(W5M);
    lcd.print(" ZMDWDVZ");
    for(byte n=0; n<7; n++){
      if(bitRead(W5D,n)==0){
        lcd.setCursor(x+6+n,y);
        lcd.print(" ");
      }
    }
  }
}
































