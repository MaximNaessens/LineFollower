#define SerialPort Serial //schakelen bluetooth  en serial port
#define Baudrate 38400

const int sensor [] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7}; //volgorde omdraaien indien links rechts is 

void setup()
{
}
void loop()
  {
    SerialPort.print("Values: ");
    for (int i = 0; i < 6; i++)
    {

      SerialPort.print(analogRead(sensor[i]));
      SerialPort.print(" ");
    }
    SerialPort.println();

  }
