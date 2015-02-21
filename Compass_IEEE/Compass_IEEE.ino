/*
    Created by: Noe Quintero

    Description:
        This .ino file shows how to use the Magnetometer library. It is intended to 
        be used as a compass when fully assembled.

    Disclaimer:
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
        DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
        ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
        (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
        LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
        ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Wire.h>                               // I2C library 
#include <math.h>                               // For trig functions   
#include "Magnetometer.h"                       // Magnetometer library

// Global Constants
const int LedPinNorth       = 7;
const int LedPinNorthEast   = 3;
const int LedPinEast        = 2;
const int LedPinSouthEast   = 4;
const int LedPinSouth       = 5;
const int LedPinSouthWest   = 6;
const int LedPinWest        = 8;
const int LedPinNorthWest   = 9; 

const int LedSequence[] = {
                            LedPinNorth,
                            LedPinNorthEast,
                            LedPinEast,
                            LedPinSouthEast,
                            LedPinSouth,
                            LedPinSouthWest,
                            LedPinWest,
                            LedPinNorthWest
                          };    // Used to sequence the LED in a pattern 

// Function Decoration
void setLED(float degrees);     //  Turns on an LED based on it's Direction.

// Initializes the Arduino
void setup()
{
    Serial.begin(115200);                       //  Initialize the USART port to 115200 baud     
    Serial.println(F("IEEE Compass Project 2015")); // Display compass  
    Serial.println();                           // Send a new line char to USART port 
    
    Wire.begin();                               // Initialize the I2C port 
    
    // Initialize the led
    for(int i = 0; i <= 6; i++)
    {
        pinMode(LedSequence[i],OUTPUT);
    }
    
    bool ledState;
    
    // Fancy blinking sequence
    for(int i = 0; i <= 5; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            ledState = digitalRead(LedSequence[j]);
            digitalWrite(LedSequence[j], (!ledState));
            delay(100);
        }
    }
}

// Main loop
void loop()
{
    // Create Magnetometer class called compass
    // Note: This should be static sine the constructor will
    //       will be executed every time the class is created.
    static Magnetometer compass;
    
    // Create the compass direction structure.
    // Note: does not have to be static.
    static MagnetometerDirection compassDirection;
    
    // Create the ack status var
    // Note: does not have to be static.
    static unsigned char ackStatus;
    
    // Read the magnetometer
    // Returns the magnetometer direction
    compassDirection = compass.Read();
    
    ackStatus = compass.GetAckStatus(); // Get the ack status
                                        // 0: ACK (success), 1: data too long for buffer,       
                                        // 2: NACK on address, 3: NACK on data
                                        // 4: other error, 5: Read error
    if(ackStatus != 0)
    {
        Serial.println(F("I2C Error occurred"));    // Display message to USART port
        while(1);                       // Loop forever
    }
    else
    {
        Serial.print(F("North in degree's: "));     // Display message to USART port
        Serial.println(compassDirection.Degrees);   // Display the degree to USART port
        setLED(compassDirection.Degrees);           // Set LED's based on degrees
    }
}

// Function Definition

// This function set's the LED's to point to north.
void setLED(float degrees)
{
    if(degrees <= 22.5 || degrees > 337.5)
    {    
        digitalWrite(LedPinEast,        LOW);
        digitalWrite(LedPinNorthEast,   LOW);
        digitalWrite(LedPinSouthEast,   LOW);
        digitalWrite(LedPinSouth,       LOW);
        digitalWrite(LedPinSouthWest,   LOW);
        digitalWrite(LedPinNorth,       HIGH);
        digitalWrite(LedPinWest,        LOW);
        digitalWrite(LedPinNorthWest,   LOW);
    }
    else if( degrees > 22.5 && degrees <= 67.5)
    {    
        digitalWrite(LedPinEast,        LOW);
        digitalWrite(LedPinNorthEast,   LOW);
        digitalWrite(LedPinSouthEast,   LOW);
        digitalWrite(LedPinSouth,       LOW);
        digitalWrite(LedPinSouthWest,   LOW);
        digitalWrite(LedPinNorth,       LOW);
        digitalWrite(LedPinWest,        LOW);
        digitalWrite(LedPinNorthWest,   HIGH);
    }
    else if( degrees > 67.5 && degrees <= 112.5)
    {    
        digitalWrite(LedPinEast,        LOW);
        digitalWrite(LedPinNorthEast,   LOW);
        digitalWrite(LedPinSouthEast,   LOW);
        digitalWrite(LedPinSouth,       LOW);
        digitalWrite(LedPinSouthWest,   LOW);
        digitalWrite(LedPinNorth,       LOW);
        digitalWrite(LedPinWest,        HIGH);
        digitalWrite(LedPinNorthWest,   LOW);
    }
    else if( degrees > 112.5 && degrees <= 157.5)
    {    
        digitalWrite(LedPinEast,        LOW);
        digitalWrite(LedPinNorthEast,   LOW);
        digitalWrite(LedPinSouthEast,   LOW);
        digitalWrite(LedPinSouth,       LOW);
        digitalWrite(LedPinSouthWest,   HIGH);
        digitalWrite(LedPinNorth,       LOW);
        digitalWrite(LedPinWest,        LOW);
        digitalWrite(LedPinNorthWest,   LOW);
    }
    else if( degrees > 157.5 && degrees <= 202.5)
    {    
        digitalWrite(LedPinEast,        LOW);
        digitalWrite(LedPinNorthEast,   LOW);
        digitalWrite(LedPinSouthEast,   LOW);
        digitalWrite(LedPinSouth,       HIGH);
        digitalWrite(LedPinSouthWest,   LOW);
        digitalWrite(LedPinNorth,       LOW);
        digitalWrite(LedPinWest,        LOW);
        digitalWrite(LedPinNorthWest,   LOW);
    }
    else if( degrees > 202.5 && degrees <= 247.5)
    {    
        digitalWrite(LedPinEast,        LOW);
        digitalWrite(LedPinNorthEast,   LOW);
        digitalWrite(LedPinSouthEast,   HIGH);
        digitalWrite(LedPinSouth,       LOW);
        digitalWrite(LedPinSouthWest,   LOW);
        digitalWrite(LedPinNorth,       LOW);
        digitalWrite(LedPinWest,        LOW);
        digitalWrite(LedPinNorthWest,   LOW);
    }
    else if( degrees > 247.5 && degrees <= 292.5)
    {    
        digitalWrite(LedPinEast,        HIGH);
        digitalWrite(LedPinNorthEast,   LOW);
        digitalWrite(LedPinSouthEast,   LOW);
        digitalWrite(LedPinSouth,       LOW);
        digitalWrite(LedPinSouthWest,   LOW);
        digitalWrite(LedPinNorth,       LOW);
        digitalWrite(LedPinWest,        LOW);
        digitalWrite(LedPinNorthWest,   LOW);
    }
    else if( degrees > 292.5 && degrees <= 337.5)
    {    
        digitalWrite(LedPinEast,        LOW);
        digitalWrite(LedPinNorthEast,   HIGH);
        digitalWrite(LedPinSouthEast,   LOW);
        digitalWrite(LedPinSouth,       LOW);
        digitalWrite(LedPinSouthWest,   LOW);
        digitalWrite(LedPinNorth,       LOW);
        digitalWrite(LedPinWest,        LOW);
        digitalWrite(LedPinNorthWest,   LOW);
    }
    else
    {
        // Degrees are out of range
        // Error with data
        // Flash all LEDs on and off
        for(int i = 2; i<=9; i++)
        {
            digitalWrite(LedSequence[i], LOW);
        }
        delay(500);
        for(int i = 2; i<=9; i++)
        {
            digitalWrite(LedSequence[i], HIGH);
        }
        delay(500);
    }
}
