#include <Wire.h>
//Declaring some global variables

//for MPU6050
int gyro_x, gyro_y, gyro_z;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
boolean set_gyro_angles;

long acc_x, acc_y, acc_z, acc_total_vector;
float angle_roll_acc, angle_pitch_acc;

float angle_pitch, angle_roll;
int angle_pitch_buffer, angle_roll_buffer;
float angle_pitch_output, angle_roll_output;
float initial_angle_pitch_output, initial_angle_roll_output;

long loop_timer;
int temp;
void setup() {
//MPU6050 setup
  Wire.begin();                                                  //Start I2C as master
  setup_mpu_6050_registers();                         //Setup the registers of the MPU6050 
  for (int cal_int = 0; cal_int < 1000 ; cal_int ++){ //Read the raw data for 1000 times
    read_mpu_6050_data();                                             
    gyro_x_cal += gyro_x;                //Add the offset
    gyro_y_cal += gyro_y;           
    gyro_z_cal += gyro_z;           
    delay(3);                                         //Delay 3us
  }

  //get avarage offset
  gyro_x_cal /= 1000;                                                 
  gyro_y_cal /= 1000;                                                 
  gyro_z_cal /= 1000;                                                 
  Serial.begin(115200); 

  loop_timer = micros(); 
}


// for IMU 

void setup_mpu_6050_registers(){
  //Activate MPU6050
  Wire.beginTransmission(0x68);                                        //Start communicating
  Wire.write(0x6B);                                                    
  Wire.write(0x00);                                                    
  Wire.endTransmission();                                             
  //Configure acc
  Wire.beginTransmission(0x68);                                        
  Wire.write(0x1C);                                                    
  Wire.write(0x10);                                                    
  Wire.endTransmission();                                             
  //Configure gyro
  Wire.beginTransmission(0x68);                                        
  Wire.write(0x1B);                                                    
  Wire.write(0x08);                                                    
  Wire.endTransmission();                                             
}


void read_mpu_6050_data(){                                             //Reading the raw data
  Wire.beginTransmission(0x68);                                        
  Wire.write(0x3B);                                                    
  Wire.endTransmission();                                              
  Wire.requestFrom(0x68,14);                                           //Request 14 bytes
  while(Wire.available() < 14);                                        //Wait until all received
  acc_x = Wire.read()<<8|Wire.read();                                  
  acc_y = Wire.read()<<8|Wire.read();                                  
  acc_z = Wire.read()<<8|Wire.read();                                  
  temp = Wire.read()<<8|Wire.read();                                   
  gyro_x = Wire.read()<<8|Wire.read();                                 
  gyro_y = Wire.read()<<8|Wire.read();                                 
  gyro_z = Wire.read()<<8|Wire.read();                                 
}

//MAIN Programe
void loop(){

  read_mpu_6050_data();   
 //Subtract offset from raw values
  gyro_x -= gyro_x_cal;                                                
  gyro_y -= gyro_y_cal;                                                
  gyro_z -= gyro_z_cal;                                                
         
  //Gyro angle calculations
  angle_pitch += gyro_x * 0.0000611;                                   
  angle_roll += gyro_y * 0.0000611;                                    
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               
  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               
  
  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  //Calculate total acc vector
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;       //pitch angle
  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;       //roll angle
  
  angle_pitch_acc -= 0.0;                                     //Calibration
  angle_roll_acc -= 0.0;                                    

  if(set_gyro_angles){                                                 //IMU already started
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct 
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;       
  }
  else{                                                                //At first start
    angle_pitch = angle_pitch_acc;                                     //Set 
    angle_roll = angle_roll_acc;                                        
    set_gyro_angles = true;         
  }
  
  //Complementary filter
  angle_pitch_output = angle_pitch_output * 0.98 + angle_pitch * 0.02;   
  angle_roll_output = angle_roll_output * 0.98 + angle_roll * 0.02;      
  Serial.print(" | Angle  = "); Serial.println(angle_roll_output);

 while(micros() - loop_timer < 4000);                                 //Wait
 loop_timer = micros();//Reset loop timer
  
}
