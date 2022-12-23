# Tamagotchi Simulator
This code simulates a Tamagotchi virtual pet on an OLED display. The Tamagotchi can be in one of several states: SLEEPING, HUNGRY, HAPPY, or UNHAPPY. The state is determined by the hunger and happiness levels of the Tamagotchi, which can be affected by pressing buttons connected to the Arduino. The Tamagotchi can also grow and hatch from an egg.

## Hardware
- Arduino
- OLED display
- Buttons
- Speaker (optional)

## Dependencies
- Wire library
- SPI library
- Adafruit GFX library
- Adafruit SSD1306 library
- **bitmaps.h** header file (included in the code)

## Functions
- **setup()**: Initializes serial communication, OLED display, and buttons. Also plays a start game sound on the speaker.
- **loop()**: Calls the updateState() and drawTamagotchi() functions, and checks the buttons.
- **updateState()**: Updates the state of the Tamagotchi based on the hunger, happiness, and sleep levels. If the Tamagotchi's hunger or happiness levels are too low, it will die. If the Tamagotchi's growth level is high enough, it will hatch from an egg.
- **drawTamagotchi()**: Draws the Tamagotchi on the OLED display based on its current state.
- **checkButtons()**: Checks the state of the buttons and performs actions based on which button is pressed. The buttons can be used to feed the Tamagotchi, put it to sleep, or play with it.
- **stateDebug()** (optional): Prints the current state and levels of the Tamagotchi to the serial monitor for debugging purposes.
- **growDebug()** (optional): Prints the growth levels of the Tamagotchi to the serial monitor for debugging purposes.

## Notes
- Make sure to include the necessary libraries and header files at the top of the code.
- The OLED display and buttons should be connected to the Arduino according to the pin definitions in the code.
- The **bitmaps.h** header file contains the bitmap images for the Tamagotchi's different states.
- The **note** array and the **speakerpin** variable are used for the optional start game sound. You can remove or modify these if you don't want to use the speaker.
- The **stateDebug()** and **growDebug()** functions are optional and can be removed if not needed.