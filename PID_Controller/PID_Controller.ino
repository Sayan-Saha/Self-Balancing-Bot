//PID Controler 

//Varriables
double Kp=0 ;    //Proportional Constant
double Ki=0 ;    //Integral Constant
double Kd=0 ;    //Differential Constant

double current_angle = 0;
double Iterm = 0;
double last_angle = 0;

double Calculate(double input_angle)
{ 
 double error = current_angle - input_angle;   //error
 
 Iterm = Iterm + ( Ki * error );     //Integral Part
 if( Iterm > 255 )
   Iterm = 255;     //Mapping the Value
  else if ( Iterm < -255 )
    Iterm = -255;    //Mapping the Value
  
  double d_error =  input_angle - last_angle;   //dtheta-by-dt
  
  double output = ( Kp * error ) + Iterm + ( Kd * d_error );   //PID
  
  if( output > 255 )
   output = 255;     //Mapping the Value
  else if ( output < -255 )
    output = -255;    //Mapping the Value
    
    last_angle = input_angle;
    return output;
}

double Get_current_angle()
{
  return current_angle;
}

double Set_current_angle( double a )
{
  current_angle = a ;
}
