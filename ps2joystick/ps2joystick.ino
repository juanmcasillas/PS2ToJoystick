// //////////////////////////////////////////////////////////////////////////
//
// PS2ToJoystick
// (C) 2020 Juan M. Casillas <juanm.casillas@gmail.com>
//
// Creates a simple Arduino Micro (Leonardo) HID Joystick to manage the
// PS2 Dualshock 2 controller pad, because the current Win 10 Driver 
// doesn't support the old XP EMS Cheap PS2->PC Controllers
//
// //////////////////////////////////////////////////////////////////////////

#include <PS2X_lib.h>  //for v1.6
#include <Joystick.h>

// comment this to allow debug console
#define RELEASE

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

    delay(5);

}
