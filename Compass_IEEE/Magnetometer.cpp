/*
    Created by: Noe Quintero

    Description:
        This library implements the HMC5883L digital compass to calculate North in 
        degree's with respect from its x and y axis. It also utilizes the Arduino's 
        I2C library and Honeywell Application Note 203.

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

#include "Magnetometer.h"

static const unsigned char MAGNETOMETER_ADDRESS = 0x1E; // Magnetometer 7-bit I2C Address

Magnetometer::Magnetometer()
{
    Wire.beginTransmission(MAGNETOMETER_ADDRESS);       // Load address to I2C buffer
    Wire.write(0x00);                                   // Set to register A
    Wire.write(0x70);                                   // 8 samples averages per measurement output
    Wire.write(0xA0);                                   // Gain = 5
    Wire.write(0x00);                                   // Continuous measurement mode
    ackStatus = Wire.endTransmission();                 // Send I2C buffer to I2C port
}

MagnetometerDirection Magnetometer::Read()
{
    unsigned char bytes_read;
    unsigned char status;
    int x,y,z;
    float degrees;
    do
    {
        Wire.beginTransmission(MAGNETOMETER_ADDRESS);   // Load address to I2C buffer
        Wire.write(0x09);                               // Set to status register
        ackStatus = Wire.endTransmission();             // Send I2C buffer to I2C port
        if(ackStatus != 0)
            return MagnetometerDirection();             // Return empty structure

        bytes_read = Wire.requestFrom(MAGNETOMETER_ADDRESS, 1); // Read status register
        if(bytes_read >= 1)
            status = Wire.read();
        else
        {
            ackStatus = 0x05;
            return MagnetometerDirection();             // Error reading data
        }
    } while (status & 0x01);                            // Poll until data is ready
    
    Wire.beginTransmission(MAGNETOMETER_ADDRESS);       // Load address to I2C buffer
    Wire.write(0x03);                                   // Set to X MSB register
    ackStatus = Wire.endTransmission();                 // Send I2C buffer to I2C port
    if(ackStatus != 0)
        return MagnetometerDirection();                 // Return empty structure

    // Read data from each axis
    bytes_read = Wire.requestFrom(MAGNETOMETER_ADDRESS, 6);
    if(bytes_read >= 6)
    {   // Data is in two's compliment
        x = Wire.read()<<8;                             // x most significant byte
        x |= Wire.read();                               // x least significant byte
        z = Wire.read()<<8;                             // z most significant byte
        z |= Wire.read();                               // z least significant byte
        y = Wire.read()<<8;                             // y most significant byte
        y |= Wire.read();                               // y least significant byte   
    }
    else
    {   
        ackStatus = 0x05;                               // Error reading data
        return MagnetometerDirection();                 // Return empty structure
    }
    
    // Calculate direction
    // Equations were referenced from Honeywell AN-203
    if(y > 0)
        degrees = 90.0 - (atan((float) x / y))*(180/3.14159265359); 
    else if(y < 0)
        degrees = 270.0 - (atan((float) x / y))*(180/3.14159265359);
    else if(x < 0)
        degrees = 180.0;
    else
        degrees = 0.0;
    return MagnetometerDirection(x, y, z, degrees);      // Return data structure 
}