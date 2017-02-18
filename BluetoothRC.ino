/*
 * BluetoothRC is written to receive data from BluetoothRC Android Application via bluetooth.
 * 
 * Parts that used for this project:
 * 1) Arduino Nano
 * 2) Bluetooth HC-05
 * 3) L293B Motor Controller
 * 4) Servo Motor (To Turn left and right)
 * 5) Dc Motor (To control the speed)
 * 6) PCB (Printed Circuit Board)
 * 7) Old broken toy car (If you do not have buy new one :) )
 * 8) Old phone batteries for power supply (Of course new batteries is better but no money no honey!!! )
 * PCB file is also attached at the same folder(BluetoothRC_PCB.pdf)
 * If you have not made PCB just ask the uncle Google ;)
 * 
 * Servo.h IS NOT USED IN THIS PROJECT. Because if you try to use
 * both SoftwareSerial.h and Servo.h servo acts like crazy.
 * Servo is controlled with analogWrites.
 * 
 * The only thing that i did not figure out is the noise in servo motor!!!
 * If someone can help please send me an email!!!
 */
#include <SoftwareSerial.h>

/*
 * DC MOTOR CONTROL PINS
 * Pin 3 is a PWM pin, because via this pin Motor's speed
 * will be controlled.
 * PWM pins support analogWrite and get values from 0 to 255.
 * Which means that we control the voltage that will go out through that pin.
 * 2 and 4 are just digital pins
 */
#define DC_MOTOR_ENABLE_PIN 3
#define DC_MOTOR_DIRA 2
#define DC_MOTOR_DIRB 4

/*
 * Pin 11 is a PWM pin and is used to control the servo with analogWrites
 */
#define SERVO_MOTOR 11
//Servos default position
#define SERVO_START_POS 120

/*
 * DEFINING BLUETOOTH PINS
 * 5=RX pin => connected to TX pin in bluetooth
 * 6=TX pin => connected to RX pin in bluetooth
 * WARNING: BOTH 5 AND 6 IS PWM PINS!!! (pins with ~ sing)
 */
SoftwareSerial bluetoothSerial(5,6);

String Data = "";
String xAxis = "";
String yAxis = "";
int commaFlag = 0;
int xAxisInt = 0;
int prevXAxisInt = 0;
int yAxisInt = 0;
int newPosition = 0;

void setup()  
{
    bluetoothSerial.begin(9600);
    Serial.begin(9600);

    pinMode(DC_MOTOR_ENABLE_PIN, OUTPUT);
    pinMode(DC_MOTOR_DIRA, OUTPUT);
    pinMode(DC_MOTOR_DIRB, OUTPUT);

    pinMode(SERVO_MOTOR, OUTPUT);
    analogWrite(SERVO_MOTOR, SERVO_START_POS);
    delay(10);
}

void loop() // run over and over
{
    while (bluetoothSerial.available() > 0)
    {
        char character = bluetoothSerial.read(); // Receive a single character from the software serial port
        if (character == '<')
        {
          Data.concat(character); // Add the received character to the receive buffer
          while (character != '>')
          {
            character = '\0';
            character = bluetoothSerial.read(); // Receive a single character from the software serial port

            // Check if the character is (,) or (-) or is a number and the add it to Data string
            if( character == 44 || character == 45 || (character >= 48 && character <= 57))
            {
              /*
               * If the current character is comma, then increase the commaFlag.
               * Which means, while the character is not comma,
               * concat the character to xAxis string, else concat
               * the current character to yAxis string.
               */
              if(character == 44 ) commaFlag++;
              //Catch the value for the X axis(Left and Right)
              if(commaFlag == 0) xAxis.concat(character);
              //Catch the value for the Y axis(Front and back)
              else if(character != 44) yAxis.concat(character);
               
              Data.concat(character); // Add the received character to the receive buffer
            }
       
          }

          if (character == '>')
          {
              Data.concat(character);

              //Converting the string to integer
              //Left Right turn
              xAxisInt = xAxis.toInt();
              //Forward Backward speed
              yAxisInt = yAxis.toInt();
              
              if((yAxisInt <= 255) && (yAxisInt >= 0)){
                digitalWrite(DC_MOTOR_DIRA, LOW);
                digitalWrite(DC_MOTOR_DIRB, HIGH);
                analogWrite(DC_MOTOR_ENABLE_PIN, yAxisInt);
              }
              else if((yAxisInt >= -255) && (yAxisInt < 0)){
                digitalWrite(DC_MOTOR_DIRA, HIGH);
                digitalWrite(DC_MOTOR_DIRB, LOW);
                analogWrite(DC_MOTOR_ENABLE_PIN, -yAxisInt);
              }
              
              /*
               * Divide the bluetooth value by 10.
               * Because the maximum value that i receive from bluetooth
               * is 300 and i do not want so big angle to turn.
               * 30 degrees angle is enough to turn my car to left of right
               * that's why i am dividing the bluetooth value by 10.
               * 300/10 = 30
               */
              xAxisInt /= 10;
              /*
               * You may wonder why you check the value if it is between 30 to -30!!
               * The reason is that the bluetooth receives very quickly the data, but the
               * servo motor is very slow.
               * Because of that sometimes i have something like data overflow.
               * I receive bigger values than i expect!!!!!!
               */
              if((xAxisInt < 31) && (xAxisInt > -31) && (xAxisInt != prevXAxisInt)){
                /*
                 * If the newPosition is  > SERVO_START_POS will turn left
                 * If the newPosition is < SERVO_START_POS will turrn right
                 */
                newPosition = SERVO_START_POS - (-xAxisInt);
                analogWrite(SERVO_MOTOR, newPosition);
              }
  
              // Clear received buffers so we're ready to receive the next line
              Data = "";
              xAxis = "";
              yAxis = "";
              // Set default values to variables
              prevXAxisInt = xAxisInt;
              xAxisInt = 0;
              yAxisInt = 0;
              commaFlag = 0;
          }
        }
    }
}
