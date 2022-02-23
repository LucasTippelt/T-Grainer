#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MIDI.h>
#include <YetAnotherPcInt.h>
#include <EEPROM.h>
#include <RotaryEncoder.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C 
#define PCINT_PINpitch1 13
#define PCINT_PINpitch2 12
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
// MIDI SEND FÜR KNÖPFE VON ARD2
MIDI_CREATE_DEFAULT_INSTANCE();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int c=0;
 int pitchcountersave;
int offset1;
int eeprom1counter=0, eeprom2counter=0, var =0, var2=0;
int potsend1, potsend2, lpscreen, hpscreen, sample=1, sampleold=1, grain=1, grainold=1, screen1val[12], s1counter=1, s2counter=1, s3counter=1, s1counterold, s2counterold, 
s3counterold, sendarr[16], lfobutton_pin=52, lfobuttonstate=0, lfo=1, index=0, indexold=1, gaincounter[16], gain[]={1, 65, 66, 0, 0, 45, 0, 1017, 2}, lfostate_gain[16], 
pitchcounter[16], pitch[]={3, 12, 13, 0, 0, 1, 10, 500, 6}, lfostate_pitch[16], pancounter[16], pan[]={2, 3, 4, 0,0, 2, 0, 128, 5}, lfostate_pan[16], panpitch[12], 
screenstate[12], lpcounter[16], lp[]={4, 7, 8, 0,0, 3, 15, 2000, 9}, lfostate_lp[16], hpcounter[16], hp[]={5, 10, 11, 0,0, 9, 15, 2000, 14}, lfostate_hp[16],
lfo1fcounter[1],lfo1f[]={6, 25, 26, 0,0, 10, 10, 2000, 27}, lfo1acounter[1], lfo1a[]={7, 28, 29, 0,0, 1, 0, 100, 30}, lfo2fcounter[1], 
lfo2f[]={8, 31, 32, 0,0, 10, 10, 2000, 33}, lfo2acounter[1], lfo2a[]={9, 34, 35, 0,0, 1, 0, 100, 36}, lfo3fcounter[1], lfo3f[]={10, 37, 38, 0,0, 10, 10, 2000, 39}, lfo3a[]={11, 40, 41, 0,0, 1, 0, 100, 42},
mgainval, mgainvalold=0, counter=0, mgain=55 /*A1*/, lfo3acounter[1], revdec[]={18, 52, 53,0,0,10,0,200,54},revdw[]={12, 43, 44,0,0,1,0,100,45},flangdep[]={13, 46, 47,0,0,1,0,100,48}, 
flangdw[]={14, 49, 50,0,0,1,0,100,51}, del_time[]={16, 56, 57,0,0,10,0,200,58}, delfeed[]={17, 59, 60,0,0,10,0,200,61},deldw[]={15, 62,63,0,0,1,0,100,64};

int revdeccounter[12],lfostate_revdec[12], revdwcounter[12], lfostate_revdw[12], flangdepcounter[12], lfostate_flangdep[12], flangdwcounter[12], lfostate_flangdw[12], 
del_timecounter[12],lfostate_del_time[12], delfeedcounter[12], lfostate_delfeed[12], deldwcounter[12], lfostate_deldw[12], lfo1waveform, lfo2waveform, lfo3waveform;
float deldwscale=1.0f, delfeedscale=1.0f, del_timescale=1.0f, revdwscale=1.0f, revdecscale=1.0f, flangdwscale=1.0f, flangdepscale=1.0f;

const int mgainnote=1, lfogainnote=2, lfopannote=3, lfopitchnote=4, lfolpnote=5, lfohpnote=6, lforevdecnote = 7, lforevdwnote = 8, lfoflangdepnote = 9,lfoflangdwnote = 10,
lfodel_timenote =11, lfodelfeednote = 12, lfodeldwnote =13, lfo1waveformnote=14, lfo2waveformnote=15, lfo3waveformnote=16,envelopenote= 17, reversenote=18, mutenote= 19,
sellpnote= 20, selhpnote=21,selrevnote = 22,selflangnote=23,seldelnote=24;

float  screen1val2[12], gainscale=0.155665f*0.633f, pitchscale=0.01f, panscale=1, lpscale=6, hpscale=20, lfofscale= 0.01f, lfoascale= 0.05f;
int reverse[12], mute[12], selrev[12], selflang[12], seldel[12], envelope[12], sellp[12], selhp[12];
bool bools1, bools2, bools3, boolloadfinish=false, mgainvalmatch, rotating, pitchrotating, pitchrotating2, start=false, A_setpitch = false, B_setpitch=false;

unsigned long starttime, currenttime, elapsedtime, starttime2, currenttime2, elapsedtime2, starttime3, currenttime3, elapsedtime3;

String screen1string1[12], screen1string2[12], screen1string3[12], myString, myString2, gainstring[2]={"Gain", "%"}, pitchstring[2]={"Pitch", ""}, panstring[2]={"Pan", " "}, 
panstring2[12], lpstring[2]={"Fq High Cut", "Hz"}, hpstring[2]={"Fq Low Cut", "Hz"}, lfo1fstring[2]={"Fq LFO 1", "Hz"}, lfo1astring[2]={"Amount LFO 1", "%"}, lfo2fstring[2]={"Fq LFO 2", "Hz"},
lfo2astring[2]={"Amount LFO 2", "%"}, lfo3fstring[2]={"Fq LFO 3", "Hz"},  lfo3astring[2]={"Amount LFO 3", "%"}, revdecstring[2]={"Reverb Decay", "ms"}, revdwstring[2]={"Reverb Dry/Wet", "%"}, 
flangdepstring[2]={"Flanger Depth", ""}, flangdwstring[2]={"Flanger Dry/Wet", "%"}, del_timestring[2]={"Delay Decay Time", "ms"}, delfeedstring[2]={"Delay Feedback", "%"},deldwstring[2]={"Delay Dry/Wet", "%"};

  

#define lfo1f_in1 lfo1f[1]
#define lfo1f_in2 lfo1f[2]
RotaryEncoder lfo1fencoder(lfo1f_in2, lfo1f_in1, RotaryEncoder::LatchMode::TWO03);
#define lfo2f_in1 lfo2f[1]
#define lfo2f_in2 lfo2f[2]
RotaryEncoder lfo2fencoder(lfo2f_in2, lfo2f_in1, RotaryEncoder::LatchMode::TWO03);
#define lfo3f_in1 lfo3f[1]
#define lfo3f_in2 lfo3f[2]
RotaryEncoder lfo3fencoder(lfo3f_in2, lfo3f_in1, RotaryEncoder::LatchMode::TWO03);
#define lfo1a_in1 lfo1a[1]
#define lfo1a_in2 lfo1a[2]
RotaryEncoder lfo1aencoder(lfo1a_in2, lfo1a_in1, RotaryEncoder::LatchMode::TWO03);
#define lfo2a_in1 lfo2a[1]
#define lfo2a_in2 lfo2a[2]
RotaryEncoder lfo2aencoder(lfo2a_in2, lfo2a_in1, RotaryEncoder::LatchMode::TWO03);
#define lfo3a_in1 lfo3a[1]
#define lfo3a_in2 lfo3a[2]
RotaryEncoder lfo3aencoder(lfo3a_in2, lfo3a_in1, RotaryEncoder::LatchMode::TWO03);
#define gain_in1 gain[1]
#define gain_in2 gain[2]
RotaryEncoder gainencoder(gain_in2, gain_in1, RotaryEncoder::LatchMode::TWO03);
#define hp_in1 hp[1]
#define hp_in2 hp[2]
RotaryEncoder hpencoder(hp_in2, hp_in1, RotaryEncoder::LatchMode::TWO03);
#define lp_in1 lp[1]
#define lp_in2 lp[2]
RotaryEncoder lpencoder(lp_in2, lp_in1, RotaryEncoder::LatchMode::TWO03);
#define pan_in1 pan[1]
#define pan_in2 pan[2]
RotaryEncoder panencoder(pan_in2, pan_in1, RotaryEncoder::LatchMode::TWO03);

#define revdec_in1 revdec[1]
#define revdec_in2 revdec[2]
RotaryEncoder revdecencoder(revdec_in2, revdec_in1, RotaryEncoder::LatchMode::TWO03);
#define revdw_in1 revdw[1]
#define revdw_in2 revdw[2]
RotaryEncoder revdwencoder(revdw_in2, revdw_in1, RotaryEncoder::LatchMode::TWO03);
#define flangdep_in1 flangdep[1]
#define flangdep_in2 flangdep[2]
RotaryEncoder flangdepencoder(flangdep_in2, flangdep_in1, RotaryEncoder::LatchMode::TWO03);
#define flangdw_in1 flangdw[1]
#define flangdw_in2 flangdw[2]
RotaryEncoder flangdwencoder(flangdw_in2, flangdw_in1, RotaryEncoder::LatchMode::TWO03);
#define del_time_in1 del_time[1]
#define del_time_in2 del_time[2]
RotaryEncoder del_timeencoder(del_time_in2, del_time_in1, RotaryEncoder::LatchMode::TWO03);
#define delfeed_in1 delfeed[1]
#define delfeed_in2 delfeed[2]
RotaryEncoder delfeedencoder(delfeed_in2, delfeed_in1, RotaryEncoder::LatchMode::TWO03);
#define deldw_in1 deldw[1]
#define deldw_in2 deldw[2]
RotaryEncoder deldwencoder(deldw_in2, deldw_in1, RotaryEncoder::LatchMode::TWO03);


    Bounce2::Button gainbutton = Bounce2::Button();
    Bounce2::Button pitchbutton = Bounce2::Button();
    Bounce2::Button panbutton = Bounce2::Button();
    Bounce2::Button lpbutton = Bounce2::Button();
    Bounce2::Button hpbutton = Bounce2::Button();
    Bounce2::Button lfo1button = Bounce2::Button();
    Bounce2::Button lfo1abutton = Bounce2::Button();
    Bounce2::Button lfo2button = Bounce2::Button();
    Bounce2::Button lfo2abutton = Bounce2::Button();
    Bounce2::Button lfo3button = Bounce2::Button();
    Bounce2::Button lfo3abutton = Bounce2::Button();
    Bounce2::Button revdecbutton  = Bounce2::Button();
    Bounce2::Button revdwbutton = Bounce2::Button();
    Bounce2::Button flangdepbutton  = Bounce2::Button();
    Bounce2::Button flangdwbutton = Bounce2::Button();
    Bounce2::Button del_timebutton  = Bounce2::Button();
    Bounce2::Button delfeedbutton = Bounce2::Button();
    Bounce2::Button deldwbutton = Bounce2::Button();

void varinit()
{
  for (int i=0;i<12;i++)
  {
    gaincounter[i]=830;
    pitchcounter[i]=100;
    pancounter[i]=63;
    lpcounter[i]=333;
    hpcounter[i]=250;
    lfostate_pitch[i]=4;
    lfostate_gain[i]=4;
    lfostate_pan[i]=4;
    lfostate_lp[i]=4;
    lfostate_hp[i]=4;
    
    revdeccounter[i]=  50;
    lfostate_revdec[i]= 4;  
    revdwcounter[i]=  50; 
    lfostate_revdw[i]= 4; 
    flangdepcounter[i]=  50; 
    lfostate_flangdep[i]= 4; 
    flangdwcounter[i]=  50; 
    lfostate_flangdw[i]= 4; 
    del_timecounter[i]= 50 ;
    lfostate_del_time[i]=  4; 
    delfeedcounter[i]=  50; 
    lfostate_delfeed[i]= 4; 
    deldwcounter[i]=  50; 
    lfostate_deldw[i]= 4; 
    reverse[i]= 0; 
    mute[i]=  0; 
    selrev[i]= 0; 
    selflang[i]=  0; 
    seldel[i]=  0; 
    envelope[i]= 1;
    
    
  }
  lfo1waveform=2; lfo2waveform=2; lfo3waveform=2;
  lfo1fcounter[0]=100;
  lfo2fcounter[0]=100;
  lfo3fcounter[0]=100;
  lfo1acounter[0]=100;
  lfo2acounter[0]=100;
  lfo3acounter[0]=100;
  
  return;
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) 
{ 
  setup2();
}

void setup() 
{ 
  
  Serial1.begin(9600);
  MIDI.begin(MIDI_CHANNEL_OMNI); 
  MIDI.setHandleNoteOn(HandleNoteOn); 
  /*Display*/ {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
    {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);   
    }
  if (EEPROM.read(4006)!=1)
        {
            var=4002;
            var2=4004;
            eeprom1counter=EEPROM.read(4000);
            eeprom2counter=EEPROM.read(4001);
        }
  else
        {
            var=4003;
            var2=4005;
            eeprom1counter=EEPROM.read(4000);
            eeprom2counter=EEPROM.read(4001);
        }
  display.clearDisplay();
  display.display();
  display.setTextSize(2);            
  display.setTextColor(SSD1306_WHITE);     
  display.setCursor(0,0);           
  if (EEPROM.read(var2)==0) 
        {
          display.println("T.Grainer");
          display.println("No load");
          delay(50);
          display.display();
          display.clearDisplay();
        }
  else 
        {    
            int x1=EEPROM.read(var)-2;
            display.println("Loading");
            display.print("State ");
            display.print("1 ");
            display.print(x1+1);
            delay(50);
            display.display();
            display.clearDisplay();
            delay(50);
        }
  }
  /*Arduino pins attach*/{
  gainbutton.attach (gain[8],  INPUT_PULLUP );
  gainbutton.interval(5); 
  gainbutton.setPressedState(LOW);   
  gain[3] = digitalRead(gain[1]); 
 pinMode(gain[1], INPUT);
 pinMode(gain[2],INPUT);  
  pitchbutton.attach (pitch[8],  INPUT_PULLUP );
  pitchbutton.interval(5); 
  pitchbutton.setPressedState(LOW);   
  pitch[3] = digitalRead(pitch[1]);
  pinMode(PCINT_PINpitch1, INPUT);
  pinMode(PCINT_PINpitch2, INPUT);
  PcInt::attachInterrupt(PCINT_PINpitch1, doEncoderA, "1", CHANGE); 
  PcInt::attachInterrupt(PCINT_PINpitch2, doEncoderB, "1", CHANGE); 
  panbutton.attach (pan[8],  INPUT_PULLUP );
  panbutton.interval(5); 
  panbutton.setPressedState(LOW); 
  pan[3]= digitalRead(pan[1]);  pan[4]= digitalRead(pan[2]);  

  lpbutton.attach (lp[8],  INPUT_PULLUP );
  lpbutton.interval(5); 
  lpbutton.setPressedState(LOW); 
  lp[3]= digitalRead(lp[1]); lp[4]= digitalRead(lp[2]);   

  hpbutton.attach (hp[8],  INPUT_PULLUP );
  hpbutton.interval(5); 
  hpbutton.setPressedState(LOW); 
  hp[3]= digitalRead(hp[1]);  hp[4]= digitalRead(hp[2]);  

  lfo1button.attach (lfo1f[8],  INPUT_PULLUP );
  lfo1button.interval(5); 
  lfo1button.setPressedState(LOW);  
  lfo1f[3]= digitalRead(lfo1f[1]);  lfo1f[4]= digitalRead(lfo1f[2]);  

  lfo1abutton.attach (lfo1a[8],  INPUT_PULLUP );
  lfo1abutton.interval(5); 
  lfo1abutton.setPressedState(LOW);  
  lfo1a[3]= digitalRead(lfo1a[1]);  lfo1a[4]= digitalRead(lfo1a[2]);  

  lfo2button.attach (lfo2f[8],  INPUT_PULLUP );
  lfo2button.interval(5); 
  lfo2button.setPressedState(LOW); 
  lfo2f[3]= digitalRead(lfo2f[1]);  lfo2f[4]= digitalRead(lfo2f[2]); 

  lfo2abutton.attach (lfo2a[8],  INPUT_PULLUP );
  lfo2abutton.interval(5); 
  lfo2abutton.setPressedState(LOW);  
  lfo2a[3]= digitalRead(lfo2a[1]); lfo2a[4]= digitalRead(lfo2a[2]);

  lfo3button.attach (lfo3f[8],  INPUT_PULLUP );
  lfo3button.interval(5); 
  lfo3button.setPressedState(LOW); 
  lfo3f[3]= digitalRead(lfo3f[1]);  lfo3f[4]= digitalRead(lfo3f[2]); 
  lfo3abutton.attach (lfo3a[8],  INPUT_PULLUP );
  lfo3abutton.interval(5); 
  lfo3abutton.setPressedState(LOW);  




  revdwbutton.attach( revdw[8], INPUT_PULLUP);
  revdwbutton.interval(5);
  revdwbutton.setPressedState(LOW);
  revdecbutton.attach( revdec[8], INPUT_PULLUP);
  revdecbutton.interval(5);
  revdecbutton.setPressedState(LOW);

  
flangdepbutton.attach( flangdep[8], INPUT_PULLUP);
  flangdepbutton.interval(5);
  flangdepbutton.setPressedState(LOW);
  flangdwbutton.attach( flangdw[8], INPUT_PULLUP);
  flangdwbutton.interval(5);
  flangdwbutton.setPressedState(LOW);
  del_timebutton.attach( del_time[8], INPUT_PULLUP);
  del_timebutton.interval(5);
  del_timebutton.setPressedState(LOW);
  delfeedbutton.attach( delfeed[8], INPUT_PULLUP);
  delfeedbutton.interval(5);
  delfeedbutton.setPressedState(LOW);
  deldwbutton.attach( deldw[8], INPUT_PULLUP);
  deldwbutton.interval(5);
  deldwbutton.setPressedState(LOW);
  }


  if (EEPROM.read(var2)==0) 
        {
          varinit();
          hpcounter[0]=250;
          setuploop();
          for (int i=0;  i<12; i++)
          {screenstate[index]=0;}
          indexold=1;
             index=0;
          start=true;
        }
    else 
        {
            int x1=EEPROM.read(var)-2;
            startloadstate(x1);
            indexold=1;
             index=0;
            screenstate[index]=0;
            start=true;
        }
  delay(3600);

  starttime=millis();
  
}


void loop() 
{ 
  if (pitchrotating==false or c==1)
  {
    endlesspotfct2 (pitch, pitchcounter, index, pitchstring, pitchscale, lfostate_pitch, 2);
    pitchrotating=true; 
    c++;
    if (c==2)
      {c=0;}
  }
  rotating=true;
  encoders();
  Ard2();
  //mgainval = analogRead(mgain);  
  /*if (index != indexold)
    {
      
      starttime = millis();
      for (int i=0; i<4; i++)
        {
          display.println("  ");
          display.display();
        } 
      if (screenstate[index]==0)
        {
          display.clearDisplay();
          display.setTextSize(1);            
          display.setTextColor(SSD1306_WHITE);     
          display.setCursor(0,0);             
          display.println("Can't fetch");
          display.println("last value");
          display.setTextSize(1);    
          lpscreen=lpcounter[index]*lpscale;
          hpscreen=hpcounter[index]*hpscale;
          myString = "";
          myString = String(lpscreen);
          display.print("Fq High Cut = ");
          display.print(myString);
          display.println("Hz");
          myString = "";
          myString = String(hpscreen);
          display.print("Fq Low Cut = ");
          display.print(myString);
          display.println("Hz");
          display.display();
        }
      else if (screenstate[index]==2 && start == false)
        {
          screen2();
        }
      else if (screenstate[index]==1 && start == false)
        {
          screen1();
        }
      else if (screenstate[index]==3 && start == false)
        {
          screen3();
        } 
      
    } */
  LFOfct();
  buttons();
  //mgainfct();
  currenttime = millis();
  elapsedtime = currenttime - starttime;
  if (elapsedtime>2400)
    {
      if (boolloadfinish == false)
        {
          MIDI.sendNoteOn(1,1,2); 
          boolloadfinish =true; 
        }
      else 
        {
        MIDI.sendNoteOn(1,0,2); 
        boolloadfinish =false;   
        }
    starttime = millis(); 
    } 
  //sends a note after 150ms to check if connection lost
  /*currenttime3 = millis();
  elapsedtime3 = currenttime3 - starttime3;
  if (elapsedtime3>50)
    {
      if (bools2 == false && start == false)
              {screen2();
              screenstate[index]=2;
              bools2=true;
              }
      else if (bools3 == false && start == false)
              {
              screen3();
              screenstate[index]=3;
              bools3=true;
              }         
      else if (bools1 == false && start == false)
              {screen1();
              screenstate[index]=1;
              bools1=true;
              }
    starttime3=millis();
    } */
  //potfct(potsend1, potsend2) and screen1-3() depending on screenstate[index]
  MIDI.read();

  
}

void setuploop()
{
  MIDI.sendProgramChange(1,1);
  for(int i=0; i<12; i++)
    {
      MIDI.sendProgramChange(1,i+1);
      delay (75);
      potfct(gain[0], gaincounter[i]);
      delay (25);
      potfct(pan[0], pancounter[i]);
      delay(25); 
      potfct (pitch[0], pitchcounter[i]);
      delay(25);
      potfct (lp[0], lpcounter[i]); 
      delay(25);
      potfct(hp[0], hpcounter[i]); 
      delay(35);
      potfct(revdec[0],revdeccounter[i]); 
      delay(25);
      potfct(revdw[0], revdwcounter[i]); 
      delay(25); 
      potfct(flangdep[0], flangdepcounter[i]); 
      delay(25); 
      potfct(flangdw[0], flangdwcounter[i]); 
      delay(25); 
      potfct(del_time[0], del_timecounter[i]); 
      delay(25);
      potfct(delfeed[0], delfeedcounter[i]); 
      delay(25); 
      potfct(deldw[0], deldwcounter[i]); 
      delay(25); 

      MIDI.sendNoteOn(reversenote, reverse[i],1); 
      delay(10);
      MIDI.sendNoteOn(mutenote, mute[i],1); 
      delay(10);
      MIDI.sendNoteOn(selrevnote, selrev[i],1);
      delay(10);
      MIDI.sendNoteOn(selflangnote, selflang[i],1); 
      delay(10);
      MIDI.sendNoteOn(seldelnote, seldel[i],1); 
      delay(10);
      MIDI.sendNoteOn(envelopenote, envelope[i], 1);
      delay(10);
      MIDI.sendNoteOn(lfopitchnote, lfostate_pitch[i],1);
      delay(10);
      MIDI.sendNoteOn(lfogainnote, lfostate_gain[i],1);
      delay(10);
      MIDI.sendNoteOn(lfopannote, lfostate_pan[i],1);
      delay(10);
      MIDI.sendNoteOn(lfolpnote, lfostate_lp[i],1);
      delay(10);
      MIDI.sendNoteOn(lfohpnote, lfostate_hp[i],1);
      delay(10);
      MIDI.sendNoteOn(lforevdecnote, lfostate_revdec[i],1);
      delay(10);
      MIDI.sendNoteOn(lforevdwnote, lfostate_revdw[i],1);
      delay(10);
      MIDI.sendNoteOn(lfoflangdepnote, lfostate_flangdep[i],1);
      delay(10);
      MIDI.sendNoteOn(lfoflangdwnote, lfostate_flangdw[i],1);
      delay(10);
      MIDI.sendNoteOn(lfodel_timenote, lfostate_del_time[i],1);
      delay(10);
      MIDI.sendNoteOn(lfodelfeednote, lfostate_delfeed[i],1);
      delay(10);
      MIDI.sendNoteOn(lfodeldwnote, lfostate_deldw[i],1);
      delay(10);
    }
    //sends midi values for each grain
  MIDI.sendProgramChange(1,index+1);  
    MIDI.sendNoteOn(lfo1waveformnote, lfo1waveform,1);  delay(20);
    MIDI.sendNoteOn(lfo2waveformnote, lfo2waveform,1); delay(20);
    MIDI.sendNoteOn(lfo3waveformnote, lfo3waveform,1); delay(20);
  potfct(lfo1f[0], lfo1fcounter[0]); delay(20);
  potfct(lfo2f[0], lfo2fcounter[0]); delay(20);
  potfct(lfo3f[0], lfo3fcounter[0]); delay(20);
  potfct(lfo1a[0], lfo1acounter[0]); delay(20);
  potfct(lfo2a[0], lfo2acounter[0]); delay(20);
  potfct(lfo3a[0], lfo3acounter[0]); delay(20);
   index=0;
   return;
  
}




void setup2() 
{ 
  varinit();
  /*Display*/ {
  display.clearDisplay();
  display.display();
  display.setTextSize(2);            
  display.setTextColor(SSD1306_WHITE);     
  display.setCursor(0,0);             
  display.println("T.Grainer");
  display.println("Connecting...");
  display.display();
  display.clearDisplay();
  delay(20);
  }
  hpcounter[0]=250;
  
  setuploop();
   delay(20);
  delay(3600);
  starttime= millis();
  connected();
  return;
}
//is triggered on received midi note which is send after max patch is started

void connected()
{
  display.clearDisplay();
  display.display();
  display.setTextSize(2);            
  display.setTextColor(SSD1306_WHITE);     
  display.setCursor(0,0);             
  display.println("T.Grainer");
  display.println("Connected.");
  display.display();
  display.clearDisplay();
  return;
}

void screen1()
{
  displayprep(1);
  /*Display Prints*/{
  display.print(screen1string1[index]);
  display.print(" = ");
  myString = "";
  myString.concat(screen1val2[index]);
  display.print(myString); 
  display.println(screen1string2[index]);
  int lpstring = lpcounter[index]*lpscale;
  myString = "";
  myString.concat(lpstring);
  display.print("Fq High Cut = ");
  display.print(myString);
  display.println("Hz");
  myString = "";
  int hpstring = hpcounter[index]*hpscale;
  myString.concat(hpstring);
  display.print("Fq Low Cut = ");
  display.print(myString);
  display.println("Hz");
  }
  display.display();
  screenstate[index]=1;
  return;
}

void screen2()
{
  displayprep(1);  
  /*dipslay prints*/{
  display.print(screen1string1[index]);
  display.print(" = ");
  myString = "";
  myString.concat(screen1val[index]);
  display.print(myString); 
  display.println(screen1string2[index]);
  display.print("LFO ");
  display.print(screen1string1[index]);
  display.print(" = ");
  display.println(screen1string3[index]);
  myString = "";
  int lpstring = lpcounter[index]*lpscale;
  myString.concat(lpstring);
  display.print("Fq High Cut = ");
  display.print(myString);
  display.println("Hz");
  myString = "";
  int hpstring = hpcounter[index]*hpscale;
  myString.concat(hpstring);
  display.print("Fq Low Cut = ");
  display.print(myString);
  display.println("Hz");
  }
  display.display();
  screenstate[index]=2;
  return;
}

void potfct (int x, int y) 
{    
  for (int i2=0; i2<16;i2++)
    {
      sendarr[i2] = y-128*i2;  
      if (sendarr[i2] <0)
        {
          sendarr[i2] = 0;
        } 
      else if (sendarr[i2]>127)
        {
          sendarr[i2]=127;
        }
    }
    //fill sendarray
  if (x!=-1)
    {
      for ( int i=0; i <16; i++)
        {
          MIDI.sendControlChange(x, sendarr[i], i+1);    
        }
      starttime = millis();
    }
    //sends controlchange with sendarr
  else 
    {
      for ( int i=0; i <16; i++)
        {
          MIDI.sendAfterTouch(sendarr[i], i+1);   
        }
        starttime = millis();
    }
    //sends aftertouch with sendarr
  return;
} 

void displayprep(int x)
{
  for (int i=0; i<6; i++)
    {
      display.println("  ");
      display.display();
    } 
  display.setTextSize(x);            
  display.setTextColor(SSD1306_WHITE);     
  display.setCursor(0,0);  
  display.clearDisplay();
  return;
}

void screen3()
{ 
  displayprep(1);           
  /*Display Prints*/{
  display.print(screen1string1[index]);
  display.print(" = ");
  myString = "";
  myString.concat(screen1val2[index]);
  display.print(myString); 
  display.println(screen1string2[index]);
  display.print("LFO ");
  display.print(screen1string1[index]);
  display.print(" = ");
  display.println(screen1string3[index]);
  int lpstring = lpcounter[index]*lpscale;
  myString = "";
  myString.concat(lpstring);
  display.print("Fq High Cut = ");
  display.print(myString);
  display.println("Hz");
  myString = "";
  int hpstring = hpcounter[index]*hpscale;
  myString.concat(hpstring);
  display.print("Fq Low Cut = ");
  display.print(myString);
  display.println("Hz");
  }
  display.display();
  screenstate[index]=3;
  return;
}
void screenwaveform()
{

return;
}

void mgainfct() 
{ 
  counter++;
  if (counter >150 && mgainval>(mgainvalold+3) || counter >150 && mgainval<(mgainvalold-3))
    {
      potfct(-1, mgainval);  
      counter = 0;
      mgainvalold = mgainval;
      }
    //potfct(-1, mgainval)
  return;  
}

void buttons()
{ 
  /*button update*/{
      lpbutton.update();
      gainbutton.update();
      panbutton.update();
      pitchbutton.update();
      hpbutton.update();
      lfo1abutton.update();
      lfo2abutton.update();
      lfo3abutton.update();
      revdecbutton.update();
      revdwbutton.update();
      flangdepbutton.update();
      flangdwbutton.update();
      del_timebutton.update();
      delfeedbutton.update();
      deldwbutton.update();
              } 
  if (gainbutton.pressed()) 
    {
      LFOfct(lfostate_gain, lfogainnote, gaincounter, gainstring, gainscale);
    }
    //LFOfct(int, int, int, string, float) für gain
  else if (panbutton.pressed()) 
    {
      LFOfct(lfostate_pan, lfopannote, pancounter, panstring, panscale, panstring2);
    }  
    //LFOfct(int, int, int, string, float, string) für pan
  else if (pitchbutton.pressed()) 
    {
      LFOfct(lfostate_pitch, lfopitchnote, pitchcounter, pitchstring, pitchscale, 1);
    }
    //LFOfct(int, int, int, string, float, int) für pitch
  else if (lpbutton.pressed()) 
    {
      LFOfct(lfostate_lp, lfolpnote, lpcounter, lpstring, lpscale);
    }
    //LFOfct(int, int, int, string, float) für High Cut
  else if (hpbutton.pressed()) 
    {
      LFOfct(lfostate_hp, lfohpnote, hpcounter, hpstring, hpscale);
    }
    //LFOfct(int, int, int, string, float) für Low Cut
  
else if (revdecbutton.pressed())
{
  LFOfct(lfostate_revdec, lforevdecnote, revdeccounter, revdecstring, revdecscale);
}
else if (revdwbutton.pressed())
{
  LFOfct(lfostate_revdw, lforevdwnote, revdwcounter, revdecstring, revdecscale);
}
else if (flangdepbutton.pressed())
{
  LFOfct(lfostate_flangdep, lfoflangdepnote, flangdepcounter, revdecstring, revdecscale);
}
else if (flangdwbutton.pressed())
{
  LFOfct(lfostate_flangdw, lfoflangdwnote, flangdwcounter, revdecstring, revdecscale);
}
else if (del_timebutton.pressed())
{
  LFOfct(lfostate_del_time, lfodel_timenote, del_timecounter, revdecstring, revdecscale);
}
else if (delfeedbutton.pressed())
{
  LFOfct(lfostate_delfeed, lfodelfeednote, delfeedcounter, revdecstring, revdecscale);
}
else if (deldwbutton.pressed())
{
  LFOfct(lfostate_deldw, lfodeldwnote, deldwcounter, revdecstring, revdecscale);
}
else if (lfo1abutton.pressed())
{
  lfo1waveform++;
  if (lfo1waveform == 5)
  {
    lfo1waveform = 0;
  }
  screen1string1[index]="LFO 1 Waveform";
  screen1val[index]=lfo1waveform;
  screen1string2[index]="";
  screen1();
  MIDI.sendNoteOn (lfo1waveformnote,lfo1waveform,1); 
}
else if (lfo2abutton.pressed())
{
  lfo2waveform++;
  if (lfo2waveform == 5)
  {lfo2waveform = 0;}
    screen1string1[index]="LFO 2 Waveform";
  screen1val[index]=lfo2waveform;
  screen1string2[index]="";
  screen1();
  MIDI.sendNoteOn (lfo2waveformnote,lfo2waveform,1); 
}
else if (lfo3abutton.pressed())
{
  lfo3waveform++;
  if (lfo3waveform == 5)
  {lfo3waveform = 0   ;}
    screen1string1[index]="LFO 3 Waveform";
  screen1val[index]=lfo3waveform;
  screen1string2[index]="";
  screen1();
  MIDI.sendNoteOn (lfo3waveformnote,lfo3waveform,1); 

}
return;
}

void endlesspotfct(int arr[], int arr2[], int z, String arr3[],float scale, int arr4[], int y, int dir) 
{ 
  
      if (dir==1) 
        {  
          arr2[index]=arr2[index]+arr[5];
        }
        //increments counter if rotary encoder turned right
      else
        {  
          arr2[index]=arr2[index]-arr[5];
        }
        //decrements counter if rotary encoder turned left
      if (y!=2)
        {
          starttime3 = millis();
          bools2 = false;
        }
        //prepares screen2() being executed a short time after rotary encoder has stopped turning
      else
        {
          starttime3 = millis();
          bools3 = false;
        }
        //prepares screen3() being executed a short time after rotary encoder has stopped turning

      
      if (arr2[z] < arr[6])
        {
          arr2[z]=arr[6];    
        }
        //checks if counter is below lower limit
      else if ( arr2[z] >arr[7])
        {
          arr2[z]=arr[7];     
        }
        //checks if counter is above upper limit
      potsend1=arr[0];
      potsend2=arr2[z];
      potfct(potsend1,potsend2);
      if (y==2)
        {
          screen1val2[z]=static_cast<float>(arr2[z])*scale;
        }
        //scales screen value (float)
      else 
        {          
          screen1val[z]=arr2[z]*scale;
        }
        //scales screen value (int)
      screen1string1[z]=arr3[0];
      if (y==0 or y==2)
        {
          screen1string2[z]=arr3[1];
        } 
        //sets unit for screen
      else
        {
          if (screen1val[index]<5)
            {
              screen1string2[z]=" L";
            }
          else if (screen1val[index]<30)
            {
              screen1string2[z]=" L-Mitte";
            }
          else if (screen1val[index]<60)
            {
              screen1string2[z]=" Mitte-L";
            }
          else if (screen1val[index]<69)
            {
              screen1string2[z]=" Mitte";
            }
          else if (screen1val[index]<94)
            {
              screen1string2[z]=" Mitte-R";
            }
          else if (screen1val[index]<123)
            {
              screen1string2[z]=" R-Mitte";
            }
          else if (screen1val[index]<129)
            {
              screen1string2[z]=" R";
            }
          panstring2[index]= screen1string2[z];
        }
        //sets Pan unit (left/middle/right) for screen
      myString2 = "";
      if (arr4[index] == 4)
        {
          myString2 = "keiner";
        }
        //sets "No LFO" for screen
      else
        {
          myString2 = String(arr4[index]);
        }
        //sets current LFO for screen
      screen1string3[z]= myString2;
  panpitch[index]=y;
  start=false;
  starttime = millis();
  return;
}

void endlesspotfct(int arr[], int arr2[], int z, String arr3[],float scale, int dir) 
{ 
  
      if (dir==1) 
        {  
          arr2[0]=arr2[0]+arr[5];
        }
        //increments counter if rotary encoder turned right
      else
        {  
          arr2[0]=arr2[0]-arr[5];
        }
        //decrements counter if rotary encoder turned left
      if (arr2[0]<arr[6])
        {
          arr2[0]=arr[6];    
        }
        //checks if counter is below lower limit
      else if ( arr2[0] >arr[7])
        {
          arr2[0]=arr[7];     
        }
        //checks if counter is above upper limit
        potsend1=arr[0];
        potsend2=arr2[0];
        potfct(potsend1,potsend2);
        bools1=false;
        starttime=millis();
        starttime3=millis();
        screen1val2[index]=static_cast<float>(arr2[0])*scale;
        screen1string1[index]=arr3[0];
        if (z ==0)
          {
            screen1string2[index]=arr3[1];  
          }
        else
          {
            screen1string2[index]=arr3[1];
          }
        
        
    //for LFO freq, float value on screen
  return;
}


void LFOfct()
{
  lfo1button.update();
  lfo2button.update();
  lfo3button.update();
  if (lfo1button.pressed() ) 
    {
      lfo=1;
      screen1string1[index]=lfo1fstring[0];
      screen1string2[index]=lfo1fstring[1];
      screen1val2[index]=static_cast<float>(lfo1fcounter[0])*lfofscale;
      screen1();
    }
    //sets lfo=1, screen1val2 and screen1strings, screen1()
  else if (lfo2button.pressed() ) 
    {
      lfo=2;
      screen1string1[index]=lfo2fstring[0];
      screen1string2[index]=lfo1fstring[1];
      screen1val2[index]=static_cast<float>(lfo2fcounter[0])*lfofscale;
      screen1();
    }
    //sets lfo=2, screen1val2 and screen1strings, screen1()
  else if (lfo3button.pressed() ) 
    {
      lfo=3;
      screen1string1[index]=lfo3fstring[0];
      screen1string2[index]=lfo1fstring[1];
      screen1val[index]=static_cast<float>(lfo3fcounter[0])*lfofscale;
      screen1();
    }
    //sets lfo=3, screen1val2 and screen1strings, screen1()
  return;
}

void LFOfct (int arr[], int x, int arr2[],String arr3[], float scale) 
{ 
  if (arr[index] == 4)
    {       
      arr[index] = lfo;
    }
  else if (arr[index] == lfo)
    {
        arr[index] = 4;  
    }
  else
    {
        arr[index] = lfo;
    }
  MIDI.sendNoteOn (x,arr[index],1); 
  starttime = millis();
  screen1val[index]=arr2[index]*scale;
  screen1string1[index]=arr3[0];
  screen1string2[index]=arr3[1];
  myString2 = "";
  if (arr[index] == 4)
    {
      myString2 = "keiner";
    }
  else
    {
      myString2 = String(arr[index]);
    }
  screen1string3[index]=myString2;
  screen2();  
  return;    
} 
//sends midi note with lfo, screen2()

void LFOfct (int arr[], int x, int arr2[],String arr3[], float scale, int y) 
{ 
  if (arr[index] == 4)
    {       
      arr[index] = lfo;
    }
  else if (arr[index] == lfo)
      {
        arr[index] = 4;  
      }
  else
      {
        arr[index] = lfo;
      }
  MIDI.sendNoteOn (x,arr[index],1);    
  starttime = millis();
  screen1val2[index]=arr2[index]*scale;
  screen1string1[index]=arr3[0];
  screen1string2[index]=arr3[1];
  myString2 = "";
  if (arr[index] == 4)
    {
      myString2 = "keiner";
    }
  else
    {
      myString2 = String(arr[index]);
    }
  screen1string3[index]=myString2;
  screen3();  
  screenstate[index]=3;
  return;    
}
//sends midi note with lfo, screen3() cause pitch needs float value on screen

void LFOfct (int arr[], int x, int arr2[],String arr3[], float scale, String arr4[])
{ 
  if (arr[index] == 4)
    {       
      arr[index] = lfo;
    }
  else if (arr[index] == lfo)
      {
        arr[index] = 4;  
      }
  else
      {
        arr[index] = lfo;
      }
  MIDI.sendNoteOn (x,arr[index],1); 
  starttime = millis();
  if (panpitch[index]==0 or panpitch[index]==1)
    {
      screen1val[index]=arr2[index]*scale;
    }
  else
    {
      screen1val2[index]=arr2[index]*scale;
    }
  screen1string1[index]=arr3[0];
  screen1string2[index]=arr4[index];
  myString2 = "";
  if (arr[index] == 4)
    {
      myString2 = "keiner";
    }
  else
    {
      myString2 = String(arr[index]);
    }
  screen1string3[index]=myString2;
  screen2();  
  screenstate[index]=2;
  return;    
}
//same but with changing units for pan not elegant but w/e

void Ard2()
{ 
  if (Serial1.available())
  {
  while (Serial1.available()) 
    {
      int x = Serial1.read();
         if (x>1 && x<13)
        { 
          eeprom1counter++;
            if (eeprom1counter > 250)
            {   
              eeprom2counter++;
              eeprom1counter=1;
              if (eeprom2counter > 200)
                {eeprom2counter = 0;
                  var=4003;
                  var2=4005;
                  EEPROM.update(4006, 1);
              }
            }
          if (x<7)
            {   
                displayprep(2);
                display.print("State ");
                display.println(x-1);
                display.println("saving...");
                display.display();
                EEPROM.update(var, x);
                
                offset1 = 333*(x-2);  
                
                for(int i=1; i<333; i++)
                    {
                        if (i<=12)
                            {
                              int gaincountersave=gaincounter[i-1]*0.25;
                              EEPROM.update(i+offset1, gaincountersave);}
                        else if (i<=24)
                            {
                              int lfostatesave=lfostate_gain[i-13];
                              EEPROM.update(i+offset1, lfostatesave);}
                        else if (i<=36)
                            {
                              int pancountersave=pancounter[i-25];
                              EEPROM.update(i+offset1, pancountersave);}
                        else if (i<=48)
                            {
                              int lfopansave=lfostate_pan[i-37];
                              EEPROM.update(i+offset1, lfopansave);}
                        else if (i<=60)
                            { pitchcountersave=pitchcounter[i-49]*0.5;
                              EEPROM.update(i+offset1, pitchcountersave);}
                        else if (i<=72)
                            { int lfopitchsave=lfostate_pitch[i-61];
                              EEPROM.update(i+offset1, lfopitchsave);}
                        else if (i<=84)
                            { int lpcountersave= lpcounter[i-73]*0.1275;
                              EEPROM.update(i+offset1,lpcountersave);}
                        else if (i<=96)
                            { int lfolpsave=lfostate_lp[i-85];
                              EEPROM.update(i+offset1, lfolpsave);}
                        else if (i<=108)
                            {int hpcountersave= hpcounter[i-97]*0.1275;
                              EEPROM.update(i+offset1, hpcountersave);}
                        else if (i<=120)
                            { int lfohpsave=lfostate_hp[i-109];
                              EEPROM.update(i+offset1, lfohpsave);}
                        else if (i<=132)
                            { int revdeccountersave=revdeccounter[i-121]*revdecscale;
                              EEPROM.update(i+offset1, revdeccountersave);}
                        else if (i<=144)
                            { int lforevdecsave=lfostate_revdec[i-133];
                              EEPROM.update(i+offset1, lforevdecsave);}
                        else if (i<=156)
                            { int revdwcountersave=revdwcounter[i-145]*revdwscale;
                              EEPROM.update(i+offset1, revdwcountersave);}
                        else if (i<=168)
                            {int lforevdwsave=lfostate_revdw[i-157];
                              EEPROM.update(i+offset1, lforevdwsave);}
                        else if (i<=180)
                            { int flangdepcountersave=flangdepcounter[i-169]*flangdepscale;
                              EEPROM.update(i+offset1, flangdepcountersave);}
                        else if (i<=192)
                            { int lfoflangdepsave=lfostate_flangdep[i-181];
                              EEPROM.update(i+offset1, lfoflangdepsave);}
                        else if (i<=204)
                            { int flangdwcountersave=flangdwcounter[i-193]*flangdwscale;
                              EEPROM.update(i+offset1, flangdwcountersave);}
                        else if (i<=216)
                            { int lfoflangdwsave=lfostate_flangdw[i-205];
                              EEPROM.update(i+offset1, lfoflangdwsave);}
                        else if (i<=228)
                            {  int envelopestate= envelope[i-217];
                              EEPROM.update(i+offset1, envelopestate);}
                        else if (i<=240)
                            {  }
                        else if (i<=252)
                            { int del_timecountersave=del_timecounter[i-241]*del_timescale;
                              EEPROM.update(i+offset1, del_timecountersave);}
                        else if (i<=264)
                            { int lfodel_timesave=lfostate_del_time[i-253];
                              EEPROM.update(i+offset1, lfodel_timesave);}
                        else if (i<=276)
                            { int delfeedcountersave=delfeedcounter[i-265]*delfeedscale;
                              EEPROM.update(i+offset1, delfeedcountersave);}
                        else if (i<=288)
                            { int lfodelfeedsave=lfostate_delfeed[i-277];
                              EEPROM.update(i+offset1, lfodelfeedsave);}
                        else if (i<=300)
                            { int deldwcountersave=deldwcounter[i-289]*deldwscale;
                              EEPROM.update(i+offset1, deldwcountersave);}
                        else if (i<=312)
                            { int lfodeldwsave=lfostate_deldw[i-301];
                              EEPROM.update(i+offset1, lfodeldwsave);}
                        else if (i==313)
                            {
                                for (int j=0; j<12; j++)
                                {
                                    if (reverse[j]==1)
                                    {
                                        if (mute[j]==1)
                                            {
                                                if (selrev[j]==1)
                                                    {
                                                        if (selflang[j]==1)
                                                            {
                                                                if (seldel[j]==1)
                                                                    {EEPROM.update(j+313, 1);}
                                                                else
                                                                    {EEPROM.update(j+313, 2);}
                                                            }
                                                        else
                                                            {
                                                                if (seldel[j]==1)
                                                                    {EEPROM.update(j+313, 3);}
                                                                else
                                                                    {EEPROM.update(j+313, 4);}
                                                            }
                                                    }
                                                else 
                                                    {
                                                        if (selflang[j]==1)
                                                            {
                                                                if (seldel[j]==1)
                                                                    {EEPROM.update(j+313, 5);}
                                                                else
                                                                    {EEPROM.update(j+313, 6);}
                                                            }
                                                        else
                                                            {
                                                                if (seldel[j]==1)
                                                                    {EEPROM.update(j+313, 7);}
                                                                else
                                                                    {EEPROM.update(j+313, 8);}
                                                            }
                                                    }
                                            }
                                        else
                                            {
                                                if (selrev[j]==1)
                                                    {
                                                        if (selflang[j]==1)
                                                            {
                                                                if (seldel[j]==1)
                                                                    {EEPROM.update(j+313, 9);}
                                                                else
                                                                    {EEPROM.update(j+313, 10);}
                                                            }
                                                        else
                                                            {
                                                                if (seldel[j]==1)
                                                                    {EEPROM.update(j+313, 11);}
                                                                else
                                                                    {EEPROM.update(j+313, 12);}
                                                            }
                                                    }
                                                else 
                                                    {
                                                        if (selflang[j]==1)
                                                            {
                                                                if (seldel[j]==1)
                                                                    {EEPROM.update(j+313, 13);}
                                                                else
                                                                    {EEPROM.update(j+313, 14);}
                                                            }
                                                        else
                                                            {
                                                                if (seldel[j]==1)
                                                                    {EEPROM.update(j+313, 15);}
                                                                else
                                                                    {EEPROM.update(j+313, 16);}
                                                            }
                                                    }
                                            }
                                    }
                                    else
                                    {
                                            if (mute[j]==1)
                                                {
                                                    if (selrev[j]==1)
                                                        {
                                                            if (selflang[j]==1)
                                                                {
                                                                    if (seldel[j]==1)
                                                                        {EEPROM.update(j+313, 17);}
                                                                    else
                                                                        {EEPROM.update(j+313, 18);}
                                                                }
                                                            else
                                                                {
                                                                    if (seldel[j]==1)
                                                                        {EEPROM.update(j+313, 19);}
                                                                    else
                                                                        {EEPROM.update(j+313, 20);}
                                                                }
                                                        }
                                                    else 
                                                        {
                                                            if (selflang[j]==1)
                                                                {
                                                                    if (seldel[j]==1)
                                                                        {EEPROM.update(j+313, 21);}
                                                                    else
                                                                        {EEPROM.update(j+313, 22);}
                                                                }
                                                            else
                                                                {
                                                                    if (seldel[j]==1)
                                                                        {EEPROM.update(j+313, 23);}
                                                                    else
                                                                        {EEPROM.update(j+313, 24);}
                                                                }
                                                        }
                                                }
                                            else
                                                {
                                                    if (selrev[j]==1)
                                                        {
                                                            if (selflang[j]==1)
                                                                {
                                                                    if (seldel[j]==1)
                                                                        {EEPROM.update(j+313, 25);}
                                                                    else
                                                                        {EEPROM.update(j+313, 26);}
                                                                }
                                                            else
                                                                {
                                                                    if (seldel[j]==1)
                                                                        {EEPROM.update(j+313, 27);}
                                                                    else
                                                                        {EEPROM.update(j+313, 28);}
                                                                }
                                                        }
                                                    else 
                                                        {
                                                            if (selflang[j]==1)
                                                                {
                                                                    if (seldel[j]==1)
                                                                        {EEPROM.update(j+313, 29);}
                                                                    else
                                                                        {EEPROM.update(j+313, 30);}
                                                                }
                                                            else
                                                                {
                                                                    if (seldel[j]==1)
                                                                        {EEPROM.update(j+313, 31);}
                                                                    else
                                                                        {EEPROM.update(j+313, 32);}
                                                                }
                                                        }
                                                }
                                        }
                                }
                            }
                        else if (i==325)
                            { int lfo1fsave;
                            lfo1fsave = lfo1fcounter[0]*0.1275; //*0.1275
                            EEPROM.update(i+offset1, lfo1fsave); }
                                
                        else if (i==326)
                            {int lfo2fsave = lfo2fcounter[0]*0.1275;
                              EEPROM.update(i+offset1, lfo2fsave);}
                        else if (i==327)
                            {int lfo3fsave = lfo3fcounter[0]*0.1275;
                              EEPROM.update(i+offset1, lfo3fsave);}
                        else if (i==328)
                            {int lfo1asave = lfo1acounter[0]*0.1275;
                              EEPROM.update(i+offset1, lfo1asave);}
                        else if (i==329)
                            {int lfo2asave = lfo2acounter[0]*0.1275;
                              EEPROM.update(i+offset1, lfo2asave);}
                        else if (i==330)
                            {int lfo3asave = lfo3acounter[0]*0.1275;
                              EEPROM.update(i+offset1, lfo3asave);}
                        else if (i==331)
                            {EEPROM.update(i+offset1, lfo1waveform);}
                        else if (i==332)
                            {EEPROM.update(i+offset1, lfo2waveform);}
                        else if (i==333)
                            {EEPROM.update(i+offset1, lfo3waveform);}
                        else {}
                    } 
                    //Save
                  
                                   
                displayprep(2); 
                display.print("State ");
                display.println(x-1);
                display.print("saved.");
                
                display.display();
            }
          else if (x <12)
            {
               loadstate(x-7);
            }
          else if (x==12)
            {
                if (EEPROM.read(var2) == 0)
                    {EEPROM.update(var2, 1);}
                else
                    {EEPROM.update(var2, 0);}
            }
          EEPROM.update(4000, eeprom1counter);
          EEPROM.update(4001, eeprom2counter);
        }
      else
      { 
        switch (x)
        {
          case 13:
              if(envelope[index]==1)
                {envelope[index]=0;}
              else
                {envelope[index]=1;}
              MIDI.sendNoteOn(envelopenote, envelope[index], 1);
              break;
          case 14:
              if(reverse[index]==1)
                {reverse[index]=0;}
              else
                {reverse[index]=1;}
              MIDI.sendNoteOn(reversenote, reverse[index], 1);
              break;
          case 15:
              if(mute[index]==1)
                {mute[index]=0;}
              else
                {mute[index]=1;}
              MIDI.sendNoteOn(mutenote, mute[index], 1);
              break;
          case 16:
              if(sellp[index]==1)
                {sellp[index]=0;}
              else
                {sellp[index]=1;}
              MIDI.sendNoteOn(sellpnote, sellp[index], 1);
              break;
          case 17:
              if(selhp[index]==1)
                {selhp[index]=0;}
              else
                {selhp[index]=1;}
              MIDI.sendNoteOn(selhpnote, selhp[index], 1);
                break;
          case 18:
              if(selrev[index]==1)
                {selrev[index]=0;}
              else
                {selrev[index]=1;}
               MIDI.sendNoteOn(selrevnote, selrev[index], 1);
                break;
          case 19:
              if(selflang[index]==1)
                {selflang[index]=0;}
              else
                {selflang[index]=1;}
               MIDI.sendNoteOn(selflangnote, selflang[index], 1);
                break;
          case 20:
              if(seldel[index]==1)
                {seldel[index]=0;}
              else
                {seldel[index]=1;}
                MIDI.sendNoteOn(seldelnote, seldel[index], 1);
                break;
          case 21:
              lforeset(1);
                break;
          case 22:
                 lforeset(2);
                break;
          case 23:
                lforeset(3);
                break;
          case 24:
                sampleold=sample;
                sample=1;
                sgsel();
                break;
          case 25:
                sampleold=sample;
                sample=2;
                sgsel();
                break;  
          case 26:
                sampleold=sample;
                sample=3;
                sgsel();
                break;  
          case 27:
                grainold=grain;
                grain=1;
                sgsel();
                break;
          case 28:
                grainold=grain;
                grain=2;
                sgsel();
                break;  
          case 29:
                grainold=grain;
                grain=3;
                sgsel();
                break;      
          case 30:
                grainold=grain;
                grain=4;
                sgsel();
                break;  
        }
      }
    }
  leds();
  }
return;
}
//handles communication with 2. arduino

void sgsel()
{
  switch (sample)
    {
      case 1:
          switch (grain)
              { 
                case 1: 
                  index = 0; 
                  break; 
                case 2: 
                  index = 1; 
                  break;
                case 3: 
                  index = 2; 
                  break;
                case 4: 
                  index = 3; 
                  break; 
              }
      break;
      case 2:
          switch (grain)
              { 
                case 1: 
                  index = 4; 
                  break; 
                case 2: 
                  index = 5; 
                  break;
                case 3: 
                  index = 6; 
                  break;
                case 4: 
                  index = 7; 
                  break; 
            }
      break;
      case 3:
          switch (grain)
              { 
                case 1: 
                  index = 8; 
                  break; 
                case 2: 
                  index = 9; 
                  break;
                case 3: 
                  index = 10; 
                  break;
                case 4: 
                  index = 11; 
                  break; 
              }
         break;
    }
  MIDI.sendProgramChange(1,index+1);
  return;
}

void leds()
{
switch (reverse[index])
{
  case 0:
    Serial1.write(16);
    break;
  case 1:
    Serial1.write(15);
    break;
}
delay(2);
switch (envelope[index])
{
  case 0:
    Serial1.write(18);
    break;
  case 1:
    Serial1.write(17);
    break;
}
delay(1);
switch (mute[index])
{
  case 0:
    Serial1.write(20);
    break;
  case 1:
    Serial1.write(19);
    break;
}
delay(1);
switch (sellp[index])
{
  case 0:
    Serial1.write(22);
    break;
  case 1:
    Serial1.write(21);
    break;
}
delay(1);
switch (selhp[index])
{
  case 0:
    Serial1.write(24);
    break;
  case 1:
    Serial1.write(23);
    break;
}
delay(1);
switch (selrev[index])
{
  case 0:
    Serial1.write(26);
    break;
  case 1:
    Serial1.write(25);
    break;
}
delay(1);
switch (selflang[index])
{
  case 0:
    Serial1.write(28);
    break;
  case 1:
    Serial1.write(27);
    break;
}
delay(1);
switch (seldel[index])
{
  case 0:
    Serial1.write(30);
    break;
  case 1:
    Serial1.write(29);
    break;
}
return;
}

void lforeset(int x)
{
  for (int i=0; i<12; i++)
  {
    if (lfostate_gain[i]==x)
        {lfostate_gain[i]=4;
        MIDI.sendNoteOn(lfogainnote, lfostate_gain[i],1);
        }
    if (lfostate_pan[i]==x)
        {lfostate_pan[i]=4;
        MIDI.sendNoteOn(lfopannote, lfostate_pan[i],1);
        }
    if (lfostate_pitch[i]==x)
        {lfostate_pitch[i]=4;
        MIDI.sendNoteOn(lfopitchnote, lfostate_pitch[i],1);
        }
    if (lfostate_lp[i]==x)
        {lfostate_lp[i]=4;
        MIDI.sendNoteOn(lfolpnote, lfostate_lp[i],1);
        }
    if (lfostate_hp[i]==x)
        {lfostate_hp[i]=4;
        MIDI.sendNoteOn(lfohpnote, lfostate_hp[i],1);
        }
    if (lfostate_revdec[i]==x)
        {lfostate_revdec[i]=4;
        MIDI.sendNoteOn(lforevdecnote, lfostate_revdec[i],1);
        }
    if (lfostate_revdw[i]==x)
        {lfostate_revdw[i]=4;
        MIDI.sendNoteOn(lforevdwnote, lfostate_revdw[i],1);
        }
    if (lfostate_flangdep[i]==x)
        {lfostate_flangdep[i]=4;
        MIDI.sendNoteOn(lfoflangdepnote, lfostate_flangdep[i],1);
        }
      
    if (lfostate_flangdw[i]==x)
        {lfostate_flangdw[i]=4;
        MIDI.sendNoteOn(lfoflangdwnote, lfostate_flangdw[i],1);
        }
    if (lfostate_del_time[i]==x)
        {lfostate_del_time[i]=4;
        MIDI.sendNoteOn(lfodel_timenote, lfostate_del_time[i],1);
        }
    if (lfostate_delfeed[i]==x)
        {lfostate_delfeed[i]=4;
        MIDI.sendNoteOn(lfodelfeednote, lfostate_delfeed[i],1);}
    if (lfostate_deldw[i]==x)
        {lfostate_deldw[i]=4;
        MIDI.sendNoteOn(lfodeldwnote, lfostate_deldw[i],1);
        }
  }
return;  
}

void loadstate(int x)
{
    displayprep(2);
    display.print("State ");
    display.println(x+1);
    display.println("loading...");
    display.display();
    int offset1;
    offset1 = 333*x;  
    for(int i=1; i<334; i++)
                        {
                            if (i<=12)
                                {gaincounter[i]=EEPROM.read(i+offset1)/0.25;}
                            else if (i<=24)
                                {lfostate_gain[i-13]=EEPROM.read(i+offset1);}
                            else if (i<=36)
                                {pancounter[i-25]=EEPROM.read(i+offset1);}
                            else if (i<=48)
                                {lfostate_pan[i-37]=EEPROM.read(i+offset1);}
                            else if (i<=60)
                                { 
                                  pitchcounter[i-49]=EEPROM.read(i+offset1);
                                  pitchcounter[i-49]=pitchcounter[i-49]/0.5;}
                            else if (i<=72)
                                {lfostate_pitch[i-61]=EEPROM.read(i+offset1);}
                            else if (i<=84)
                                {lpcounter[i-73]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i<=96)
                                {lfostate_lp[i-85]=EEPROM.read(i+offset1 );}
                            else if (i<=108)
                                {hpcounter[i-97]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i<=120)
                                {lfostate_hp[i-109]=EEPROM.read(i+offset1);}
                            else if (i<=132)
                                {revdeccounter[i-121]=EEPROM.read(i+offset1)/revdecscale;}
                            else if (i<=144)
                                {lfostate_revdec[i-133]=EEPROM.read(i+offset1);}
                            else if (i<=156)
                                {revdwcounter[i-145]=EEPROM.read(i+offset1)/revdwscale;}
                            else if (i<=168)
                                {lfostate_revdw[i-157]=EEPROM.read(i+offset1);}
                            else if (i<=180)
                                {flangdepcounter[i-169]=EEPROM.read(i+offset1)/flangdepscale;}
                            else if (i<=192)
                                {lfostate_flangdep[i-181]=EEPROM.read(i+offset1);}
                            else if (i<=204)
                                {flangdwcounter[i-193]=EEPROM.read(i+offset1)/flangdwscale;}
                            else if (i<=216)
                                {lfostate_flangdw[i-205]=EEPROM.read(i+offset1);}
                            else if (i<=228)
                                {envelope[i-217]=EEPROM.read(i+offset1);}
                            else if (i<=240)
                                {}
                            else if (i<=252)
                                {del_timecounter[i-241]=EEPROM.read(i+offset1)/del_timescale;}
                            else if (i<=264)
                                {lfostate_del_time[i-253]=EEPROM.read(i+offset1);}
                            else if (i<=276)
                                {delfeedcounter[i-265]=EEPROM.read(i+offset1)/delfeedscale;}
                            else if (i<=288)
                                {lfostate_delfeed[i-277]=EEPROM.read(i+offset1);}
                            else if (i<=300)
                                {deldwcounter[i-289]=EEPROM.read(i+offset1)/deldwscale;}
                            else if (i<=312)
                                {lfostate_deldw[i-301]=EEPROM.read(i+offset1);}
                            else if (i<=324)
                                {
                                    switch(EEPROM.read(i+offset1))
                                    {
                                        case 1: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=1;
                                            selrev[i-313]=1;
                                            selflang[i-313]=1;
                                            seldel[i-313]=1;
                                            break;}
                                        case 2: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=1;
                                            selrev[i-313]=1;
                                            selflang[i-313]=1;
                                            seldel[i-313]=0;
                                            break;}
                                        case 3: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=1;
                                            selrev[i-313]=1;
                                            selflang[i-313]=0;
                                            seldel[i-313]=1;
                                            break;}
                                        case 4: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=1;
                                            selrev[i-313]=1;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                        case 5: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=1;
                                            selrev[i-313]=0;
                                            selflang[i-313]=1;
                                            seldel[i-313]=1;
                                            break;}
                                        case 6: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=1;
                                            selrev[i-313]=0;
                                            selflang[i-313]=1;
                                            seldel[i-313]=0;
                                            break;}
                                        case 7: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=1;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=1;
                                            break;}
                                        case 8: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=1;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                        case 9: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=0;
                                            selrev[i-313]=1;
                                            selflang[i-313]=1;
                                            seldel[i-313]=1;
                                            break;}
                                        case 10: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=0;
                                            selrev[i-313]=1;
                                            selflang[i-313]=1;
                                            seldel[i-313]=0;
                                            break;}
                                        case 11: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=0;
                                            selrev[i-313]=1;
                                            selflang[i-313]=0;
                                            seldel[i-313]=1;
                                            break;}
                                        case 12: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=0;
                                            selrev[i-313]=1;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                        case 13: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=1;
                                            seldel[i-313]=1;
                                            break;}
                                        case 14: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=1;
                                            seldel[i-313]=0;
                                            break;}
                                        case 15: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=1;
                                            break;}
                                        case 16: 
                                            {reverse[i-313]=1;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                        case 17: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=1;
                                            selrev[i-313]=1;
                                            selflang[i-313]=1;
                                            seldel[i-313]=1;
                                            break;}
                                        case 18: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=1;
                                            selrev[i-313]=1;
                                            selflang[i-313]=1;
                                            seldel[i-313]=0;
                                            break;}
                                        case 19: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=1;
                                            selrev[i-313]=1;
                                            selflang[i-313]=0;
                                            seldel[i-313]=1;
                                            break;}
                                        case 20: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=1;
                                            selrev[i-313]=1;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                        case 21: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=1;
                                            selrev[i-313]=0;
                                            selflang[i-313]=1;
                                            seldel[i-313]=1;
                                            break;}
                                        case 22: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=1;
                                            selrev[i-313]=0;
                                            selflang[i-313]=1;
                                            seldel[i-313]=0;
                                            break;}
                                        case 23: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=1;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=1;
                                            break;}
                                        case 24: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=1;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                        case 25: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=1;
                                            selflang[i-313]=1;
                                            seldel[i-313]=1;
                                            break;}
                                        case 26: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=1;
                                            selflang[i-313]=1;
                                            seldel[i-313]=0;
                                            break;}
                                        case 27: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=1;
                                            selflang[i-313]=0;
                                            seldel[i-313]=1;
                                            break;}
                                        case 28: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=1;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                        case 29: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=1;
                                            seldel[i-313]=1;
                                            break;}
                                        case 30: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=1;
                                            seldel[i-313]=0;
                                            break;}
                                        case 31: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=1;
                                            break;}
                                        case 32: 
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                        default:
                                            {reverse[i-313]=0;
                                            mute[i-313]=0;
                                            selrev[i-313]=0;
                                            selflang[i-313]=0;
                                            seldel[i-313]=0;
                                            break;}
                                    }
                                }
                            else if (i==325)
                                {lfo1fcounter[0]=EEPROM.read(i+offset1);
                                  lfo1fcounter[0]=lfo1fcounter[0]/0.1275;
                                  
                                }
                            else if (i==326)
                                {lfo2fcounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==327)
                                {lfo3fcounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==328)
                                {lfo1acounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==329)
                                {lfo2acounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==330)
                                {lfo3acounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==331)
                                {lfo1waveform=EEPROM.read(i+offset1);}
                            else if (i==332)
                                {lfo2waveform=EEPROM.read(i+offset1);}
                            else if (i==333)
                                {lfo3waveform=EEPROM.read(i+offset1);}
                            else
                                {}
                        } 
    setuploop();
    displayprep(2);
    display.print("State ");
    display.println(x+1);
    display.println("loaded.");
    display.display();
    leds();
return;
}

void startloadstate(int x)
{
    
    int offset1;
    offset1 = 333*x;  
    for(int i=1; i<333; i++)
                        {
                            if (i<=12)
                                {gaincounter[i]=EEPROM.read(i+offset1)/0.25;}
                            else if (i<=24)
                                {lfostate_gain[i-13]=EEPROM.read(i+offset1);}
                            else if (i<=36)
                                {pancounter[i-25]=EEPROM.read(i+offset1);}
                            else if (i<=48)
                                {lfostate_pan[i-37]=EEPROM.read(i+offset1);}
                            else if (i<=60)
                                { 
                                  pitchcounter[i-49]=EEPROM.read(i+offset1);
                                  pitchcounter[i-49]=pitchcounter[i-49]/0.5;}
                            else if (i<=72)
                                {lfostate_pitch[i-61]=EEPROM.read(i+offset1);}
                            else if (i<=84)
                                {lpcounter[i-73]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i<=96)
                                {lfostate_lp[i-85]=EEPROM.read(i+offset1 );}
                            else if (i<=108)
                                {hpcounter[i-97]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i<=120)
                                {lfostate_hp[i-109]=EEPROM.read(i+offset1);}
                            else if (i<=132)
                                {revdeccounter[i-121]=EEPROM.read(i+offset1)/revdecscale;}
                            else if (i<=144)
                                {lfostate_revdec[i-133]=EEPROM.read(i+offset1);}
                            else if (i<=156)
                                {revdwcounter[i-145]=EEPROM.read(i+offset1)/revdwscale;}
                            else if (i<=168)
                                {lfostate_revdw[i-157]=EEPROM.read(i+offset1);}
                            else if (i<=180)
                                {flangdepcounter[i-169]=EEPROM.read(i+offset1)/flangdepscale;}
                            else if (i<=192)
                                {lfostate_flangdep[i-181]=EEPROM.read(i+offset1);}
                            else if (i<=204)
                                {flangdwcounter[i-193]=EEPROM.read(i+offset1)/flangdwscale;}
                            else if (i<=216)
                                {lfostate_flangdw[i-205]=EEPROM.read(i+offset1);}
                            else if (i<=228)
                                {envelope[i-217]=EEPROM.read(i+offset1);}
                            else if (i<=240)
                                {
                                }
                            else if (i<=252)
                                {del_timecounter[i-241]=EEPROM.read(i+offset1)/del_timescale;}
                            else if (i<=264)
                                {lfostate_del_time[i-253]=EEPROM.read(i+offset1);}
                            else if (i<=276)
                                {delfeedcounter[i-265]=EEPROM.read(i+offset1)/delfeedscale;}
                            else if (i<=288)
                                {lfostate_delfeed[i-277]=EEPROM.read(i+offset1);}
                            else if (i<=300)
                                {deldwcounter[i-289]=EEPROM.read(i+offset1)/deldwscale;}
                            else if (i<=312)
                                {lfostate_deldw[i-301]=EEPROM.read(i+offset1);}
                            else if (i<=324)
                                {
                                    switch(EEPROM.read(i+offset1))
                                    {
                                        case 1: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=true;
                                            selrev[i-313]=true;
                                            selflang[i-313]=true;
                                            seldel[i-313]=true;
                                            break;}
                                        case 2: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=true;
                                            selrev[i-313]=true;
                                            selflang[i-313]=true;
                                            seldel[i-313]=false;
                                            break;}
                                        case 3: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=true;
                                            selrev[i-313]=true;
                                            selflang[i-313]=false;
                                            seldel[i-313]=true;
                                            break;}
                                        case 4: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=true;
                                            selrev[i-313]=true;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                        case 5: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=true;
                                            selrev[i-313]=false;
                                            selflang[i-313]=true;
                                            seldel[i-313]=true;
                                            break;}
                                        case 6: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=true;
                                            selrev[i-313]=false;
                                            selflang[i-313]=true;
                                            seldel[i-313]=false;
                                            break;}
                                        case 7: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=true;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=true;
                                            break;}
                                        case 8: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=true;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                        case 9: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=false;
                                            selrev[i-313]=true;
                                            selflang[i-313]=true;
                                            seldel[i-313]=true;
                                            break;}
                                        case 10: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=false;
                                            selrev[i-313]=true;
                                            selflang[i-313]=true;
                                            seldel[i-313]=false;
                                            break;}
                                        case 11: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=false;
                                            selrev[i-313]=true;
                                            selflang[i-313]=false;
                                            seldel[i-313]=true;
                                            break;}
                                        case 12: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=false;
                                            selrev[i-313]=true;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                        case 13: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=true;
                                            seldel[i-313]=true;
                                            break;}
                                        case 14: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=true;
                                            seldel[i-313]=false;
                                            break;}
                                        case 15: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=true;
                                            break;}
                                        case 16: 
                                            {reverse[i-313]=true;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                        case 17: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=true;
                                            selrev[i-313]=true;
                                            selflang[i-313]=true;
                                            seldel[i-313]=true;
                                            break;}
                                        case 18: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=true;
                                            selrev[i-313]=true;
                                            selflang[i-313]=true;
                                            seldel[i-313]=false;
                                            break;}
                                        case 19: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=true;
                                            selrev[i-313]=true;
                                            selflang[i-313]=false;
                                            seldel[i-313]=true;
                                            break;}
                                        case 20: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=true;
                                            selrev[i-313]=true;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                        case 21: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=true;
                                            selrev[i-313]=false;
                                            selflang[i-313]=true;
                                            seldel[i-313]=true;
                                            break;}
                                        case 22: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=true;
                                            selrev[i-313]=false;
                                            selflang[i-313]=true;
                                            seldel[i-313]=false;
                                            break;}
                                        case 23: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=true;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=true;
                                            break;}
                                        case 24: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=true;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                        case 25: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=true;
                                            selflang[i-313]=true;
                                            seldel[i-313]=true;
                                            break;}
                                        case 26: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=true;
                                            selflang[i-313]=true;
                                            seldel[i-313]=false;
                                            break;}
                                        case 27: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=true;
                                            selflang[i-313]=false;
                                            seldel[i-313]=true;
                                            break;}
                                        case 28: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=true;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                        case 29: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=true;
                                            seldel[i-313]=true;
                                            break;}
                                        case 30: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=true;
                                            seldel[i-313]=false;
                                            break;}
                                        case 31: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=true;
                                            break;}
                                        case 32: 
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                        default:
                                            {reverse[i-313]=false;
                                            mute[i-313]=false;
                                            selrev[i-313]=false;
                                            selflang[i-313]=false;
                                            seldel[i-313]=false;
                                            break;}
                                    }
                                }
                            else if (i==325)
                                {lfo1fcounter[0]=EEPROM.read(i+offset1);
                                  lfo1fcounter[0]=lfo1fcounter[0]/0.1275;
                                  
                                }
                            else if (i==326)
                                {lfo2fcounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==327)
                                {lfo3fcounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==328)
                                {lfo1acounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==329)
                                {lfo2acounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==330)
                                {lfo3acounter[0]=EEPROM.read(i+offset1)/0.1275;}
                            else if (i==331)
                                {lfo1waveform=EEPROM.read(i+offset1);}
                            else if (i==332)
                                {lfo2waveform=EEPROM.read(i+offset1);}
                            else if (i==333)
                                {lfo3waveform=EEPROM.read(i+offset1);}
                            else
                                {}
                        } 
    setuploop();
    leds();
     
return;
}

void encoders()
{
  static int lfo1fencoderpos=0;
  lfo1fencoder.tick();
  
  int lfo1fencodernewPos = lfo1fencoder.getPosition();
  
  static int lfo2fencoderpos=0;
  lfo2fencoder.tick();
  int lfo2fencodernewPos = lfo2fencoder.getPosition();
  
  static int lfo3fencoderpos=0;
  lfo3fencoder.tick();
  int lfo3fencodernewPos = lfo3fencoder.getPosition();

  static int lfo1aencoderpos=0;
  lfo1aencoder.tick();
  int lfo1aencodernewPos = lfo1aencoder.getPosition();
 
  static int gainencoderpos=0;
  gainencoder.tick();
  int gainencodernewPos = gainencoder.getPosition();

  static int hpencoderpos=0;
  hpencoder.tick();
  int hpencodernewPos = hpencoder.getPosition();
 
    static int lpencoderpos=0;
  lpencoder.tick();
  int lpencodernewPos = lpencoder.getPosition();

  static int panencoderpos=0;
  panencoder.tick();
  int panencodernewPos = panencoder.getPosition();

  //static int pitchencoderpos=0;
//  pitchencoder.tick();
  //int pitchencodernewPos = pitchencoder.getPosition();
 
 if (lfo1fencoderpos != lfo1fencodernewPos) {
    int dir = int(lfo1fencoder.getDirection());
    endlesspotfct(lfo1f, lfo1fcounter, 0, lfo1fstring,lfofscale,dir);
    lfo1fencoderpos = lfo1fencodernewPos;
  }
  else if (lfo2fencoderpos != lfo2fencodernewPos) {
    int dir = int(lfo2fencoder.getDirection());
    endlesspotfct(lfo2f, lfo2fcounter, 0, lfo2fstring,lfofscale,dir);
    lfo2fencoderpos = lfo2fencodernewPos;
  } 
  else if (lfo3fencoderpos != lfo3fencodernewPos) {
    int dir = int(lfo3fencoder.getDirection());
    endlesspotfct(lfo3f, lfo3fcounter, 0, lfo3fstring,lfofscale,dir);
    lfo3fencoderpos = lfo3fencodernewPos;
  }
 // else if (pitchencoderpos != pitchencodernewPos) {
   //int dir = int(pitchencoder.getDirection());
   //endlesspotfct (pitch, pitchcounter, index, pitchstring, pitchscale, lfostate_pitch, 2,dir);
 // pitchencoderpos = pitchencodernewPos;} 
   else if (lpencoderpos != lpencodernewPos) {
    int dir = int(lpencoder.getDirection());
    endlesspotfct (lp, lpcounter, index, lpstring, lpscale,lfostate_lp,0,dir);
    lpencoderpos = lpencodernewPos;
  } else if (panencoderpos != panencodernewPos) {
    int dir = int(panencoder.getDirection());
    endlesspotfct(pan, pancounter, index, panstring, panscale, lfostate_pan, 1,dir);
    panencoderpos = panencodernewPos;
  } else if (hpencoderpos != hpencodernewPos) {
    int dir = int(hpencoder.getDirection());
    endlesspotfct(hp, hpcounter, index, hpstring,hpscale,lfostate_hp, 0,dir);
    hpencoderpos = hpencodernewPos;
  } else if (lfo1aencoderpos != lfo1aencodernewPos) {
    int dir = int(lfo1aencoder.getDirection());
    endlesspotfct(lfo1a, lfo1acounter, 0, lfo1astring,lfoascale,dir);
    lfo1aencoderpos = lfo1aencodernewPos;
  } else if (gainencoderpos != gainencodernewPos) {
    int dir = int(gainencoder.getDirection());
    endlesspotfct(gain, gaincounter, index, gainstring, gainscale, lfostate_gain, 0, dir);
    gainencoderpos = gainencodernewPos;
  }
  
    static int revdecencoderpos=0;
  revdecencoder.tick();
  int revdecencodernewPos = revdecencoder.getPosition();
  if (revdecencoderpos != revdecencodernewPos) {
    int dir = int(revdecencoder.getDirection());
    endlesspotfct(revdec, revdeccounter, index, revdecstring,revdecscale,lfostate_revdec, 0,dir);
    revdecencoderpos = revdecencodernewPos;
  } 
      static int revdwencoderpos=0;
  revdwencoder.tick();
  int revdwencodernewPos = revdwencoder.getPosition();
  if (revdwencoderpos != revdwencodernewPos) {
    int dir = int(revdwencoder.getDirection());
    endlesspotfct(revdw, revdwcounter, index, revdwstring,revdwscale,lfostate_revdw, 0,dir);
    revdwencoderpos = revdwencodernewPos;
  }
     static int flangdepencoderpos=0;
  flangdepencoder.tick();
  int flangdepencodernewPos = flangdepencoder.getPosition();
  if (flangdepencoderpos != flangdepencodernewPos) {
    int dir = int(flangdepencoder.getDirection());
    endlesspotfct(flangdep, flangdepcounter, index, flangdepstring,flangdepscale,lfostate_flangdep, 0,dir);
    flangdepencoderpos = flangdepencodernewPos;
  }
   static int flangdwencoderpos=0;
  flangdwencoder.tick();
  int flangdwencodernewPos = flangdwencoder.getPosition();
  if (flangdwencoderpos != flangdwencodernewPos) {
    int dir = int(flangdwencoder.getDirection());
    endlesspotfct(flangdw, flangdwcounter, index, flangdwstring,flangdwscale,lfostate_flangdw, 0,dir);
    flangdwencoderpos = flangdwencodernewPos;
  }
      static int del_timeencoderpos=0;
  del_timeencoder.tick();
  int del_timeencodernewPos = del_timeencoder.getPosition();
  if (del_timeencoderpos != del_timeencodernewPos) {
    int dir = int(del_timeencoder.getDirection());
    endlesspotfct(del_time, del_timecounter, index, del_timestring,del_timescale,lfostate_del_time, 0,dir);
    del_timeencoderpos = del_timeencodernewPos;
  }
      static int delfeedencoderpos=0;
  delfeedencoder.tick();
  int delfeedencodernewPos = delfeedencoder.getPosition();
  if (delfeedencoderpos != delfeedencodernewPos) {
    int dir = int(delfeedencoder.getDirection());
    endlesspotfct(delfeed, delfeedcounter, index, delfeedstring,delfeedscale,lfostate_delfeed, 0,dir);
    delfeedencoderpos = delfeedencodernewPos;
  }
        static int deldwencoderpos=0;
  deldwencoder.tick();
  int deldwencodernewPos = deldwencoder.getPosition();
  if (deldwencoderpos != deldwencodernewPos) {
    int dir = int(deldwencoder.getDirection());
    endlesspotfct(deldw, deldwcounter, index, deldwstring,deldwscale,lfostate_deldw, 0,dir);
    deldwencoderpos = deldwencodernewPos;
  }
    static int lfo2aencoderpos=0;
  lfo2aencoder.tick();
  int lfo2aencodernewPos = lfo2aencoder.getPosition();
  if (lfo2aencoderpos != lfo2aencodernewPos) {
    int dir = int(lfo2aencoder.getDirection());
    endlesspotfct(lfo2a, lfo2acounter, 0, lfo2astring,lfofscale,dir);
    lfo2aencoderpos = lfo2aencodernewPos;
  }
  static int lfo3aencoderpos=0;
  lfo3aencoder.tick();
  int lfo3aencodernewPos = lfo3aencoder.getPosition();
  if (lfo3aencoderpos != lfo3aencodernewPos) {
    int dir = int(lfo3aencoder.getDirection());
    endlesspotfct(lfo3a, lfo3acounter, 0, lfo3astring,lfofscale,dir);
    lfo3aencoderpos = lfo3aencodernewPos;
  }
  
  
  
  
return;
}

void doEncoderB(const char* message)
{
  
  if (message == "1")
    {
      if( digitalRead(pitch[2]) != B_setpitch ) 
        {
        B_setpitch = !B_setpitch;
        }
      if ( B_setpitch && !A_setpitch )
        {
          pitchcounter[index]-= pitch[5];
        }
      pitchrotating = false;
       
    } 
  return;
}


void doEncoderA(const char* message)
{ 
  
  if (message == "1")
    {
      if( digitalRead(pitch[1]) != A_setpitch ) 
        {
        A_setpitch = !A_setpitch;
        }
      if ( A_setpitch && !B_setpitch )
        {
          pitchcounter[index]+= pitch[5];
        }
        pitchrotating = false; 
        
    } 
  return;
}

void endlesspotfct2(int arr[], int arr2[], int z, String arr3[],float scale, int arr4[], int y) 
{ 
       if (arr2[0]<arr[6])
        {
          arr2[0]=arr[6];    
        }
        //checks if counter is below lower limit
      else if ( arr2[0] >arr[7])
        {
          arr2[0]=arr[7];     
        }
        //checks if counter is above upper limit

      potsend1=arr[0];
      potsend2=arr2[z];
      potfct(potsend1,potsend2);
      if (y==2)
        {
          screen1val2[z]=static_cast<float>(arr2[z])*scale;
        }
        //scales screen value (float)
      else 
        {          
          screen1val[z]=arr2[z]*scale;
        }
        //scales screen value (int)
      screen1string1[z]=arr3[0];
      if (y==0 or y==2)
        {
          screen1string2[z]=arr3[1];
        } 
        //sets unit for screen
      else
        {
          if (screen1val[index]<5)
            {
              screen1string2[z]=" L";
            }
          else if (screen1val[index]<30)
            {
              screen1string2[z]=" L-Mitte";
            }
          else if (screen1val[index]<60)
            {
              screen1string2[z]=" Mitte-L";
            }
          else if (screen1val[index]<69)
            {
              screen1string2[z]=" Mitte";
            }
          else if (screen1val[index]<94)
            {
              screen1string2[z]=" Mitte-R";
            }
          else if (screen1val[index]<123)
            {
              screen1string2[z]=" R-Mitte";
            }
          else if (screen1val[index]<129)
            {
              screen1string2[z]=" R";
            }
          panstring2[index]= screen1string2[z];
        }
        //sets Pan unit (left/middle/right) for screen
      myString2 = "";
      if (arr4[index] == 4)
        {
          myString2 = "keiner";
        }
        //sets "No LFO" for screen
      else
        {
          myString2 = String(arr4[index]);
        }
        //sets current LFO for screen
      screen1string3[z]= myString2;
      if (y!=2)
        {
          bools2=false;
        }
       
      else
        {
         bools3=false;
        }
        starttime3=millis();
  panpitch[index]=y;
return;
}
