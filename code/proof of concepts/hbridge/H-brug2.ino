#define SerialPort Serial3 //schakelen bluetooth  en serial port
#define Baudrate 38400
#define MotorPinL_F PA8
#define MotorPinL_B PA9
#define MotorPinR_F PA10
#define MotorPinR_B PB6
int MotorL_F = 0;
int MotorL_B = 0;
int MotorR_F = 0;
int MotorR_B = 0;
void setup()
{ SerialPort.begin(Baudrate);
  pinMode(MotorPinL_F, OUTPUT);
  pinMode(MotorPinL_B, OUTPUT);
  pinMode(MotorPinR_F, OUTPUT);
  pinMode(MotorPinR_B, OUTPUT);}
void loop(){
{
{  for(int MotorL_F=0; MotorL_F<=255; MotorL_F++) 
{  delay(25);
   analogWrite(MotorPinL_F, MotorL_F);  }
}
{  for(int MotorL_F=255; MotorL_F>=0; MotorL_F--)
{  delay(25);
   analogWrite(MotorPinL_F, MotorL_F);  }
}
{  for(int MotorL_B=0; MotorL_B<=255; MotorL_B++)
{  delay(25);
   analogWrite(MotorPinL_B, MotorL_B);  }
}
{  for(int MotorL_B=255; MotorL_B>=0; MotorL_B--)
{  delay(25);
   analogWrite(MotorPinL_B, MotorL_B);  }
}
{  for(int MotorR_F=0; MotorR_F<=255; MotorR_F++)
{  delay(25);
   analogWrite(MotorPinR_F, MotorR_F);  }
}    
{  for(int MotorR_F=255; MotorR_F>=0; MotorR_F--)
{  delay(25);
   analogWrite(MotorPinR_F, MotorR_F);  }
}
{  for(int MotorR_B=0; MotorR_B<=255; MotorR_B++)
{  delay(25);
   analogWrite(MotorPinR_B, MotorR_B);  }
}    
{  for(int MotorR_B=255; MotorR_B>=0; MotorR_B--)
{  delay(25);
   analogWrite(MotorPinR_B, MotorR_B);  }
}
}
}
