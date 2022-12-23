# Tamagotchi Project
------------------------------
 사용자와 상호작용하는 다마고치 Source Code for Arduino UNO Device.
 
## Equipment Components
> Arduino UNO

> 0.96 inch OLED Display (128*32)

> Speaker or Buzzer

> Button * 3

> 1K Resistor * 3

------------------------------
## Source Code
------------------------------
### Arduino Main Source Code File
> tamagotchi.ino

**updateState method do**

Update charactor state:
1. Increase hunger
2. Decrease happiness
3. Increase sleep
4. Increase growth

And status change action:
1. growth >= 2000
    - Poketmon hatch from egg
2. hunger >= 1000 || happiness <= 0
    - Charactor status change to DEAD
3. sleep >= 500
    - Charactor status change to SLEEP
4. hunger >= 500
    - Charactor status change to HUNGRY
5. happiness < 500
    - Charactor status change to UNHAPPY
6. hunger < 500 && happiness > 500
    - Charactor status change to HAPPY
```
    updateState();
```

**drawTamagotchi method do**

Display Tamagotchi egg and status. Also play sound when status change:
1. sleep >= 500
    - Display sleep status on the right side (zzZ)
2. Display egg in the center
3. hunger > 500
    - Display hunger status on the left side (food)
4. happiness < 500
    - Display unhappy status on the left side (love)
```
    drawTamagotchi();
```

**checkButtons method do**

Button action

Button 1:
 if status is sleep button 1 is not working.
 playing Button action sound.
 if the Tamagotchi is hungry, decrease the hunger level and display food in middel of screen.
 otherwise, increase the happiness level and display love in middle of screen.

Button 2:
 playing Button action sound.
 if the Tamagotchi is sleeping, wake it up And decrease happiness.

Button 3:
 playing Button action sound.
 Make Tamagotchi to not sleepy. And increase growth_hate.
 If Tamagotchi status is happy decrease happiness. Otherwise increase hunger level.


### Display bitmap data file
> bitmaps.h

Bitmap display images file width, height
```
    #define *_WIDTH
    #define *_HEIGHT
```

Bitmap images 
```
    static const unsigned char PROGMEM *[]
```

------------------------------
## Creator & Contact
------------------------------
> SSU Department of Media Management 20 bjkang
> kaq6822@naver.com