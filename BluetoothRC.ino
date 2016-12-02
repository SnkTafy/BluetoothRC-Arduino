#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11);
String Data = "";

void setup()  
{
    mySerial.begin(9600);
    Serial.begin(9600);
}

void loop() // run over and over
{
    while (mySerial.available() > 0)
    {
        char character = mySerial.read(); // Receive a single character from the software serial port

        if (character == '<')
        {
          Data.concat(character); // Add the received character to the receive buffer
          while (character != '>')
          {
            character = '\0';
            character = mySerial.read(); // Receive a single character from the software serial port

            // Check if the character is (,) or (-) or is a number and the add it to Data string
            if(character == 44 || character == 45 || (character >= 48 && character <= 57))
            {
              Data.concat(character); // Add the received character to the receive buffer
            }
       
          }
          if (character == '>')
          {
              Data.concat(character);
              //Serial.print("Received: ");
              Serial.println(Data);
              Serial.println('\n');
  
              // Add your code to parse the received line here....
  
              // Clear receive buffer so we're ready to receive the next line
              Data = "";
          }
        }
    }
}
