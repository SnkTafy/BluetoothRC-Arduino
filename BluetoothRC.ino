#include <SoftwareSerial.h>
#include <StepperControl.h>

#define DC_MOTOR_PIN1 3
#define DC_MOTOR_PIN2 4
#define DC_MOTOR_PIN3 11
#define DC_MOTOR_PIN4 12

SoftwareSerial bluetoothSerial(5,6);

String Data = "";
String xAxis = "";
String yAxis = "";
int commaFlag = 0;
int xAxisInt = 0;
int prevXAxisInt = 0;
int yAxisInt = 0;

void setup()  
{
    bluetoothSerial.begin(9600);
    Serial.begin(9600);

    pinMode(IN1, OUTPUT); 
    pinMode(IN2, OUTPUT); 
    pinMode(IN3, OUTPUT); 
    pinMode(IN4, OUTPUT);

    pinMode(DC_MOTOR_PIN1, OUTPUT);
    pinMode(DC_MOTOR_PIN2, OUTPUT);
    pinMode(DC_MOTOR_PIN3, OUTPUT);
    pinMode(DC_MOTOR_PIN4, OUTPUT);
    //analogWrite(DC_MOTOR_PIN1, 200);
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
              //Serial.println(Data);
              //Serial.println('\n');

              //Converting the string to integer
              //Left Right turn
              xAxisInt = xAxis.toInt();
              //Forward Backward speed
              yAxisInt = yAxis.toInt();
              
              if((yAxisInt <= 255) && (yAxisInt >= 0)){
                digitalWrite(DC_MOTOR_PIN4, LOW);
                digitalWrite(DC_MOTOR_PIN2, HIGH);
                analogWrite(DC_MOTOR_PIN3, 0);
                analogWrite(DC_MOTOR_PIN1, yAxisInt);
              }
              else if((yAxisInt >= -255) && (yAxisInt < 0)){
                digitalWrite(DC_MOTOR_PIN4, HIGH);
                digitalWrite(DC_MOTOR_PIN2, LOW);
                analogWrite(DC_MOTOR_PIN3, -yAxisInt);
                analogWrite(DC_MOTOR_PIN1, 0);
              }
              
              /*
               * Divide the bluetooth value by 5.
               * Because the biggest value that i receive from bluetooth
               * is 300 and i do not want so much steps
               * 60 steps is enough to turn my car to left of right
               * that why i am dividing the bluetooth by 5. 300/5 = 60
              */
              xAxisInt /= 10;
              /*
               * You may wonder why you check the value if it is between 60 to -60!!
               * The reason is that the bluetooth receives very quickly the data, but the
               * step motor is very slow to execute all the steps.
               * Because of that sometimes i have somethink like data overflow.
               * I receive bigger values than i expect!!!!!!
              */
              if((xAxisInt < 61) && (xAxisInt > -61) && (xAxisInt != prevXAxisInt)){
                //call the turn stepper to make a step
                TurnStepper(xAxisInt);
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
