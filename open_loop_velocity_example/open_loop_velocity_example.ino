#include <Arduino.h>
#include <SimpleFOC.h>

//#define DebugSerial Serial
HardwareSerial Serial1(PB7, PB6);  // uart1
#define DebugSerial Serial1
// HardwareSerial Serial3(PB11, PB10);   // uart3
// #define DebugSerial Serial3

BLDCMotor motor = BLDCMotor(7);
BLDCDriver6PWM driver = BLDCDriver6PWM(PA8, PB13, PA9, PB14, PA10, PB15);

MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

// LED Blinking
bool blinkstate;
u_int32_t last_blink;
u_int16_t blinktime = 300;

Commander command = Commander(DebugSerial);
bool monitoring_var = false; // bool to toggle debug printing
bool* var_ref = &monitoring_var;
void toggle_monitoring(char* str){
  *var_ref = !*var_ref;
  if (*var_ref) { DebugSerial.println("monitoring enabled"); } else { DebugSerial.println("monitoring disabled"); }
}
void doMotor(char* cmd) { command.motor(&motor, cmd); }
void mdisable(char* cmd) { motor.disable(); DebugSerial.println("motor disabled"); blinktime = 600;}
void menable(char* cmd) { motor.enable(); DebugSerial.println("motor enabled"); blinktime = 300;}

void setup() {
  pinMode(PC11, OUTPUT);  // LED_BUILTIN

  DebugSerial.begin(115200);
  SimpleFOCDebug::enable(&DebugSerial);
  delay(500);
  SIMPLEFOC_DEBUG("Startup");
  
  SIMPLEFOC_DEBUG("Setup Complete");
  _delay(1000);

}


void loop() {
  motor.loopFOC();    // main FOC algorithm function
		
  motor.move();   // Motion control function
  

  if (monitoring_var){
    motor.monitor();
  }

  command.run();  // commander interface

  if ((millis() - last_blink) > blinktime){
    digitalWrite(PC11, blinkstate);
    blinkstate = !blinkstate;
    last_blink = millis();
  }

  // Blink to visibly show hangs
  // digitalWrite(PC11, HIGH);  // PC11
  // SIMPLEFOC_DEBUG("High");
  
  // // i2C test - monitor sensor value
  // sensor.update();
  // SIMPLEFOC_DEBUG("Angle: ",sensor.getAngle());
  // delay(1000);
  
  // digitalWrite(PC11, LOW);
  // SIMPLEFOC_DEBUG("Low");
  // delay(1000);

  // // RX test - echo
  // if (DebugSerial.available()){
  //   DebugSerial.println(DebugSerial.readString()); 
  // }
}