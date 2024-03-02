//USE ENUM STATES FOR STATE, USE DEFINE FOR BUTTONS?

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>

#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

byte bell[8] = { //bell symbol
  B00000,
  B00100,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  
};

byte leftArrow[8] = { 
  B00001,
  B00011,
  B00111,
  B01111,
  B00111,
  B00011,
  B00001,
  
};

byte rightArrow[8] = { 
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000,
  
};

byte upArrow[8] = { 
  B00000,
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B00000,
  
};

byte downArrow[8] = { 
  B00000,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100,
  B00000,
  
};

byte keypad[8] = { 
  B00000,
  B00000,
  B00100,
  B10001,
  B00100,
  B00000,
  B00000,
  
};


byte dial[8] = { 
  B00000,
  B01110,
  B10101,
  B10111,
  B10001,
  B01110,
  B00000,
  
};

enum states
{
  DISPLAY_TIME_ALARM_ON,
  DISPLAY_TIME_ALARM_OFF,
  DISPLAY_ALARM_TIME_ON,
  DISPLAY_ALARM_TIME_OFF,
  SET_CURRENT_TIME_OFF,
  SET_CURRENT_TIME_ON,
  SET_ALARM_TIME,
  BUZZER_ON,
  SNOOZE,
  TIME_OUT 
};

int s = 0;
int m = 0;
int h = 0;

int ts;
int tm;
int th;

int as = 4;
int am = 0;
int ah = 0;
int i = 0;
int snoozetimer = 30;
unsigned long time;
unsigned long stopTime;
int addBack;
int curPos;
states state;



void setup() {
 

}

void loop() {
  time = millis();
  
  if(s>=60)
  {
   s = 0;
   m = m+1;
  }
  if(m>=60)
  {
    m = 0;
    h = h+1;
  }
  if(h>=24)
  {
    h = 0;  
  }
  
  uint8_t buttons = lcd.readButtons();

  switch (state)
    {
      case DISPLAY_TIME_ALARM_ON:
        Serial.println("state: Alarm on");
        if (buttons) {
          if (buttons & BUTTON_RIGHT ) {state = DISPLAY_ALARM_TIME_ON; s = s+1; }
          if (buttons & BUTTON_LEFT ) {state = DISPLAY_TIME_ALARM_OFF; s = s+1; delay(1000); lcd.clear(); }
          if (buttons & BUTTON_UP ) {
            stopTime = time;
            ts=s; tm=m; th=h;
            curPos = 1;
            state = SET_CURRENT_TIME_ON; 
            delay(1000);
            lcd.clear();
          }
          if (buttons & BUTTON_DOWN ) {
            stopTime = time;
            curPos = 1;
            state = SET_ALARM_TIME; 
            delay(1000);
            lcd.clear();
          }
        }
        else {
          timeAlarmOn();
          s = s+1;
          delay(1000);
        }
        
        break;

      case DISPLAY_TIME_ALARM_OFF:
        Serial.println("state: Alarm off");
        if (buttons) {
          if (buttons & BUTTON_RIGHT ) {state = DISPLAY_ALARM_TIME_OFF; s = s+1; }
          if (buttons & BUTTON_LEFT ) {state = DISPLAY_TIME_ALARM_ON; s = s+1; delay(1000); lcd.clear(); }
          if (buttons & BUTTON_UP ) {
            ts=s; tm=m; th=h;
            curPos = 1;
            state = SET_CURRENT_TIME_OFF; 
            delay(1000);
            lcd.clear();
          }
          if (buttons & BUTTON_DOWN ) {
            stopTime = time;
            curPos = 1;
            state = SET_ALARM_TIME; 
            delay(1000);
            lcd.clear();
          }
        }
        else {
          timeAlarmOff();
          s = s+1;
          delay(1000);
        }
        
        break;
        
      case DISPLAY_ALARM_TIME_ON:
        Serial.println("state: Display Alarm (On) Time");
        displayAlarmTimeOn(); 
        s=s+1;
        break;

      case DISPLAY_ALARM_TIME_OFF:
        Serial.println("state: Display Alarm (Off) Time");
        displayAlarmTimeOff();
        s=s+1;
        break;
        
      case SET_CURRENT_TIME_OFF:
        setCurrentTime();
        if (buttons){
          if (buttons & BUTTON_SELECT ) {
            state = DISPLAY_TIME_ALARM_OFF; 
            s=ts; m=tm; h=th;
            lcd.clear();
          }
        }
        if (curPos==2){
          if (buttons){
            if (buttons & BUTTON_UP) { tm=tm+1; delay(100); }
            if (buttons & BUTTON_DOWN) { tm=tm-1; delay(100); }
            if (buttons & BUTTON_RIGHT) { curPos=3; delay(500);}
            if (buttons & BUTTON_LEFT ) { curPos=1; delay(500); }
          }
        }
        if (curPos==1){
          if (buttons){
            if (buttons & BUTTON_UP) { th=th+1; delay(100); }
            if (buttons & BUTTON_DOWN) { th=th-1; delay(100); }
            if (buttons & BUTTON_RIGHT) {curPos=2; delay(500); }
          }
        }
        
        if (curPos==3){
          if (buttons){
            if (buttons & BUTTON_UP) { ts=ts+1; delay(100); }
            if (buttons & BUTTON_DOWN) { ts=ts-1; delay(100); }
            if (buttons & BUTTON_LEFT) {curPos=2; delay(500); }
          }
        }
        break;

      case SET_CURRENT_TIME_ON:
        setCurrentTime();
        if (buttons){
          if (buttons & BUTTON_SELECT ) {
            state = DISPLAY_TIME_ALARM_ON; 
            s=ts; m=tm; h=th;
            lcd.clear();
          }
        }
        if (curPos==2){
          if (buttons){
            if (buttons & BUTTON_UP) { tm=tm+1; delay(100); }
            if (buttons & BUTTON_DOWN) { tm=tm-1; delay(100); }
            if (buttons & BUTTON_RIGHT) { curPos=3; delay(500);}
            if (buttons & BUTTON_LEFT ) { curPos=1; delay(500); }
          }
        }
        if (curPos==1){
          if (buttons){
            if (buttons & BUTTON_UP) { th=th+1; delay(100); }
            if (buttons & BUTTON_DOWN) { th=th-1; delay(100); }
            if (buttons & BUTTON_RIGHT) {curPos=2; delay(500); }
          }
        }
        
        if (curPos==3){
          if (buttons){
            if (buttons & BUTTON_UP) { ts=ts+1; delay(100); }
            if (buttons & BUTTON_DOWN) { ts=ts-1; delay(100); }
            if (buttons & BUTTON_LEFT) {curPos=2; delay(500); }
          }
        }
        break;

      case SET_ALARM_TIME:
        setAlarmTime();
        if (buttons){
          if (buttons & BUTTON_SELECT ) {
            state = DISPLAY_TIME_ALARM_ON; 
            if(addBack>60){
            s=s+addBack-60;
            m=m+1;
            addBack = 0;
            }
            else{
            s=s+addBack;
            addBack = 0;
            }
            lcd.clear();
          }
        }
        if (curPos==2){
          if (buttons){
            if (buttons & BUTTON_UP) { am=am+1; delay(150); }
            if (buttons & BUTTON_DOWN) { am=am-1; delay(150); }
            if (buttons & BUTTON_RIGHT) { curPos=3; delay(500);}
            if (buttons & BUTTON_LEFT ) { curPos=1; delay(500); }
          }
        }
        if (curPos==1){
          if (buttons){
            if (buttons & BUTTON_UP) { ah=ah+1; delay(150); }
            if (buttons & BUTTON_DOWN) { ah=ah-1; delay(150); }
            if (buttons & BUTTON_RIGHT) {curPos=2; delay(500); }
          }
        }
        
        if (curPos==3){
          if (buttons){
            if (buttons & BUTTON_UP) { as=as+1; delay(150); }
            if (buttons & BUTTON_DOWN) { as=as-1; delay(150); }
            if (buttons & BUTTON_LEFT) {curPos=2; delay(500); }
          }
        }
        
        break;

      case BUZZER_ON:
        Serial.println("buzzer on");
        if (buttons) {
          
         if (buttons & BUTTON_SELECT ) {
          lcd.clear();
          lcd.setBacklight(WHITE);
          state = DISPLAY_TIME_ALARM_OFF; 
          delay(1000);
         }
         
         if (buttons & BUTTON_LEFT ) {state = SNOOZE; }
         if (buttons & BUTTON_RIGHT ) {state = SNOOZE; }
         if (buttons & BUTTON_UP ) {state = SNOOZE; }
         if (buttons & BUTTON_DOWN ) {state = SNOOZE; }
        }
        
        else {buzzerOn();}
        s=s+1;
        break;

      case SNOOZE:
        Serial.println("Snooze");
        lcd.setBacklight(WHITE);
        snooze();
        s=s+1;
        delay(1000);
        
        break;
      
    }
    if ( as == s & am == m & ah == h & state == DISPLAY_TIME_ALARM_ON ) {
      state = BUZZER_ON; 
      }
    else if ( as == s & am == m & ah == h & state == DISPLAY_ALARM_TIME_ON ) {
      state = BUZZER_ON; 
      }

}

void displayTime()
{
  
  char buffer [9];
  sprintf(buffer, "%02d:%02d:%02d", h, m, s);
  lcd.setCursor(0,0);
  lcd.print(buffer);

}
  
void timeAlarmOn()
{
  lcd.setCursor(8,0); lcd.write(byte(0)); //bell symbol
  lcd.setCursor(0,0); displayTime(); //time
  lcd.setCursor(0,1); lcd.write(byte(1)); lcd.setCursor(1,1); lcd.print("A-Off"); //Alarm Off Toggle
  lcd.setCursor(10,0); lcd.write(byte(3)); lcd.setCursor(11,0); lcd.print("Edit"); lcd.setCursor(15,0); lcd.write(byte(6)); //Edit Time
  lcd.setCursor(7,1); lcd.write(byte(4)); lcd.setCursor(8,1); lcd.print("EdA"); lcd.setCursor(11,1); lcd.write(byte(6)); //Edit Alarm Time
  lcd.setCursor(13,1); lcd.write(byte(2)); lcd.setCursor(14,1); lcd.print("A"); lcd.setCursor(15,1); lcd.write(byte(6)); //Show Alarm Time
}

void timeAlarmOff()
{
  lcd.setCursor(0,0); displayTime(); //time
  lcd.setCursor(0,1); lcd.write(byte(1)); lcd.setCursor(1,1); lcd.print("A-On"); //Alarm On Toggle
  lcd.setCursor(10,0); lcd.write(byte(3)); lcd.setCursor(11,0); lcd.print("Edit"); lcd.setCursor(15,0); lcd.write(byte(6)); //Edit Time
  lcd.setCursor(6,1); lcd.write(byte(4)); lcd.setCursor(7,1); lcd.print("EdA"); lcd.setCursor(10,1); lcd.write(byte(6)); //Edit Alarm Time
  lcd.setCursor(12,1); lcd.write(byte(2)); lcd.setCursor(13,1); lcd.print("A"); lcd.setCursor(14,1); lcd.write(byte(6)); //Show Alarm Time
  
}

void displayAlarmTimeOn()
{
  lcd.clear();
  lcd.print("Alarm Time:");
  lcd.setCursor(0,1);
  char buffer [9];
  sprintf(buffer, "%02d:%02d:%02d", ah, am, as); //alarm time
  lcd.print(buffer);
  delay(3000);
  lcd.clear();
  state = DISPLAY_TIME_ALARM_ON;

}

void displayAlarmTimeOff()
{
  lcd.clear();
  lcd.print("Alarm Time:");
  lcd.setCursor(0,1); 
  char buffer [9];
  sprintf(buffer, "%02d:%02d:%02d", ah, am, as); //alarm time  
  lcd.print(buffer);
  delay(3000);
  lcd.clear();
  state = DISPLAY_TIME_ALARM_OFF;
}

void setCurrentTime()
{
  char buffer [9];
  sprintf(buffer, "%02d:%02d:%02d", th, tm, ts);
  lcd.setCursor(0,0);
  lcd.print(buffer);
  if(ts==60)
  {
   ts = 0;
   tm = tm+1;
  }
  if (ts<0) { ts = 59; }
  if(tm==60)
  {
    tm = 0;
    th = th+1;
  }
  if (tm<0) { tm = 59; }
  if(th==24) { th = 0; }
  if(th<0) {th = 23; }
  
  lcd.setCursor(0,1); lcd.write(byte(5));
  lcd.setCursor(1,1); lcd.print("Edit");
  lcd.setCursor(7,1); lcd.print("SEL:Set");
}
  
void setAlarmTime()
{
  addBack = ((time - stopTime)/1000);
  
  char buffer [9];
  sprintf(buffer, "%02d:%02d:%02d", ah, am, as);
  lcd.setCursor(0,0);
  lcd.print(buffer);
  if(as==60)
  {
   as = 0;
   am = am+1;
  }
  if (as<0) { as = 59; }
  if(am==60)
  {
    am = 0;
    ah = ah+1;
  }
  if (am<0) { am = 59; }
  if(ah==24) { ah = 0; }
  if(ah<0) {ah = 23; }

  lcd.setCursor(0,1); lcd.write(byte(5));
  lcd.setCursor(1,1); lcd.print("Edit");
  lcd.setCursor(7,1); lcd.print("SEL:Set");
}

void buzzerOn()
{
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.write(byte(5));
  lcd.setCursor(1,1);
  lcd.print("Zz");
  lcd.setCursor(5,1);
  lcd.print("SEL:Off");
  lcd.setCursor(0,0);
  displayTime(); 
  lcd.setBacklight(RED);
  delay(166);
  lcd.setBacklight(YELLOW);
  delay(166);
  lcd.setBacklight(GREEN);
  delay(166);  
  lcd.setBacklight(TEAL);
  delay(166);  
  lcd.setBacklight(BLUE);
  delay(166);  
  lcd.setBacklight(VIOLET);
  delay(170); 
 
}

void snooze()
{
  snoozetimer = snoozetimer - 1;
  
  
  if (snoozetimer == 0)
  {
    snoozetimer = 30;
    state = BUZZER_ON;  
  }
  else
  {
  lcd.clear();
  displayTime();  
  }
}

