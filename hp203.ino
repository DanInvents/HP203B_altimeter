// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HP203B
// This code is designed to work with the HP203B_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Catchall?sku=HP203B_I2CS#tabs-0-product_tabset-2

#include <Wire.h>

// HP203B I2C address is 0x76. You can change the address by soldering the jumper located underneath the board.
#define Addr 0x76
float told;
void setup()
{
  // Initialise I2C communication as Master
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(115200);
  delay(300);
}

void loop()
{
  unsigned int data[6];

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send OSR and channel setting command
  Wire.write(0x40 |0x00 | 0x00); //Here I don't know what the fist element is. The second is the 3-bit OSR, and the third the 2-bit CHNL
  //The higher the over sampling rate (OSR) the lower the measurement noise, and the longer time it takes to do a measurement
  //By default the OSR = 128, you can change it by replacing 0x14 with different numbers following the indications below:
  //OSR = 4096 (0x00); 2048 (0x04); 1024 (0x08); 512 (0x0C); 256 (0x10); 128 (0x14);
  //By setting the last hexadecimal number to 0x02 instead of 0x00 the sensor only measures the temperature.
  // Stop I2C transmission
  Wire.endTransmission();
  delay(50);

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x10); //Here we select the PT register for pressure and temperature measurement
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6); //Here we request from the sensor the bytes of data to compute the pressure and temperature

  // Read 6 bytes of data
  // cTemp msb, cTemp csb, cTemp lsb, pressure msb, pressure csb, pressure lsb
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }

  // Convert the data to 20-bits
  float cTemp = (((data[0] & 0x0F) * 65536) + (data[1] * 256) + data[2]) / 100.00;
  float fTemp = (cTemp * 1.8) + 32;
  float pressure = (((data[3] & 0x0F) * 65536) + (data[4] * 256) + data[5]) / 100.00;

  // Start I2C transmission


  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x31); //Here I request altitude data
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 3 bytes of data
  Wire.requestFrom(Addr, 3);

  // Read 3 bytes of data
  // altitude msb, altitude csb, altitude lsb
  if (Wire.available() == 3)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
  }

  // Convert the data to 20-bits
  float altitude = (((data[0] & 0x0F) * 65536) + (data[1] * 256) + data[2]) / 100.00;

  // Output data to serial monitor
  //Serial.print("Altitude : ");
  Serial.println(altitude);
  Serial.println(" m");
  Serial.print("Pressure : ");
  Serial.print(pressure);
  Serial.println(" Pa");
  Serial.print("Temperature in Celsius : ");
  Serial.print(cTemp);
  Serial.println(" C");
  Serial.print("Temperature in Fahrenheit : ");
  Serial.print(fTemp);
  Serial.println(" F");
  delay(100);
}
