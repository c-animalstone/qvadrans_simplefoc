/**
 *  Encoder example code 
 * 
 * This is a code intended to test the encoder connections and to demonstrate the encoder setup.
 * 
 */

#include <SimpleFOC.h>

//#define DebugSerial Serial
HardwareSerial Serial1(PB7, PB6);  // uart1
#define DebugSerial Serial1


Encoder encoder = Encoder(PA5, PB3, 1024);
// interrupt routine intialisation
void doA(){encoder.handleA();}
void doB(){encoder.handleB();}

void setup() {
  // monitoring port
  Serial1.begin(115200);

  // enable/disable quadrature mode
  encoder.quadrature = Quadrature::ON;

  // check if you need internal pullups
  encoder.pullup = Pullup::USE_EXTERN;
  
  // initialise encoder hardware
  encoder.init();
  // hardware interrupt enable
  encoder.enableInterrupts(doA, doB);

  Serial1.println("Encoder ready");
  _delay(1000);
}

void loop() {
  // iterative function updating the sensor internal variables
  // it is usually called in motor.loopFOC()
  // not doing much for the encoder though
  encoder.update();
  // display the angle and the angular velocity to the terminal
  Serial1.print(encoder.getAngle());
  Serial1.print("\t");
  Serial1.println(encoder.getVelocity());
}
