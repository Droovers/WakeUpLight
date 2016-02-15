/*===============================================================================================================
 | This function displays the correct menu on the LCD.                                                            |
 ================================================================================================================*/

void showMenu()
{
  switch(submenu)
  {
  default:
    switch(menuMain)
    {
    default:    //Homescreen
      displayTime();
      analogWrite(LCD_LED,BrightnessLow);
      Home(0);
      break;

    case wekker:    //Settings wekker
      lcd.setCursor(0,0);
      lcd.print("Settings >Wekker");
      lcd.setCursor(10,1);
      lcd.print("Vrij ");
      lcd.print(vDaysoff);
      if(vbEnc == true)
      {
        submenu = wekker;
        lcd.clear();
        vbEnc = false;
      }
      Home(1);
      break;

    case vrij:    //Settings vrije dag
      lcd.setCursor(0,0);
      lcd.print("Settings >Vrij ");
      lcd.print(vDaysoff);
      lcd.setCursor(10,1);
      lcd.print("Licht");
      if(vbEnc == true){
        submenu = vrij;
        lcd.clear();
        vbEnc = false;
      }
      Home(1);
      break; 

    case licht:    //Settings licht
      lcd.setCursor(0,0);
      lcd.print("Settings >Licht");
      lcd.setCursor(10,1);
      lcd.print("Tijd");
      if(vbEnc == true){
        submenu = licht;
        lcd.clear();
        vbEnc = false;
      }
      Home(1);
      break;

    case tijd:    //Settings tijd
      lcd.setCursor(0,0);
      lcd.print("Settings >Tijd");
      lcd.setCursor(10,1);
      lcd.print("Misc");
      if(vbEnc == true){
        submenu = tijd;
        lcd.clear();
        vbEnc = false;
      }
      Home(1);
      break;

    case misc:    //Settings miscellaneous
      lcd.setCursor(0,0);
      lcd.print("Settings >Misc");
      lcd.setCursor(10,1);
      lcd.print("Wekker");
      if(vbEnc == true){
        submenu = misc;
        lcd.clear();
        vbEnc = false;
      }
      Home(1);
      break;

    }
    break;
    /*----------------------------------------------------------------------------*/
  case wekker:      //Submenu alarm

    switch(menuWekker)
    {
    default:
      lcd.setCursor(0,0);
      if(subsubmenu == 0){
        lcd.print("Settings wekker:");
        lcd.setCursor(4,1);
        lcd.print(">Zet wekker ");
        if(vbEnc == true && cHome >0){
          subsubmenu = 1;
          vbEnc = false;
          lcd.clear();
        }
        Home(1);
      }
      if(subsubmenu != 0){
        lcd.print("Wekker ");
        lcd.print(subsubmenu);
        if(bitRead(activeAlarm,subsubmenu-1) == 1){
          lcd.print(": Aan");
          if(vbEnc == HIGH){
            activateAlarm(subsubmenu,0);
          }
        }
        else{
          lcd.print(": Uit");
          if(vbEnc == HIGH){
            activateAlarm(subsubmenu,0);
          }
        }
        displayAlarm(subsubmenu,1,1);

        if(eChangeUp == true){
          if(subsubmenu == 5){
            subsubmenu = 1;
          }
          else{
            subsubmenu++;
          }
          lcd.clear();
          eChangeUp=false;

        }
        if(eChangeDown == true){
          subsubmenu--;
          lcd.clear();
          if(subsubmenu==0){
            subsubmenu = 5;
          }
          eChangeDown=false;
        }
        if(vbEsc == true){
          subsubmenu = 0;
          lcd.clear();
          Home(0);
          vbEsc = false;
        }
      }
      break;

    case 1:
      lcd.setCursor(0,0);
      if(subsubmenu == 0){
        lcd.print("Settings wekker:");
        lcd.setCursor(4,1);
        lcd.print(">Edit wekker");
        if(vbEnc == 1){
          subsubmenu = 1;
          Home(0);
          lcd.clear();
          vbEnc = false;
        }
        Home(1);
      }
      if(subsubmenu == 1){
        lcd.print("Edit wekker ");
        lcd.print(nWekker);

        if(eChangeUp == true){
          nWekker++;
          if(nWekker == 6){
            nWekker = 1;
          }
        }
        if(eChangeDown == true){
          nWekker--;
          if(nWekker==0){
            nWekker = 5;
          }
        }

        if(vbEnc == true){
          actief = bitRead(activeAlarm,nWekker-1);
          if(nWekker == 1){
            uur = W1H;
            minuut = W1M;
            dag = W1D;
          }
          if(nWekker == 2){
            uur = W2H;
            minuut = W2M;
            dag = W2D;
          }
          if(nWekker == 3){
            uur = W3H;
            minuut = W3M;
            dag = W3D;
          }
          if(nWekker == 4){
            uur = W4H;
            minuut = W4M;
            dag = W4D;
          }
          if(nWekker == 5){
            uur = W5H;
            minuut = W5M;
            dag = W5D;
          }
          if(uur > 23){
            uur = 23;
          }
          if(minuut >59){
            minuut = 59;
          }
          subsubmenu = 2;
          lcd.clear();
          Home(0);
          vbEnc = false;
        }
        if(vbEsc == true){
          subsubmenu = 0;

          lcd.clear();
          Home(0);
          vbEsc = false;
        }
      }
      if(subsubmenu == 2){      //uur

        lcd.print("Edit w");
        lcd.print(nWekker);
        lcd.print(": Tijd");

        lcd.setCursor(5,1);
        if(uur<10){
          lcd.print("0");
        }
        lcd.print(uur);
        lcd.print(":");
        if(minuut<10){
          lcd.print("0");
        }
        lcd.print(minuut);

        if(eChangeUp == true){
          uur++;
          if(uur >= 24){
            uur = 0;
          }
        }
        if(eChangeDown == true){
          if(uur <= 0){
            uur = 23;
          }
          else{
            uur--;
          }
        }

        if(vbEnc == true){
          subsubmenu = 3;
          Home(0);
          lcd.clear();
          vbEnc = false;
        }
        if(vbEsc == true){
          subsubmenu = 1;
          lcd.clear();
          Home(0);
          vbEsc = false;
        }
      }
      if(subsubmenu == 3){      //Minuut
        lcd.print("Edit w");
        lcd.print(nWekker);
        lcd.print(": Tijd");

        lcd.setCursor(5,1);
        if(uur<10){
          lcd.print("0");
        }
        lcd.print(uur);
        lcd.print(":");
        if(minuut<10){
          lcd.print("0");
        }
        lcd.print(minuut);

        if(eChangeUp == true){
          minuut++;
          if(minuut >= 60){
            minuut = 0;
          }
        }
        if(eChangeDown == true){
          if(minuut == 0){
            minuut = 59;
          }
          else{
            minuut--;
          }
        }

        if(vbEnc == true){
          subsubmenu = 4;
          lcd.clear();
          Home(0);
          vbEnc = false;
        }
        if(vbEsc == true){
          subsubmenu = 2;
          Home(0);
          vbEsc = false;
        }
      }

      if(subsubmenu == 4){      //Interval
        lcd.print("Edit w");
        lcd.print(nWekker);
        lcd.print(":Interval");
        lcd.setCursor(3,1);
        if(cDag == 0){
          lcd.print(">Werkdagen");
        }
        if(cDag == 1){
          lcd.print(">Weekend  ");
        }
        if(cDag == 2){
          lcd.print(">Custom   ");
        }
        if(cDag == 3){
          lcd.print(">Eenmalig ");
        }

        if(eChangeUp == true){
          cDag++;
          if(cDag >= 3){
            cDag = 0;
          }
        }
        if(eChangeDown == true){
          if(cDag == 0){
            cDag = 2;
          }
          else{
            cDag--;
          }
        }

        if(vbEnc == true){
          if(cDag == 0){
            subsubmenu = 6;
            dag = B0111110;
            cDag = 0;
          }
          if(cDag == 1){
            subsubmenu = 6;
            dag = B1000001;
            cDag = 0;
          }
          if(cDag == 2){
            subsubmenu = 5;
            cDag = 0;
          }
          if(cDag == 3){
            subsubmenu = 6;
            cDag = 0;
          }

          lcd.clear();
          Home(0);
          vbEnc = false;
        }
        if(vbEsc == true){
          subsubmenu = 3;
          lcd.clear();
          Home(0);
          vbEsc = false;
        }
      }
      if(subsubmenu == 5){      //Custom interval
        lcd.print("Custom interval:");
        lcd.setCursor(1,1);
        if(cDag == 0){
          lcd.print(">Zondag ");
          if(dagActive == true){
            lcd.print("    aan");
          }
          else{
            lcd.print("    uit");
          }
        }
        if(cDag == 1){
          lcd.print(">Maandag ");
          if(dagActive == true){
            lcd.print("   aan");
          }
          else{
            lcd.print("   uit");
          }
        }
        if(cDag == 2){
          lcd.print(">Dinsdag ");
          if(dagActive == true){
            lcd.print("   aan");
          }
          else{
            lcd.print("   uit");
          }
        }
        if(cDag == 3){
          lcd.print(">Woensdag ");
          if(dagActive == true){
            lcd.print("  aan");
          }
          else{
            lcd.print("  uit");
          }
        }
        if(cDag == 4){
          lcd.print(">Donderdag ");
          if(dagActive == true){
            lcd.print(" aan");
          }
          else{
            lcd.print(" uit");
          }
        }
        if(cDag == 5){
          lcd.print(">Vrijdag ");
          if(dagActive == true){
            lcd.print("   aan");
          }
          else{
            lcd.print("   uit");
          }
        }
        if(cDag == 6){
          lcd.print(">Zaterdag ");
          if(dagActive == true){
            lcd.print("  aan");
          }
          else{
            lcd.print("  uit");
          }
        }
        if(cDag == 7){
          lcd.print(">Bevestig      ");
        }

        if(eChangeUp == true && cDag < 7){
          cDag++;
          dagActive=bitRead(dag,cDag);
        }
        if(eChangeDown == true && cDag > 0){
          cDag--;
          dagActive=bitRead(dag,cDag);
        }

        if(vbEnc == true && cDag < 7){
          if(dagActive == true){
            dagActive = false;
            bitWrite(dag,cDag,dagActive);
          }
          else{
            dagActive = true;
            bitWrite(dag,cDag,dagActive);
          }
          vbEnc = false;
        }
        if(vbEnc == true && cDag == 7){
          subsubmenu = 6;
          Home(0);
          lcd.clear();
          vbEnc = false;
        }
        if(vbEsc == true){
          subsubmenu = 1;
          lcd.clear();
          Home(0);
          vbEsc = false;
        }
      }

      if(subsubmenu == 6){      //Wekker inschakelen?
        lcd.print("Zet wekker?");
        lcd.setCursor(6,1);
        if(actief == true){
          lcd.print("Ja ");
        }
        else{
          lcd.print("Nee");
        }


        if(eChangeUp == true || eChangeDown == true){
          if(actief == true){
            actief = false;
          }
          else{
            actief = true;
          }
        }

        if(vbEnc == true){
          bitWrite(activeAlarm,nWekker-1,actief);
          subsubmenu = 7;
          lcd.clear();
          vbEnc = false;
        }

        if(vbEsc == true){
          subsubmenu = 5;
          lcd.clear();
          Home(0);
          vbEsc = false;
        }
      }
      if(subsubmenu == 7){      //Bevestigen
        lcd.print("  Bevestigen:");
        lcd.setCursor(1,1);

        if(uur<10){
          lcd.print("0");
        }
        lcd.print(uur);
        lcd.print(":");
        if(minuut<10){
          lcd.print("0");
        }
        lcd.print(minuut);
        lcd.print(" ZMDWDVZ");
        for(byte n=0; n<7; n++){
          if(bitRead(dag,n)==0){
            lcd.setCursor(7+n,1);
            lcd.print(" ");
          }
        }
        lcd.setCursor(15,1);
        lcd.print(actief);

        if(vbEncLong == true){
          setAlarm(nWekker,uur,minuut,dag,actief);
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("Wekker ");
          lcd.print(nWekker);
          lcd.setCursor(3,1);
          lcd.print("ingesteld");
          delay(3000);
          nWekker = 0;
          uur = 0;
          minuut = 0;
          dag = 0;
          actief = false;          
          subsubmenu = 0;
          submenu = 0;
          lcd.clear();
          vbEnc = false;
        }

        if(vbEsc == true){
          subsubmenu = 6;
          lcd.clear();
          Home(0);
          vbEsc = false;
        }
      }
      break;

    }
    break;

    /*----------------------------------------------------------------------------*/
  case vrij:
    switch(menuVrij)
    {
    default:
      lcd.setCursor(0,0);
      lcd.print("Settings vrij:  ");
      lcd.setCursor(7,1);
      //      lcd.print("        ");
      lcd.print(intvDaysoff);
      lcd.print(" dagen");

      if(eChangeUp == true && intvDaysoff < 9){    //encoder ++
        intvDaysoff++;
      }
      if(eChangeDown == true && intvDaysoff > 0){  //encoder --
        intvDaysoff--;
      }

      if(vbEsc == true){
        intvDaysoff = vDaysoff;
        cHome = 0;  
        submenu = 0;
        lcd.clear();
        vbEsc = false;
      }
      if(vbEncLong == true){
        vDaysoff = intvDaysoff;
        cHome = 0;  
        submenu = 0;
        lcd.clear();
        vbEncLong == false;
      }
      break;
    }
    break;

    /*----------------------------------------------------------------------------*/
  case licht:
    switch(menuLicht)
    {
    default:
      if(subsubmenu == 0){
        lcd.setCursor(0,0);
        lcd.print("Settings licht:");
        lcd.setCursor(4,1);
        lcd.print(">LED");

        if(vbEnc == true){
          subsubmenu = 1;
          vbEnc = false;
          lcd.clear();
        }
        Home(1);
      }

      if(subsubmenu == 1){
        lcd.setCursor(0,0);
        lcd.print("Settings LED:  ");
        lcd.setCursor(4,1);
        lcd.print(">Brightness");

        if(eChangeUp == true || eChangeDown == true){
          subsubmenu == 2;
          eChangeUp = eChangeDown = false;
        }

        if(vbEnc == true){
          subsubmenu = 3;
          vbEnc = false;
          lcd.clear();
        }

        if(vbEsc == true){
          subsubmenu = 0;          
          lcd.clear();
          Home(0);
          vbEsc = false;
        }
        Home(1);
      }

      if(subsubmenu == 3){
        lcd.setCursor(0,0);
        lcd.print("LED timeout:");
        lcd.setCursor(4,1);
        lcd.print("");
      
        if(eChangeUp == true || eChangeDown == true){
          subsubmenu == 1;
          eChangeUp = eChangeDown = false;
        }

        if(vbEnc == true){
          subsubmenu = 4;
          vbEnc = false;
          lcd.clear();
        }

        if(vbEsc == true){
          subsubmenu = 0;          
          lcd.clear();
          Home(0);
          vbEsc = false;
        }
        Home(1);
      }
      break;

    case 1:
      if(subsubmenu == 0){
        lcd.setCursor(0,0);
        lcd.print("Settings licht:");
        lcd.setCursor(4,1);
        lcd.print(">LCD");
        Home(1);
      }

      if(subsubmenu == 1){
        lcd.setCursor(0,0);
        lcd.print("LCD dim:    ");
        lcd.setCursor(4,1);
        lcd.print("vFelheid");
      }

      if(subsubmenu == 2){
        lcd.setCursor(0,0);
        lcd.print("LCD fel:    ");
        lcd.setCursor(4,1);
        lcd.print(">");
      }
      if(subsubmenu == 3){
        lcd.setCursor(0,0);
        lcd.print("LCD timeout:");
        lcd.setCursor(4,1);
        lcd.print(">Timeout");
      }
      break;
    }
    break;

    /*----------------------------------------------------------------------------*/
  case tijd:
    switch(menuTijd)
    {
    default:
      lcd.setCursor(0,0);
      lcd.print("Settings time:");
      lcd.setCursor(6,1);
      lcd.print(">Edit tijd");
      Home(1);
      break;
    }
    break;

    /*----------------------------------------------------------------------------*/
  case misc:
    switch(menuMisc)
    {
    default:
      getTemp();
      lcd.setCursor(0,0);
      lcd.print("Settings misc:");
      lcd.setCursor(5,1);
      lcd.print(vTempMsb);
      lcd.print(",");
      lcd.print(vTempLsb);
      lcd.print(" C    ");
      Home(1);
      break;

    case 1:      
      lcd.setCursor(0,0);
      lcd.print("Settings misc:");
      lcd.setCursor(6,1);
      lcd.print(">Test");
      if(vbEnc == 1){
        LEDtest();
      }
      Home(1);
      break;

    case 2:      

      lcd.setCursor(0,0);
      lcd.print("Settings misc:");
      if(subsubmenu == 0){
        lcd.setCursor(6,1);
        lcd.print(">Safety");
        lcd.print(cutoffTemp);
        if(vbEnc == true){
          subsubmenu = 1;
          lcd.clear();
        }
      }

      if(subsubmenu == 1){      //mSafety
        lcd.setCursor(7,1);
        lcd.print(intCutoffTemp);
        lcd.print(" C     ");

        if(eChangeUp == true && cutoffTemp < 99){    //encoder ++
          intCutoffTemp++;
        }
        if(eChangeDown == true && cutoffTemp > 30){  //encoder --
          intCutoffTemp--;
        }

        if(vbEncLong == true){
          subsubmenu=0;
          if(cutoffTemp != intCutoffTemp){
            cutoffTemp = intCutoffTemp;
            EEPROM.write(eaCutoffTemp,cutoffTemp);
            Home(0);
            vbEncLong = false;
          }
        }
        if(vbEsc == 1){
          subsubmenu=0;
          intCutoffTemp = cutoffTemp;
          Home(0);
          vbEsc = false;
        }
        if(cHome >= 2){
          Home(3);
        }
      }
      Home(1);
      break;
    }
    break;
  }
}





/*==============================================================================================================
 | When in the menustructure, this function resets the screen after a certain amount of time to the homescreen.  | 
 | Home(0) resets the counter, Home(1) adds 2 to the counter, Home(2) adds 1 to the counter; 0 resets, 1 short   |
 | wait till reset, 2 long wait till reset. Home 3 counteracts home 1.                                                                     |
 ================================================================================================================*/
void Home(byte a)
{
  if(a==0){
    cHome=0;
  }
  if(a==1){
    cHome=cHome+2;
    analogWrite(LCD_LED,vBrightnessHigh);
  }
  if(a==2){
    cHome++;
    analogWrite(LCD_LED,vBrightnessHigh);
  }

  if(a==3){
    cHome=cHome-2;
    analogWrite(LCD_LED,vBrightnessHigh);
  }

  if(cHome == (1000*tHome*2)/(tDelay))
  {
    lcd.clear();
    submenu=0;
    subsubmenu = 0;
    menuMain=0;
  }
}










