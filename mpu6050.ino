#include<Wire.h>

#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY 65.536
 
#define M_PI 3.14159265359	    
 
#define dt 0.01	
 const int MPU=0x68;   //MPU I2C ADDRESS
 int16_t AcX,AcY,Temp,AcZ,GyX,GyY,GyZ;

 int minVal=265;
 int maxVal=402;

 double angle;
 
 int16_t accData[3];
 int16_t gyrData[3];
 double *pitch;
 double *roll;
 
 
 void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);    //PWR-MGMT REG
  Wire.write(0);    //WAKE UP
  Wire.endTransmission(true);
  Serial.begin(115200);
  } 
 
 void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);    //REG 03BH FOR ACXH
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);    //RE FOR 14 REG
  
  AcX = ( Wire.read() << 8 ) | Wire.read();
  AcY = ( Wire.read() << 8 ) | Wire.read();
  AcZ = ( Wire.read() << 8 ) | Wire.read();
  Temp = ( Wire.read() << 8 ) | Wire.read();
  GyX = ( Wire.read() << 8 ) | Wire.read();
  GyY = ( Wire.read() << 8 ) | Wire.read();
  GyZ = ( Wire.read() << 8 ) | Wire.read();
  
  accData[0] = short(AcX);
  accData[1] = short(AcY);
  accData[2] = short(AcZ);
  
  gyrData[0] = short(GyX);
  gyrData[1] = short(GyY);
  gyrData[2] = short(GyZ);
  
  float pitchAcc, rollAcc;               
 
    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *pitch += ((float)gyrData[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
    *roll -= ((float)gyrData[1] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis
 
    // Compensate for drift with accelerometer data if !bullshit
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
    {
	// Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2((float)accData[1], (float)accData[2]) * 180 / M_PI;
        *pitch = *pitch * 0.98 + pitchAcc * 0.02;
 
	// Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2((float)accData[0], (float)accData[2]) * 180 / M_PI;
        *roll = *roll * 0.98 + rollAcc * 0.02;
    }
  
     Serial.print("AngleY= ");
     Serial.println(*pitch);
     
     Serial.print("AngleY= ");
     Serial.println(*roll);

     Serial.println("-----------------------------------------");
     delay(10);
  }
