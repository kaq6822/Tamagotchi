#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bitmaps.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// define the states for the Tamagotchi
enum TamagotchiState {
  DEAD,
  SLEEPING,
  HUNGRY,
  HAPPY,
  UNHAPPY
};

int speakerpin = 11; // Speaker pin
int note[] = {2093,2349,2637,2793,3136,3520,3951,4186}; // do re mi fa so la si do
bool stat_sound = true;

// define the pins for the buttons
const uint8_t buttonPins[3] = { 10, 9, 8 };

// define the current state and frame
TamagotchiState state = SLEEPING;
const unsigned long frameDelay = 1000;

// define the hunger and happiness variables
int16_t hunger = 0;
int16_t happiness = 500; // max is 1000
int16_t sleep = 0;

// define the character growth
uint16_t growth = 0;
uint16_t growth_happy = 0;
uint16_t growth_sleep = 0;
uint16_t growth_food = 0;
uint16_t growth_hate = 0;

void setup() {
  // initialize the serial communication
  Serial.begin(9600);

  // initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // initialize the buttons
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Game start sound play
  for (int i=0; i < 8; i++) {
    tone(speakerpin, note[i], 150);
    delay(200);
  }
}

void loop() {
  // update the state
  updateState();

  // draw the Tamagotchi on the display
  drawTamagotchi();

  // check the buttons
  checkButtons();
}

void stateDebug() {
  Serial.print("state: ");
  if (state == 1) Serial.print("SLEEPING");
  if (state == 2) Serial.print("HUNGRY");
  if (state == 3) Serial.print("HAPPY");
  if (state == 4) Serial.print("UNHAPPY");
  Serial.print(", hunger: ");
  Serial.print(hunger);
  Serial.print(", happiness: ");
  Serial.print(happiness);
  Serial.print(", sleep: ");
  Serial.print(sleep);  
  Serial.println();
}

void growDebug() {
  Serial.print("growth: ");
  Serial.print(growth);
  Serial.print(", growth_happy: ");
  Serial.print(growth_happy);
  Serial.print(", growth_sleep: ");
  Serial.print(growth_sleep);
  Serial.print(", growth_food: ");
  Serial.print(growth_food);
  Serial.print(", growth_hate: ");
  Serial.print(growth_hate);
  Serial.println();
}

void updateState() {
  // decrease the hunger and happiness, sleep levels
  hunger++;
  happiness--;
  if (sleep < 500) {
    sleep++;
  }

  // grow for hatch form the egg
  growth++;
  if (growth >= 2000) {
    hatchFromEgg();
    delay(0xFFFFFFFF);
  }

  // Debug Tamagotchi status in serial moniter
  // stateDebug();
  growDebug();

  if (hunger >= 1000 || happiness <= 0) { // if the hunger or happiness levels are too low, set the state to DEAD
      state = DEAD;

      display.clearDisplay();
      display.drawBitmap((128 - SKULL_WIDTH)/2, 0, skull, SKULL_WIDTH, SKULL_HEIGHT, 1);
      display.display();

      for (int i=7; i >= 0; i--) {
        tone(speakerpin, note[i], 150);
        delay(200);
      }

      tone(speakerpin, note[0], 150);

      // you have to reset arduino board.
      delay(0xFFFFFFFF);
  }
  
  if (sleep < 500) {
    if (hunger >= 500) { // if the happiness level is low and the hunger level is high, set the state to HUNGRY
      growth_food+=3;
      state = HUNGRY;
    } else if (happiness < 500) { // if the happiness level is high and the hunger level is low, set the state to HAPPY
      growth_hate+=3;
      state = UNHAPPY;
    } else {
      growth_happy++;
      state = HAPPY;
    }
  }// otherwise, set the state to SLEEPING
  else {
    growth_sleep+=3;
    state = SLEEPING;
  }
}

void hatchFromEgg() {
  display.clearDisplay();

  // fireworks display
  display.drawBitmap(0, 0, fire, FIRE_WIDTH, FIRE_HEIGHT, 1);
  display.drawBitmap(127 - FIRE_WIDTH, 0, fire, FIRE_WIDTH, FIRE_HEIGHT, 1);

  // Tamagotchi display
  if (growth_food >= growth_hate && growth_food >= growth_happy && growth_food >= growth_sleep) {
    display.drawBitmap((128 - KUKURI_WIDTH)/2, 0, kukuri, KUKURI_WIDTH, KUKURI_HEIGHT, 1);
  } else if (growth_hate >= growth_food && growth_hate >= growth_happy && growth_hate >= growth_sleep) {
    display.drawBitmap((128 - GOOS_WIDTH)/2, 0, goos, GOOS_WIDTH, GOOS_HEIGHT, 1);
  } else if (growth_sleep >= growth_food && growth_sleep >= growth_happy && growth_sleep >= growth_hate) {
    display.drawBitmap((128 - MUKGOJA_WIDTH)/2, 0, mukgoja, MUKGOJA_WIDTH, MUKGOJA_HEIGHT, 1);
  } else {
    display.drawBitmap((128 - PICHU_WIDTH)/2, 0, pichu, PICHU_WIDTH, PICHU_HEIGHT, 1);
  }

  for (int i=3; i < 8; i++) {
    tone(speakerpin, note[i], 250);
    delay(300);
  }
  
  display.display();
}

void statChangeSound() {
  if (stat_sound) {
    tone(speakerpin, note[6], 150);
    stat_sound = false;
  }
}

void actionSound() {
  tone(speakerpin, note[7], 150);  
}

void drawTamagotchi() {
  display.clearDisplay();

  // sleep 'zzz' emotion display
  if (sleep >= 500) {
    display.drawBitmap(127-ZZZ_WIDTH, 0, zzz, ZZZ_WIDTH, ZZZ_HEIGHT, 1);
  }

  // Tamagotchi display
  display.drawBitmap((128 - EGG_WIDTH)/2, 0, egg, EGG_WIDTH, EGG_HEIGHT, 1);

  // Need food, happness emotion display
  if (hunger > 500) {
    display.drawBitmap(0, 0, food, FOOD_WIDTH, FOOD_HEIGHT, 1);
    statChangeSound();
  } else if (happiness < 500) {
    display.drawBitmap(0, 0, heart, HEART_WIDTH, HEART_HEIGHT, 1);
    statChangeSound();
  }

  display.display();
}

void checkButtons() {
  if (state != SLEEPING) {
    // check the first button
    if (digitalRead(buttonPins[0]) == HIGH) {
      actionSound();
      stat_sound = true;
      sleep = 0;
      // if the Tamagotchi is hungry, decrease the hunger level
      if (state == HUNGRY) {
        Serial.println("Button 1 pressed!: HUNGRY");
        hunger -= 250;
        if (hunger < 0) {
          hunger = 0;
        }
        display.clearDisplay();
        display.drawBitmap((128 - FOOD_WIDTH)/2, 0, food, FOOD_WIDTH, FOOD_HEIGHT, 1);
        display.display();
        delay(frameDelay);
      }
      // otherwise, increase the happiness level
      else {
        Serial.println("Button 1 pressed!: HAPPINESS");
        happiness += 250;
        if (happiness > 1000) {
          happiness = 1000;
        }
        display.clearDisplay();
        display.drawBitmap((128 - HEART_WIDTH)/2, 0, heart, HEART_WIDTH, HEART_HEIGHT, 1);
        display.display();
        delay(frameDelay);
      }
    }
  }

  // check the second button
  if (digitalRead(buttonPins[1]) == HIGH) {
    Serial.println("Button 2 pressed!");
    actionSound();
    sleep = 0;
    // if the Tamagotchi is sleeping, wake it up
    if (state == SLEEPING) {
      happiness -= 100;
      if (happiness < 0) {
        happiness = 0;
      }
    }
  }

  // check the third button
  if (digitalRead(buttonPins[2]) == HIGH) {
    Serial.println("Button 3 pressed!");
    actionSound();
    sleep = 0;
    display.clearDisplay();
    display.drawBitmap((128 - PUNCH_WIDTH)/2, 0, punch, PUNCH_WIDTH, PUNCH_HEIGHT, 1);
    display.display();
    delay(frameDelay);

    // hate increse
    growth_hate += 100;
    
    // if the Tamagotchi is happy, decrease the happiness level
    if (state == HAPPY) {
      happiness -= 250;
      if (happiness < 0) {
        happiness = 0;
      }
    }
    // otherwise, increase the hunger level
    else {
      hunger += 250;
      if (hunger > 1000) {
        hunger = 1000;
      }
    }
  }
}