**About**

This project consists of an arduino-based hardware controller which controls a granular synthesizer. ***You may choose to use the software without the hardware controller (default on startup).***

**How to use**

- ***color scheme:*** turquoise = not selected/off, light purple = selected/on
- these numbers are for mapping the LFO onto something (in this case the gain) ![image](https://user-images.githubusercontent.com/93798450/154538849-fdbc00ac-137c-4085-b19d-3cd232ed3719.png)
- select the version you want to use from the "Version" menu bar
- load sample into your project from the "Samples" menu bar
- select your grain
- the ***sequencer*** is 2 bars in 1/8 notes
- to ***toggle the sequencer*** use this button ![image](https://user-images.githubusercontent.com/93798450/154749271-eeb6afb4-bf1b-4cf9-a7e9-ac53ddcfb1bb.png)
- the small button (next to "Sequencer" text in L-Grainer) is only to make the current beat visible, the sequencer will still work if it's not pressed
- if you zoom in far enough on a waveform, it shows you the length of a 1/4 note in your currently selected bpm
- if you select "change LFO scales" in the "Version" menu bar, you can change how much the LFOs should affect each effect (values between 0.1 and 1.5 are advised)
to permanently save your changes edit the value in one of these objects and save ![image](https://user-images.githubusercontent.com/93798450/154543253-67be026f-e74d-4336-a913-e513f01213c0.png)

- you can record by chosing a file name after clicking on the folder icon and then clicking on the record button
- the ***RMS levelmeter*** (top right) is measuring ***before limiter*** (-0.5dB threshold). 

**Presets**
- save presets as ***.json files***
- if you copy the samples used in a preset to your patcher/application folder, loading that preset will also load the samples

**On Windows**
- if you hold down CTRL and click on a waveform, you can can zoom with vertical mouse movement or scroll left or right with horizontal mouse movement
- if you hold down ALT, you can grow or shrink your selection with vertical mouse movement or reposition it with horizontal mouse movement
- if you hold down SHIFT, you can affect just the start or end time of your selection


**License & Credits**

All the code in this project is released under ***GPL v3***. 

The software was developed by Lucas Tippelt.

The hardware was designed and build by Valentin Vankann, Kristian Hammer and Pierre Kirchhoff.

The UI was designed by Valentin Vankann.

The product video was made by Kristian Hammer, Pierre Kirchhoff, Luca Meyer, Li-Yue Kwok and Sinan Kleb.

**Workaround for Linux**

There is no native Version for Linux now, however its possible to run the TGrainer in Wine, this is my working configuration in Lutris. Obviously you'll have to move the files into the wine directory. ![image](https://user-images.githubusercontent.com/99901362/156372893-39c5e768-94a2-4293-8d77-80e3e0fc6f60.png)

known Issues: 
the max runtime enviroment running in Wine can't parse .mp3 files (at least for me), however most other file types like .wav work fine 
you might have to fiddle with the audio driver in the options menu to get an high quality experience (directsound seemed to work best with my portaudio linux driver)


