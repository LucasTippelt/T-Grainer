/*
Copyright (C) <2022>  <Lucas Tippelt>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
int loadstart;
unsigned long starttime;
unsigned long elapsedtime;
unsigned long currenttime;
bool bool1=true, bool2=true;
const int test_buttonPin = 5; 
Bounce2::Button testbutton = Bounce2::Button();
int index = 1;
int indexold;
int arrindex[2];
String myString;
String myString2;
#define encoderPinA 2
#define encoderPinB 3
int savebuttonpin= 4;
int encoderPos = 1; // a counter for the dial
unsigned int lastReportedPos = 1; // change management
static boolean rotating = false; // debounce management
Bounce2::Button savebutton = Bounce2::Button();
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
boolean A_set = false;
boolean B_set = false;

int envpin = 10;
Bounce2::Button envbutton = Bounce2::Button();
int envled = 11;
int mutepin = 12;
Bounce2::Button mutebutton = Bounce2::Button();
int muteled = 13;
int revpin = 15;
Bounce2::Button revbutton = Bounce2::Button();
int revled = 16;
int lppin = 17;
Bounce2::Button lpbutton = Bounce2::Button();
int lpled = 49;
int hppin = 50;
Bounce2::Button hpbutton = Bounce2::Button();
int hpled = 51;
int reverbpin = 52;
Bounce2::Button reverbbutton = Bounce2::Button();
int reverbled = 22;
int flangpin = 23;
Bounce2::Button flangbutton = Bounce2::Button();
int flangled = 24;
int delpin = 25;
Bounce2::Button delbutton = Bounce2::Button();
int delled = 26;
int lfo1rpin = 27;
Bounce2::Button lfo1rbutton = Bounce2::Button();
int lfo1rled = 28;
int lfo2rpin = 29;
Bounce2::Button lfo2rbutton = Bounce2::Button();
int lfo2rled = 30;
int lfo3rpin = 31;
Bounce2::Button lfo3rbutton = Bounce2::Button();
int lfo3rled = 32;
int s1pin = 33;
Bounce2::Button s1button = Bounce2::Button();
int s1led = 34;
int s2pin = 35;
Bounce2::Button s2button = Bounce2::Button();
int s2led = 36;
int s3pin = 37;
Bounce2::Button s3button = Bounce2::Button();
int s3led = 38;
int g1pin = 39;
Bounce2::Button g1button = Bounce2::Button();
int g1led = 40;
int g2pin = 41;
Bounce2::Button g2button = Bounce2::Button();
int g2led = 42;
int g3pin = 43;
Bounce2::Button g3button = Bounce2::Button();
int g3led = 44;
int g4pin = 45;
Bounce2::Button g4button = Bounce2::Button();
int g4led = 46;

void setup()
{

  loadstart=EEPROM.read(6),
pinMode(encoderPinA, INPUT);
pinMode(encoderPinB, INPUT);
digitalWrite(encoderPinA, HIGH); // turn on pullup resistors
digitalWrite(encoderPinB, HIGH); // turn on pullup resistors
attachInterrupt(0, doEncoderA, CHANGE); // encoder pin on interrupt 0 (pin 2)
attachInterrupt(1, doEncoderB, CHANGE); // encoder pin on interrupt 1 (pin 3)


    {
      
    pinMode(envled,OUTPUT);
    pinMode(revled,OUTPUT);
    pinMode(muteled,OUTPUT);
    pinMode(lpled,OUTPUT);
    pinMode(hpled,OUTPUT);
    pinMode(s1led,OUTPUT);
    pinMode(s2led,OUTPUT);
    pinMode(s3led,OUTPUT);
    pinMode(g1led,OUTPUT);
    pinMode(g2led,OUTPUT);
    pinMode(g3led,OUTPUT);
    pinMode(g4led,OUTPUT);
    pinMode(reverbled,OUTPUT);
    pinMode(flangled,OUTPUT);
    pinMode(envled,OUTPUT);
    pinMode(delled,OUTPUT);
    digitalWrite(s1led, HIGH);
    digitalWrite(g1led, HIGH);

    
      envbutton.attach (envpin,  INPUT_PULLUP );
    envbutton.interval(5); 
    envbutton.setPressedState(LOW);   
      mutebutton.attach (mutepin,  INPUT_PULLUP );
    mutebutton.interval(5); 
    mutebutton.setPressedState(LOW);   
      revbutton.attach (revpin,  INPUT_PULLUP );
    revbutton.interval(5); 
    revbutton.setPressedState(LOW);   
        lpbutton.attach (lppin,  INPUT_PULLUP );
    lpbutton.interval(5); 
    lpbutton.setPressedState(LOW);   
      reverbbutton.attach (reverbpin,  INPUT_PULLUP );
    reverbbutton.interval(5); 
    reverbbutton.setPressedState(LOW);   
      flangbutton.attach (flangpin,  INPUT_PULLUP );
    flangbutton.interval(5); 
    flangbutton.setPressedState(LOW);   
      delbutton.attach (delpin,  INPUT_PULLUP );
    delbutton.interval(5); 
    delbutton.setPressedState(LOW);   
        hpbutton.attach (hppin,  INPUT_PULLUP );
    hpbutton.interval(5); 
    hpbutton.setPressedState(LOW);   

          lfo1rbutton.attach (lfo1rpin,  INPUT_PULLUP );
    lfo1rbutton.interval(5); 
    lfo1rbutton.setPressedState(LOW);
         lfo2rbutton.attach (lfo2rpin,  INPUT_PULLUP );
    lfo2rbutton.interval(5); 
    lfo2rbutton.setPressedState(LOW);   
         lfo3rbutton.attach (lfo3rpin,  INPUT_PULLUP );
    lfo3rbutton.interval(5); 
    lfo3rbutton.setPressedState(LOW);      
      s1button.attach (s1pin,  INPUT_PULLUP );
    s1button.interval(5); 
    s1button.setPressedState(LOW);   
      s2button.attach (s2pin,  INPUT_PULLUP );
    s2button.interval(5); 
    s2button.setPressedState(LOW);   
        s3button.attach (s3pin,  INPUT_PULLUP );
    s3button.interval(5); 
    s3button.setPressedState(LOW);   
      g1button.attach (g1pin,  INPUT_PULLUP );
    g1button.interval(5); 
    g1button.setPressedState(LOW);   
      g2button.attach (g2pin,  INPUT_PULLUP );
    g2button.interval(5); 
    g2button.setPressedState(LOW);   
      g3button.attach (g3pin,  INPUT_PULLUP );
    g3button.interval(5); 
    g3button.setPressedState(LOW);   
        g4button.attach (g4pin,  INPUT_PULLUP );
    g4button.interval(5); 
    g4button.setPressedState(LOW);   


        savebutton.attach (savebuttonpin,  INPUT_PULLUP );
    savebutton.interval(5); 
    savebutton.setPressedState(LOW);   
    }
        Serial1.begin(9600);
    
     
  delay(20); 

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


    display.clearDisplay();
    display.setTextSize(1);
    display.println(" ");
    display.setTextSize(2);            
    display.setTextColor(SSD1306_WHITE);     
    display.setCursor(0,0);             
    display.println("T.Grainer");
    display.println("WS int. 21");
      delay(5);
   display.display();
display.clearDisplay();
delay(2500);
arrindex[0]=1;
arrindex[1]=1;
    getindex();   
    myString = "";
    myString = String(arrindex[0]);
    myString2 = "";
    myString2 = String(arrindex[1]);
    screen();
 
}

void buttons()
{ 
  envbutton.update();
  revbutton.update();
  mutebutton.update();
  lpbutton.update();
  hpbutton.update();
  reverbbutton.update();
  flangbutton.update();
  delbutton.update();
  lfo1rbutton.update();
  lfo2rbutton.update();
  lfo3rbutton.update();
  s1button.update();
  s2button.update();
  s3button.update();
  g1button.update();
  g2button.update();
  g3button.update();
  g4button.update();

  if (envbutton.pressed())
    {
      Serial1.write(13);
    }
  else if (revbutton.pressed())
    {
      Serial1.write(14);
    }
  else if (mutebutton.pressed())
    {
      Serial1.write(15);
    }
  else if (lpbutton.pressed())
    {
      Serial1.write(16);
    }
  else if (hpbutton.pressed())
    {
      Serial1.write(17);
    }
  else if (reverbbutton.pressed())
    {
      Serial1.write(18);
    }
  else if (flangbutton.pressed())
    {
      Serial1.write(19);
    }
  else if (delbutton.pressed())
    {
      Serial1.write(20);
    }
  else if (lfo1rbutton.pressed())
    {
      Serial1.write(21);
      displayres(1);
    }
  else if (lfo2rbutton.pressed())
    {
      Serial1.write(12);
      displayres(2);
    }
  else if (lfo3rbutton.pressed())
    {
      Serial1.write(23);
      displayres(3);
    }
  else if (s1button.pressed())
    {
      Serial1.write(24);
      arrindex[0]=1;
      digitalWrite(s1led, HIGH);
      digitalWrite(s2led, LOW);
      digitalWrite(s3led, LOW);
      bool1=true;
    }
  else if (s2button.pressed())
    {
      Serial1.write(25);
      arrindex[0]=2;
      digitalWrite(s1led, LOW);
      digitalWrite(s2led, HIGH);
      digitalWrite(s3led, LOW);
      bool1=true;
    }
  else if (s3button.pressed())
    {
      Serial1.write(26);
      arrindex[0]=3;
      bool1=true;
      digitalWrite(s1led, LOW);
      digitalWrite(s2led, LOW);
      digitalWrite(s3led, HIGH);
    }
  else if (g1button.pressed())
    {
      Serial1.write(27);
      arrindex[1]=1;
      digitalWrite(g1led, HIGH);
      digitalWrite(g2led, LOW);
      digitalWrite(g3led, LOW);
      digitalWrite(g4led, LOW);
      bool1=true;
    }
  else if (g2button.pressed())
    {
      Serial1.write(28);
      arrindex[1]=2;
      digitalWrite(g1led, LOW);
      digitalWrite(g2led, HIGH);
      digitalWrite(g3led, LOW);
      digitalWrite(g4led, LOW);
      bool1=true;
    }
  else if (g3button.pressed())
    {
      Serial1.write(29);
      arrindex[1]=3;
            digitalWrite(g1led, LOW);
      digitalWrite(g2led, LOW);
      digitalWrite(g3led, HIGH);
      digitalWrite(g4led, LOW);
      bool1=true;
    }
  else if (g4button.pressed())
    {
      Serial1.write(30);
      arrindex[1]=4;
                  digitalWrite(g1led, LOW);
      digitalWrite(g2led, LOW);
      digitalWrite(g3led, LOW);
      digitalWrite(g4led, HIGH);
      bool1=true;
    }
return;
}

void getindex()
{
  if (index==1)
  {arrindex[0]=1;arrindex[1]=1; }
  else if (index==2)
  {arrindex[0]=1;arrindex[1]=2; }
  else if (index==3)
  {arrindex[0]=1;arrindex[1]=3; }
  else if (index==4)
  {arrindex[0]=1;arrindex[1]=4; }
  else if (index==5)
  {arrindex[0]=2;arrindex[1]=1; }
  else if (index==6)
  {arrindex[0]=2;arrindex[1]=2; }
  else if (index==7)
  {arrindex[0]=2;arrindex[1]=3; }
  else if (index==8)
  {arrindex[0]=2;arrindex[1]=4; }
  else if (index==9)
  {arrindex[0]=3;arrindex[1]=1; }
  else if (index==10)
  {arrindex[0]=3;arrindex[1]=2; }
  else if (index==11)
  {arrindex[0]=3;arrindex[1]=3; }
  else if (index==12)
  {arrindex[0]=3;arrindex[1]=4; }
  else
  {display.clearDisplay(); display.setCursor(0,0); display.print("ERROR INDEX"); display.display();}
   
return;
}

void screen()
{     
    
return;
}

void displayres(int x)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.println(" ");
    display.setTextSize(2);            
    display.setTextColor(SSD1306_WHITE);     
    display.setCursor(0,0);     
    if (x== 1)
      {
        display.println("LFO 1");
      }
    else if (x== 2) 
      {
        display.println("LFO 2");
      }
    else if (x== 3) 
      {
        display.println("LFO 3");
      }
    display.println("was reset");
      delay(5);
   display.display();
return;  
}

void loop()
{
  buttons();
  rotating = true; // reset the debouncer

  if (lastReportedPos != encoderPos)
    {
      display.clearDisplay();
      display.setTextSize(2);    
      display.setTextColor(SSD1306_WHITE);     
      display.setCursor(0,0);   
      if (encoderPos<6)
        {
          display.println("Save");
          display.print("state ");
          display.print(encoderPos);
          display.print("?");
        }
      else if (encoderPos<11)
        {
          display.println("Load");
          display.print("state ");
          display.print(encoderPos-5);
          display.print("?");
        }
      else if (encoderPos==11  && loadstart==1) 
        {
          display.println("Autoload");
          display.println("last save. ");
        }
      else if (encoderPos==11  && loadstart==0) 
        {
          display.println("Don't load");
          display.println("last save.");
        }
      else
        {display.clearDisplay(); display.setCursor(0,0); display.print("ERROR");}
      display.display();
      lastReportedPos = encoderPos;
      bool1=true;
      starttime=millis();
    }
  savebutton.update();
  if (savebutton.pressed())
    {   
       if (encoderPos==11)
        { if (loadstart==0)
            {loadstart=1;}
          else 
            {loadstart=0;}
        }
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);     
      display.setCursor(0,0);   
      display.setTextSize(2);    
      if (encoderPos<6)
        {
          display.print("State ");
          display.println(encoderPos);
          display.println(" saved."); 
          Serial1.write(encoderPos+1);
        } 
      else if (encoderPos >5 && encoderPos != 11) 
        {
          display.print("State ");
          display.println(encoderPos-5);
          display.print("loaded.");
          index=1;
          Serial1.write(encoderPos+1);
        }
      else if (encoderPos==11  && loadstart==1) 
        {
          display.setTextSize(2);   
          display.println("Autoload");
          display.println("last save. ");
          EEPROM.write(6,1);
          Serial1.write(12);
        }
      else 
        {
          display.setTextSize(2);   
          display.println("Don't load");
          display.println("last save.");
          EEPROM.write(6,0);
          Serial1.write(12);
        }
      display.display();
      
      bool1=true;
      starttime=millis();
    }
 
  
  if (bool1==true)
  {
  currenttime=millis();
  elapsedtime=currenttime-starttime;
  }
  if (bool2==true or elapsedtime>5000)
      {      
      getindex();   
      myString = "";
      myString = String(arrindex[0]);
      myString2 = "";
      myString2 = String(arrindex[1]);
      screen();
      bool1=false;
      bool2=false;
      }

  indexold=index;
   leds(); 
} 

void leds()
{
  while (Serial1.available()) 
    {
      int x= Serial1.read();
      switch (x)
        {
          case 15:
            digitalWrite(revled, HIGH);
            break;
          case 16:
            digitalWrite(revled, LOW);
            break;
          case 17:
            digitalWrite(envled, HIGH);
            break;
          case 18:
            digitalWrite(envled, LOW);
            break;
          case 19:
            digitalWrite(muteled, HIGH);
            break;
          case 20:
            digitalWrite(muteled, LOW);
            break;
          case 21:
            digitalWrite(lpled, HIGH);
            break;
          case 22:
            digitalWrite(lpled, LOW);
            break;
          case 23:
            digitalWrite(hpled, HIGH);
            break;
          case 24:
            digitalWrite(hpled, LOW);
            break;
          case 25:
            digitalWrite(reverbled, HIGH);
            break;
          case 26:
            digitalWrite(reverbled, LOW);
            break;
          case 27:
            digitalWrite(flangled, HIGH);
            break;
          case 28:
            digitalWrite(flangled, LOW);
            break;
          case 29:
            digitalWrite(delled, HIGH);
            break;
          case 30:
            digitalWrite(delled, LOW);
            break;

        }
    }
return;
}


// ------------- Interrupt Service Routines ------------------------------

// ISR Interrupt on A changing state (Increment position counter)
void doEncoderA()
{
if ( rotating ) delay (1); // wait a little until the bouncing is done
if( digitalRead(encoderPinA) != A_set ) // debounce once more
{
A_set = !A_set;
// adjust counter +1 if A leads B
if ( A_set && !B_set )
{
encoderPos++;
}
  if (encoderPos>11)
    {encoderPos=1;}
  else if (encoderPos<1)
    {encoderPos=11;}
rotating = false; // no more debouncing until loop() hits again
}
}// ------------------- doEncoderA ----------------------------------------


// ISR Interrupt on B changing state, same as A above
void doEncoderB()
{
if ( rotating ) delay (1);
if( digitalRead(encoderPinB) != B_set )
{
B_set = !B_set;
//adjust counter -1 if B leads A
if( B_set && !A_set )
  {
    encoderPos -= 1;
  }
    if (encoderPos>11)
    {encoderPos=1;}
  else if (encoderPos<1)
    {encoderPos=11;}
rotating = false;
}
}
