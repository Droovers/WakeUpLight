#define	Setup	0x2FD18E7
#define	Cancel	0x2FD30CF
#define	Off	0x2FDA857
#define	On	0x2FD38C7
#define	Flash	0x2FD8877
#define	Strobe	0x2FDF00F
#define	Fade	0x2FDC837
#define	Smooth	0x2FD08F7
#define	R1	0x2FD807F
#define	R2	0x2FD20DF
#define	R3	0x2FDE01F
#define	R4	0x2FD50AF
#define	R5	0x2FDB04F
#define	G1	0x2FD40BF
#define	G2	0x2FDA05F
#define	G3	0x2FD10EF
#define	G4	0x2FDD02F
#define	G5	0x2FD708F
#define	B1	0x2FDC03F
#define	B2	0x2FD609F
#define	B3	0x2FD906F
#define	B4	0x2FDE817
#define	B5	0x2FDB847
#define	White	0x2FD9867
#define	I4	0x2FDF807
#define	I3	0x2FDD827
#define	I2	0x2FD7887
#define	I1	0x2FD58A7

void readInput()
{
  //-------Rotation from encoder---------
  eNewPosition = myEnc.read()/4;
  if (eNewPosition == eOldPosition) {
    eChangeUp = false;
    eChangeDown = false;
  }
  if (eNewPosition > eOldPosition) {
    eChangeUp = true;
    eChangeDown = false;
    eOldPosition = eNewPosition;
  }
  if (eNewPosition < eOldPosition) {
    eChangeUp = false;
    eChangeDown = true;
    eOldPosition = eNewPosition;
  }
  //*************************************\\


  //Save previous input && read current input
  lastbEscState = bEscState;
  lastbEncState = bEncState;
  bEscState = digitalRead(bEsc);
  bEncState = digitalRead(bEnc);

  //-------Esc button--------------------
  if(bEscState != lastbEscState && bEscState == HIGH){
    vbEsc=true;
  }
  else{
    vbEsc = false;
  }

  //---Short&long press encoder button---
  if(bEncState == lastbEncState && bEncState == HIGH){
    cbEnc++;
  }
  else{
    cbEnc = 0;
  }

  if(bEncState == lastbEncState && bEncState == LOW){
    vbEnc = false;
  }

  if(cbEnc>=(1000*3)/(tDelay)){
    vbEncLong = true;
    skipbEnc = true;
    cbEnc = 0;
  }
  else{
    vbEncLong = false;
    if(bEncState != lastbEncState && bEncState == LOW && skipbEnc == false){
      vbEnc = true;
      skipbEnc = false;
    }
  }
  if(bEncState != lastbEncState && bEncState == LOW && skipbEnc == true){
    vbEnc = false;
    skipbEnc = false;
  }

  //---IR-remote-input-------------------
  if (irrecv.decode(&results))
  {
    if(results.value != 0xFFFFFFFF){
      switch (results.value){

      default:
        break;
      case Setup:
        break;
      case Cancel:
        break; 
      case Off:
        sendRGB(0,0,0,I);
        break; 
      case On:
        sendRGB(vR,vG,vB,I);
        break; 
      case Flash:
        break; 
      case Strobe:
        break; 
      case Fade:
        fadeOut();
        break; 
      case Smooth:
        break; 
      case R1:
        sendRGB(255,0,0,I);
        break; 
      case R2:
        sendRGB(255,50,0,I);
        break; 
      case R3:
        sendRGB(255,100,0,I);
        break; 
      case R4:
        sendRGB(255,150,0,I);
        break; 
      case R5:
        sendRGB(255,255,0,I);
        break; 
      case G1:
        sendRGB(0,255,0,I);
        break; 
      case G2:
        sendRGB(0,255,100,I);
        break; 
      case G3:
        sendRGB(0,255,150,I);
        break; 
      case G4:
        sendRGB(0,255,200,I);
        break; 
      case G5:
        sendRGB(0,255,255,I);
        break;
      case B1:
        sendRGB(0,0,255,I);
        break; 
      case B2:
        sendRGB(100,0,255,I);
        break; 
      case B3:
        sendRGB(150,0,255,I);
        break; 
      case B4:
        sendRGB(200,0,255,I);
        break; 
      case B5:
        sendRGB(255,0,255,I);
        break; 
      case White:
        sendRGB(255,255,255,I);
        break; 
      case I4:
        I=1;
        sendRGB(vR,vG,vB,I);
        break; 
      case I3:
        I=2;
        sendRGB(vR,vG,vB,I);
        break; 
      case I2:
        I=4;
        sendRGB(vR,vG,vB,I);
        break; 
      case I1:
        I=6;
        sendRGB(vR,vG,vB,I);
        break; 
      }
    }
    irrecv.resume(); // Receive the next value
  }
  //-----------------
  if(fade != 0){
    fadeOut();
  }
  //Select main menu item to display
  if(submenu == 0)
  {    
    //Next menu, reset homescreencounter, clear lcd.
    if(eChangeUp == true){
      menuMain++;
      subsubmenu = 0;
      Home(0);
      //    mChange = true;
      if(menuMain>=6){
        menuMain=1;
      }
      lcd.clear();
      eChangeUp = false;
    }
    //Prev menu, reset homescreencounter, clear lcd.
    if(eChangeDown == true){
      if(menuMain<=0){
        menuMain=5;
      }
      menuMain--;
      subsubmenu = 0;
      Home(0);
      //  mChange = true;
      if(menuMain<=0){
        menuMain=5;
      }
      lcd.clear();
      eChangeDown = false;
    }
    if(vbEsc == true){
      menuMain = 0;
      subsubmenu = 0;
      lcd.clear();
      vbEsc = false;
    }
  }

  //If state changed, button is high & menu wekker is active: change menu-item
  if(submenu == wekker && subsubmenu == 0)
  {    //Next menu, reset homescreencounter, clear lcd.
    if(eChangeUp == true){            //enc++
      if(menuWekker == 1){
        menuWekker = 0;
      }
      else{
        menuWekker++;
      }
      subsubmenu = 0;
      Home(0);
      lcd.clear();
      eChangeUp = false;
    } //Prev menu, reset homescreencounter, clear lcd.
    if(eChangeDown == true){        //enc--
      if(menuWekker==0){
        menuWekker=1;
      }
      menuWekker--;
      subsubmenu = 0;
      Home(0);
      if(menuWekker==0){
        menuWekker=2;
      }
      lcd.clear();
      eChangeDown = false;
    }
    if(vbEsc == true){
      submenu = 0;
      subsubmenu = 0;
      lcd.clear();
      vbEsc = false;
    }
  }

  //If state changed, button is high & menu vrij is active: change menu-item; NOT IN USE, ONLY 1 MENU ITEM
  //  if(submenu == vrij && subsubmenu == 0)
  //  {    //Next menu, reset homescreencounter, clear lcd.
  //    if(eChangeUp == true){        //enc++
  //      menuVrij++;
  //      subsubmenu = 0;
  //      Home(0);
  //      if(menuVrij==1){
  //        menuVrij=0;
  //      }
  //      lcd.clear();
  //    } //Prev menu, reset homescreencounter, clear lcd.
  //    if(eChangeDown == true){    //enc--
  //      if(menuVrij==-1){
  //        menuVrij=0;
  //      }
  //      menuVrij--;
  ////      subsubmenu = 0;
  //      Home(0);
  //      if(menuVrij==-1){
  //        menuVrij=0;
  //      }
  //      lcd.clear();
  //    }
  //    if(vbEsc == true){
  //      submenu = 0;
  //      subsubmenu = 0;
  //      lcd.clear();
  //    }
  //  }

  //If state changed, button is high & menu licht is active: change menu-item
  if(submenu == licht && subsubmenu == 0)
  {    //Next menu, reset homescreencounter, clear lcd.
    if((eChangeUp == true || eChangeDown == true) && subsubmenu == 0){        //enc++
      if(menuLicht == 0){
        menuLicht = 1;
      }
      else{
        menuLicht = 0;
      }
      Home(0);
      lcd.clear();
      eChangeUp = false;
      eChangeDown = false;
    } //Prev menu, reset homescreencounter, clear lcd.
    //    if(eChangeDown == true){    //enc--
    //      if(menuLicht==0){
    //        menuLicht=4;
    //      }
    //      menuLicht--;
    //      subsubmenu = 0;
    //      Home(0);
    //      if(menuLicht==-1){
    //        menuLicht=3;
    //      }
    //      lcd.clear();
    //      eChangeDown = false;
    //    }
    if(vbEsc == true){
      submenu = 0;
      subsubmenu = 0;
      lcd.clear();
      vbEsc = false;
    }
  }

  //If state changed, button is high & menu tijd is active: change menu-item
  if(submenu == tijd && subsubmenu == 0)
  {    //Next menu, reset homescreencounter, clear lcd.
    if(eChangeUp == true){          //enc++
      menuTijd++;
      subsubmenu = 0;
      Home(0);
      if(menuTijd==2){
        menuTijd=0;
      }
      lcd.clear();
      eChangeUp = false;
    } //Prev menu, reset homescreencounter, clear lcd.
    if(eChangeDown == true){        //enc--
      if(menuTijd==0){
        menuTijd=1;
      }
      menuTijd--;
      subsubmenu = 0;
      Home(0);
      if(menuTijd==-1){
        menuTijd=1;
      }
      lcd.clear();
      eChangeDown = false;
    }
    if(vbEsc == true){
      submenu = 0;
      subsubmenu = 0;
      lcd.clear();
      vbEsc = false;
    }
  }

  //If state changed, button is high & menu tijd is active: change menu-item
  if(submenu == misc && subsubmenu == 0)
  {    //Next menu, reset homescreencounter, clear lcd.
    if(eChangeUp == true){        //enc++
      if(menuMisc==2){
        menuMisc=0;
      }
      else{
        menuMisc++;
      }
      subsubmenu = 0;
      Home(0);
      lcd.clear();
      eChangeUp = false;
    }//Prev menu, reset homescreencounter, clear lcd.
    if(eChangeDown == true){    //enc--
      if(menuMisc==0){
        menuMisc=2;
      }
      else{
        menuMisc--;
      }
      subsubmenu = 0;
      Home(0);
      if(menuMisc==0){
        menuMisc=3;
      }
      lcd.clear();
      eChangeDown = false;
    }
    if(vbEsc == true){
      submenu = 0;
      subsubmenu = 0;
      Home(0);
      lcd.clear();
      vbEsc = false;
    }
  }
}

// Read saved settings from EEprom
void readSettings(){
  activeAlarm = EEPROM.read(eaActiveAlarm);

  W1H = EEPROM.read(eaW1H);
  W1M = EEPROM.read(eaW1M);
  W1D = EEPROM.read(eaW1D);

  W2H = EEPROM.read(eaW2H);
  W2M = EEPROM.read(eaW2M);
  W2D = EEPROM.read(eaW2D);

  W3H = EEPROM.read(eaW3H);
  W3M = EEPROM.read(eaW3M);
  W3D = EEPROM.read(eaW3D);

  W4H = EEPROM.read(eaW4H);
  W4M = EEPROM.read(eaW4M);
  W4D = EEPROM.read(eaW4D);

  W5H = EEPROM.read(eaW5H);
  W5M = EEPROM.read(eaW5M);
  W5D = EEPROM.read(eaW5D);

  DST = EEPROM.read(eaDST);
  intCutoffTemp = cutoffTemp = EEPROM.read(eaCutoffTemp);
}

//void saveSettings(){
//  if(cutoffTemp != EEPROM.read(eaCutoffTemp);){
//    cutoffTemp = intCutoffTemp;
//    EEPROM.write(eaCutoffTemp,cutoffTemp);
//  }
//}











