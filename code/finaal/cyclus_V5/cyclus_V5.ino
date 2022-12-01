#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial3 //schakelen bluetooth  en serial port
#define Baudrate 38400
#define Led PC13
#define Led_Power PB12
#define Led_Running PB13
#define Led_Connect PB14
#define MotorPinL_F PA8
#define MotorPinL_B PA9
#define MotorPinR_F PA10
#define MotorPinR_B PB6
#define Drukknop PB4


bool debug;
int MotorL_F = 0;
int MotorL_B = 0;
int MotorR_F = 0;
int MotorR_B = 0;
bool Start;
float iTerm = 0;
float lastErr;
float position;
float Old_position;
int Old_PosBlack;
unsigned long previous, calculationTime;
const int sensor [] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7}; //volgorde omdraaien indien links rechts is 
unsigned long vorige;
bool rondje;
long normalised[8];
uint16_t i, j;




SerialCommand sCmd(SerialPort);

struct param_t
{
  unsigned long cycleTime;
  /* andere parameters die in het eeprom geheugen moeten opgeslagen worden voeg je hier toe ... */
  int black[8];
  int white[8];
  float kp;
  float ki;
  float kd;
  float diff;
  float power;
} params;

void setup()
{
  SerialPort.begin(Baudrate);
  pinMode(Led, OUTPUT);
  pinMode(Led_Power, OUTPUT);
  pinMode(Led_Running, OUTPUT);
  pinMode(Led_Connect, OUTPUT);
  pinMode(MotorPinL_F, OUTPUT);
  pinMode(MotorPinL_B, OUTPUT);
  pinMode(MotorPinR_F, OUTPUT);
  pinMode(MotorPinR_B, OUTPUT);
  pinMode(Drukknop, INPUT_PULLUP);
  sCmd.addCommand("Set", onSet);
  sCmd.addCommand("Debug", onDebug);
  sCmd.addCommand("Calibrate", onCalibrate);
  sCmd.addCommand("Start", onStart);
  sCmd.setDefaultHandler(onUnknownCommand);

  i=0;
  j=0;
  EEPROM_readAnything(0, params);

  pinMode(13, OUTPUT);
  digitalWrite(Led_Power, HIGH);
  SerialPort.println("----------------------");
  SerialPort.println("   Robot van Maxim    ");
  SerialPort.println("----------------------");
  SerialPort.println("  Ready for take off  ");
  SerialPort.println("----------------------");
  
  vorige=micros();
}

void loop()
{
  rondje = false;
  sCmd.readSerial();
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;
    int PosBlack = 0;
    bool Line = 0;
    float powerLeft;
    float powerRight;

    // SerialPort.print("Sensors: ");
    for (int i = 0; i < 8; i++)
    {
      long value = analogRead(sensor[i]);

      // if (value < params.white[i]) { SerialPort.print("Opnieuw calibreren witte sensor : "); SerialPort.println(i+1);} //Controleren of sensor opnieuw gekalibreerd moet worden in Wit.
      // if (value > params.black[i]) { SerialPort.print("Opnieuw calibreren zwarte sensor: "); SerialPort.println(i+1);} //Controleren of sensor opnieuw gekalibreerd moet worden in Zwart.
      
      normalised[i] = constrain(value, params.white[i], params.black[i]);
      normalised[i] = map(normalised[i], params.black[i], params.white[i], 0, 4096); //Kan aangepast worden naargelang de hoogte, omdraaien 0 en 4000 indien je wit laag wilt en zwart hoog

      if (normalised[i] < normalised[PosBlack]) PosBlack = i;
      
      
    }
if ((normalised[PosBlack] < 1000)) {
     if (PosBlack == 0) position = -4;
     else if (PosBlack == 7) position = 4;
     else {

     long Szero = normalised[PosBlack];
     long SminusOne = normalised[PosBlack-1];
     long SplusOne = normalised[PosBlack+1];

     float b = (SplusOne - SminusOne);
      b = b / 2;
      float a = SplusOne -b -Szero;

     position = -b / (2 * a);
     position += PosBlack - 3.5; 
     Old_position = position; 
     } 
} else 
{
  position = Old_position;
  
  //start_auto();
}
     
     position *= 9.525;  //sensor distance in mm 

      /* bereken error = setpoint - positie */
      float error = -position; 
      
      /* proportioneel regelen */
      float output = error * params.kp;
      
      /* integrerend regelen */
      iTerm += params.ki*error;
      iTerm = constrain(iTerm, -510, 510);
      output += iTerm;
     
      /* differentiërend regelen */
      output = output + (params.kd * (error - lastErr));
      lastErr = error;
      
      /* output begrenzen tot wat fysiek mogelijk is */
      output = constrain(output, -510, 510);
      

      if (output >= 0)
      {
        powerLeft = constrain(params.power + params.diff * output, -255, 255);
        powerRight = constrain(powerLeft - output, -255, 255);
        powerLeft = powerRight + output;
      }
      else
      {
        powerRight = constrain(params.power - params.diff * output, -255, 255);
        powerLeft = constrain(powerRight + output, -255, 255);
        powerRight = powerLeft - output;
      }
     if (Start == true) {
     if (powerLeft > 0)
      {
        MotorL_F = constrain(abs(powerLeft), 0, 255);
        MotorL_B = 0;
      }
      else
      {
        MotorL_F = 0;
        MotorL_B = constrain(abs(powerLeft), 0, 255);
      }
           if (powerRight > 0)
      {
        MotorR_F = constrain(abs(powerRight), 0, 255);
        MotorR_B = 0;
      }
      else
      {
        MotorR_F = 0;
        MotorR_B = constrain(abs(powerRight), 0, 255);
      }
     } else {
        MotorR_F = 0;
        MotorR_B = 0;
        MotorL_F = 0;
        MotorL_B = 0;
      }

    /* code die cyclisch moet uitgevoerd worden programmeer je hier ... */
  }
   analogWrite(MotorPinL_F, MotorL_F);
   analogWrite(MotorPinL_B, MotorL_B);
   analogWrite(MotorPinR_F, MotorR_F);
   analogWrite(MotorPinR_B, MotorR_B);
  
  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;

bool Start_knop = digitalRead(Drukknop);
bool flip;
   if ((Start_knop == LOW) && (flip == LOW)) {
       start_auto();
       flip = 1;
       delay(300);
       
  } else if (Start_knop == HIGH) {
       flip = 0;   
  }
}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
    if (strcmp(param, "cycle") == 0)
  {
    long newCycleTime = atol(value);
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;
    params.kd /= ratio;

    params.cycleTime = newCycleTime;
    SerialPort.print("Cycle set on: ");
    SerialPort.println(newCycleTime);
  }
  else if (strcmp(param, "ki") == 0)
  {
    float sampleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.ki = atof(value) * sampleTimeInSec;
    SerialPort.print("Ki set on: ");
    SerialPort.println(value);
  }
  else if (strcmp(param, "kd") == 0)
  {
    float sampleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.kd = atof(value) / sampleTimeInSec;
    SerialPort.print("Kd set on: ");
    SerialPort.println(value);
  }else if (strcmp(param, "kp") == 0){
    params.kp = atof(value);
    SerialPort.print("Kp set on: ");
    SerialPort.println(value);
    }else if (strcmp(param, "power") == 0){
    params.power = atof(value);
    SerialPort.print("Power set on: ");
    SerialPort.println(value);
    }else if (strcmp(param, "diff") == 0){
    params.diff = atof(value);
    SerialPort.print("Diff set on: ");
    SerialPort.println(value);}
  EEPROM_writeAnything(0, params);
}
void start_auto(){
  SerialPort.println("Start");
  lastErr = 0;
  iTerm = 0;
  Start = !Start; 
  Old_position = 0;
  Old_PosBlack = 0;
  digitalWrite(Led_Running,Start);
}

void onStart()
{
 start_auto();
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
  SerialPort.print("Kp: ");
  SerialPort.println(params.kp);

  float sampleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / sampleTimeInSec;
  SerialPort.print("Ki: ");
  SerialPort.println(ki);

  float kd = params.kd * sampleTimeInSec;
  SerialPort.print("Kd: ");
  SerialPort.println(kd);

  SerialPort.print("Power: ");
  SerialPort.println(params.power);

  SerialPort.print("Diff: ");
  SerialPort.println(params.diff);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
    //waarden sensor
    SerialPort.print("Sensors: ");
    for (int i = 0; i < 8; i++)
    {
      long value = analogRead(sensor[i]);      
      normalised[i] = constrain(value, params.white[i], params.black[i]);
      normalised[i] = map(normalised[i], params.black[i], params.white[i], 0, 4096);
      SerialPort.print(normalised[i]);
      SerialPort.print(" ");
    }
    SerialPort.println();
    
    //waarden black
    SerialPort.print("Black: ");
    for (int i = 0; i < 8; i++)
    {
      SerialPort.print(params.black[i]);
      SerialPort.print(" ");
    }
    SerialPort.println();
    
    //waarden white
    SerialPort.print("White: ");
    for (int i = 0; i < 8; i++)
    {
      SerialPort.print(params.white[i]);
      SerialPort.print(" ");
    }
    SerialPort.println();

    SerialPort.print("calculation time: ");
    SerialPort.println(calculationTime);
    calculationTime = 0;
}
void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    for (int i = 0; i < 8; i++) {
      
      params.black[i] = 0; 
      
      for (int j = 0; j < 50; j++) //50x de waarde uilezen
        {
          int valueC = analogRead(sensor[i]);
          if (valueC > params.black[i])
          {
            params.black[i] = valueC;
            delay(50);
          }
        }
    } 
      SerialPort.println("done");
      
          SerialPort.print("Black: ");
          for (int i = 0; i < 8; i++)
          {
            SerialPort.print(params.black[i]);
            SerialPort.print(" ");
          }
          SerialPort.println();  
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");    
    for (int i = 0; i < 8; i++) { 

      params.white[i] = 4096;
  
      for (int j = 0; j < 50; j++) //50x de waarde uilezen
        {
          int valueC = analogRead(sensor[i]);
          if (valueC < params.white[i])
          {
            params.white[i] = valueC;
            delay(50);
          }
        }
    
    }
    SerialPort.println("done");   
    SerialPort.print("White: ");
    for (int i = 0; i < 8; i++)
    {
      SerialPort.print(params.white[i]);
      SerialPort.print(" ");
    }
    SerialPort.println();

  }

  EEPROM_writeAnything(0, params);
}
