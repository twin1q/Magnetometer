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

#include <Wire.h>                       // I2C library 
#include <math.h>                       // For trig functions

// Magnetometer data structure
struct MagnetometerDirection
{
    int X , Y, Z;   // Vectors
    float Degrees;  // Direction of north in degree's
    
    MagnetometerDirection(int x = 0, int y = 0, int z = 0, float degrees = 0.0f): 
        X(x), Y(y), Z(z), Degrees(degrees) { } 
};

class Magnetometer
{ 
    public:
        Magnetometer();                 // Constructor for Class
                                        // Initializes the magnetometer
                                        // This function runs when the class is created

        MagnetometerDirection Read();   // Returns magnetometer data
        
        // Gets I2C status. Check this after the object is created and 
        // after every read to ensure the data is valid
        unsigned char GetAckStatus()
        {
            return ackStatus;           // 0: ACK (success), 1: data too long for buffer,       
                                        // 2: NACK on address, 3: NACK on data
                                        // 4: other error, 5: Read error
        }
    private:
        // I2C ackStatus
        unsigned char ackStatus;
};