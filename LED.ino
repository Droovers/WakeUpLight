/*===============================================================================================================
 | This function activates the testmode for the RGB-strip. Fadein R, then fadein G, then fadeinB. takes 7 sec.   |
 ================================================================================================================*/
void LEDtest(){
  vR=1;
  vG=1;
  vB=1;

  while(vR>0 & vG>0 & vB>0)
  {
    lcd.setCursor(0,0);
    lcd.print("TEST TEST TEST  ");
    lcd.setCursor(0,1);
    lcd.print(" TEST TEST TEST ");

    if(vR<255 & vG==1 & vB==1){
      vR = vR++;
    }
    if(vR == 255){
      vR=0;
    }
    if(vR==0 & vG<255 & vB==1){
      vG = vG++;
    }
    if(vG == 255){
      vG=0;
    }
    if(vR==0 & vG==0 & vB<255){
      vB = vB++;
    }
    if(vB == 255){
      vB=0;
    }
    analogWrite(pRed, vR);
    analogWrite(pGreen, vG);
    analogWrite(pBlue, vB);
    delay(10);
  }
  lcd.clear();
}

void sendRGB(byte R, byte G, byte B, byte intensity){

  if(R > 0 || G >0 || B > 0){
    vR=R;
    vG=G;
    vB=B;
  }

  R=R/intensity;
  G=G/intensity;
  B=B/intensity;

  analogWrite(pRed, R);
  analogWrite(pGreen, G);
  analogWrite(pBlue, B);
}

void fadeOut(){
  if(fade == 1){
    if(vR >= vG && vR >= vB){
      byte tFade=30/vR;
    }
    if(vG >= vR && vG >= vB){
      byte tFade=30/vG;
    }
    if(vB >= vR && vB >= vG){
      byte tFade=30/vB;
    }
    unsigned long tFadeStart = millis();
    fade = 2;
  }

}















