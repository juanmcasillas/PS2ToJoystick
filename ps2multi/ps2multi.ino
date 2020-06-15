// //////////////////////////////////////////////////////////////////////////
//
// PS2Multi
// (C) 2020 Juan M. Casillas <juanm.casillas@gmail.com>
//
// Creates a simple Arduino Micro (Leonardo) HID Joystick to manage the
// PS2 Dualshock 2 controller pad, because the current Win 10 Driver 
// doesn't support the old XP EMS Cheap PS2->PC Controllers
//
// 06-15-2020 Improved the code, so we can manage the R6Pedals (Keyboard HID)
// and PS2ToJoystick (Joystick HID) in the same arduino.
//
// //////////////////////////////////////////////////////////////////////////

#include <PS2X_lib.h>  //for v1.6
#include <Joystick.h>


#include <Keyboard.h>
#include <Bounce2.h>

#define BOUNCE_LOCK_OUT 1
#define BOUNCE_WAIT 1
#define BOUNCE_COUNT 1
#define NBUTTONS 2

int buttonPins[] = {
  5,//9, //Q button 1 (left)
  4,//8  //E button 2 (right)
};

char buttonPresets[] = { 
  'q', //button 1
  'e', //button 2
  };

boolean buttonPressed[NBUTTONS]; // Instantiate a Bounce object array to store each debouncer in
Bounce debouncers[NBUTTONS];     //Instantiate a counter array for each button to debounce count timer
int debounceCount[NBUTTONS];


// comment this to allow debug console
#define RELEASE
#define DELAY_TIME 20

#ifndef DBG_OUTPUT_PORT
#define DBG_OUTPUT_PORT Serial
#endif 

#ifndef RELEASE
#define DEBUGLOG(...) DBG_OUTPUT_PORT.printf(__VA_ARGS__)
#define SERIAL_SPEED 115200
#else
#define DEBUGLOG(...)
#endif



//
// Set the Arduino PINS. I use an Arduino Pro Micro Clone (Leonardo)
// All the inputs are Digital Inputs. Remember that PS2_DAT requires
// a pull up resistor (4.7 KOhm)
//

#define PS2_DAT        6  //14    
#define PS2_CMD        7  //15
#define PS2_SEL        8  //16
#define PS2_CLK        9  //17

// Axis Limits
#define MIN_X           0
#define MAX_X           255
#define MIN_Y           0
#define MAX_Y           255

// Button Mapping
#define JOY_CROSS       0
#define JOY_CIRCLE      1
#define JOY_SQUARE      2
#define JOY_TRIANGLE    3

#define JOY_L1          4
#define JOY_L2          5
#define JOY_L3          6

#define JOY_R1          7
#define JOY_R2          8
#define JOY_R3          9

#define JOY_SELECT      10
#define JOY_START       11

#define JOY_PAD_UP      12
#define JOY_PAD_DOWN    13
#define JOY_PAD_LEFT    14
#define JOY_PAD_RIGHT   15

#define pressures   false
#define rumble      false

Joystick_ Joystick;
PS2X ps2x; 

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup(){
 
#ifndef RELEASE 
  DBG_OUTPUT_PORT.begin(SERIAL_SPEED);
#endif  

    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

    switch (error) {
        case 0:
            DEBUGLOG("Found Controller, configured successful ");
            break;
        case 1:
            DEBUGLOG("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
            break;
        case 2:
            DEBUGLOG("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
            break;
        case 3:
            DEBUGLOG("Controller refusing to enter Pressures mode, may not support it. ");
            break;
    }

    type = ps2x.readType(); 
    switch(type) {
        case 0:
            DEBUGLOG("Unknown Controller type found ");
            break;
        case 1:
            DEBUGLOG("DualShock Controller found ");
            break;
        case 2:
            DEBUGLOG("GuitarHero Controller found ");
            break;
        case 3:
            DEBUGLOG("Wireless Sony DualShock Controller found ");
            break;
    }

    // left
    Joystick.setXAxisRange(MIN_X,MAX_X);
    Joystick.setYAxisRange(MIN_Y,MAX_Y);

    // right
    Joystick.setRxAxisRange(MIN_X,MAX_X);
    Joystick.setRyAxisRange(MIN_Y,MAX_Y);
    
    Joystick.begin();
    
    Joystick.setXAxis(int(MAX_X/2));
    Joystick.setYAxis(int(MAX_Y/2));

    Keyboard.begin();
    for (int i = 0; i < NBUTTONS; i++) {
        debouncers[i] = Bounce();
        debounceCount[i] = 0;
        pinMode(buttonPins[i],INPUT_PULLUP);
        (debouncers[i]).attach(buttonPins[i]);
        (debouncers[i]).interval(BOUNCE_WAIT);
        delay(100);
        buttonPressed[i] = false;
    }
}


void loop() {
    if(error == 1) //skip loop if no controller found
    return; 
  
    ps2x.read_gamepad(); 
    
    if(ps2x.ButtonPressed(PSB_CIRCLE)) Joystick.pressButton(JOY_CIRCLE);
    if(ps2x.ButtonReleased(PSB_CIRCLE)) Joystick.releaseButton(JOY_CIRCLE);

    if(ps2x.ButtonPressed(PSB_CROSS)) Joystick.pressButton(JOY_CROSS);
    if(ps2x.ButtonReleased(PSB_CROSS)) Joystick.releaseButton(JOY_CROSS);

    if(ps2x.ButtonPressed(PSB_SQUARE)) Joystick.pressButton(JOY_SQUARE);
    if(ps2x.ButtonReleased(PSB_SQUARE)) Joystick.releaseButton(JOY_SQUARE);

    if(ps2x.ButtonPressed(PSB_TRIANGLE)) Joystick.pressButton(JOY_TRIANGLE);
    if(ps2x.ButtonReleased(PSB_TRIANGLE)) Joystick.releaseButton(JOY_TRIANGLE);

    if(ps2x.ButtonPressed(PSB_L1)) Joystick.pressButton(JOY_L1);
    if(ps2x.ButtonReleased(PSB_L1)) Joystick.releaseButton(JOY_L1);

    if(ps2x.ButtonPressed(PSB_L2)) Joystick.pressButton(JOY_L2);
    if(ps2x.ButtonReleased(PSB_L2)) Joystick.releaseButton(JOY_L2);
    
    if(ps2x.ButtonPressed(PSB_L3)) Joystick.pressButton(JOY_L3);
    if(ps2x.ButtonReleased(PSB_L3)) Joystick.releaseButton(JOY_L3);


    if(ps2x.ButtonPressed(PSB_R1)) Joystick.pressButton(JOY_R1);
    if(ps2x.ButtonReleased(PSB_R1)) Joystick.releaseButton(JOY_R1);

    if(ps2x.ButtonPressed(PSB_R2)) Joystick.pressButton(JOY_R2);
    if(ps2x.ButtonReleased(PSB_R2)) Joystick.releaseButton(JOY_R2);
    
    if(ps2x.ButtonPressed(PSB_R3)) Joystick.pressButton(JOY_R3);
    if(ps2x.ButtonReleased(PSB_R3)) Joystick.releaseButton(JOY_R3);

    if(ps2x.ButtonPressed(PSB_SELECT)) Joystick.pressButton(JOY_SELECT);
    if(ps2x.ButtonReleased(PSB_SELECT)) Joystick.releaseButton(JOY_SELECT);

    if(ps2x.ButtonPressed(PSB_START)) Joystick.pressButton(JOY_START);
    if(ps2x.ButtonReleased(PSB_START)) Joystick.releaseButton(JOY_START);


    Joystick.setXAxis(ps2x.Analog(PSS_LX));
    Joystick.setYAxis(ps2x.Analog(PSS_LY));
    Joystick.setRxAxis(ps2x.Analog(PSS_RX));
    Joystick.setRyAxis(ps2x.Analog(PSS_RY));


    // Direction Pad
    if(ps2x.ButtonPressed(PSB_PAD_UP)) Joystick.pressButton(JOY_PAD_UP);
    if(ps2x.ButtonReleased(PSB_PAD_UP)) Joystick.releaseButton(JOY_PAD_UP);

    if(ps2x.ButtonPressed(PSB_PAD_DOWN)) Joystick.pressButton(JOY_PAD_DOWN);
    if(ps2x.ButtonReleased(PSB_PAD_DOWN)) Joystick.releaseButton(JOY_PAD_DOWN);

    if(ps2x.ButtonPressed(PSB_PAD_RIGHT)) Joystick.pressButton(JOY_PAD_RIGHT);
    if(ps2x.ButtonReleased(PSB_PAD_RIGHT)) Joystick.releaseButton(JOY_PAD_RIGHT);

    if(ps2x.ButtonPressed(PSB_PAD_LEFT)) Joystick.pressButton(JOY_PAD_LEFT);
    if(ps2x.ButtonReleased(PSB_PAD_LEFT)) Joystick.releaseButton(JOY_PAD_LEFT);


    // read and debounce buttons (for keyboard mapping)

    for (int j = 0; j < NBUTTONS; j++) { // iterate over each button (pin)
        
        (debouncers[j]).update();         //check current value
        int value = (debouncers[j]).read();
        
        if ( value == LOW ) { // if button pressed
        
        // The button has been held down long enough and it hasn't been previously registered as pressed
        if(debounceCount[j] == BOUNCE_COUNT && buttonPressed[j] == false) { 
            Keyboard.press(char(buttonPresets[j])); //Keyboard.write('1');
            buttonPressed[j] = true;
            } else {
                if(debounceCount[j] < BOUNCE_COUNT) { 
                debounceCount[j] = debounceCount[j] + 1; 
                }
            }
            
        } else { //button is not pressed
            
            if(debounceCount[j] > 0) {
            debounceCount[j] = debounceCount[j] - 1; // keep decreasing count unless 0
            } else {
            Keyboard.release(char(buttonPresets[j])); // if 0 then release button
            buttonPressed[j] = false;
            }
        }
    }

    delay(DELAY_TIME);

}
