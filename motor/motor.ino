#include <AFMotor.h>

AF_DCMotor motor1(1);       // Wheel 1
AF_DCMotor motor2(2);      // Wheel 2

void setup() 
{
  motor1.setSpeed(200);  // Wheel 1 Top Speed
  motor1.run(RELEASE);
  motor2.setSpeed(200);  // Wheel 2 Top Speed  
  motor2.run(RELEASE); 
}


void loop() 
{
   if( output > 0 )          //Forward
   {
     motor1.run(FORWARD);
     motor1.setSpeed(200);
     motor2.run(FORWARD);
     motor2.setSpeed(200);
   }
   else                      // Backward.
    {
     motor1.run(BACKWARD);
     motor1.setSpeed(200);
     motor2.run(BACKWARD);
     motor2.setSpeed(200);
   }
   
   
}
