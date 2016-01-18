/* Arduino as slave connected to Raspbery Pi via i2c
 * with 433MHz receiver to receive remote sensor data.
 * A remote Arduino with a 433MHz transmitter has send 
 * a float using the Virtual Wire Library.
 *
 * Connect to Raspberry Pi as follows:-
 * GND to GND
 * SDA to SDA
 * SCL to SCL
 *
 * Connect to 433MHz Receiver as follows:-
 * VCC to 5V
 * GND to GND
 * DATA to pin 2
 */

/** Libraries */
// For i2c Communications
#include <Wire.h>        
// for 433MHz receiver
#include <VirtualWire.h>
// For i2c Communication - Slave
#define SLAVE_ADDRESS 0x04

/** Program Variables */
// Receiver Data pin
const int DATAPIN = 2;
// Receiver Data Rate
const int DATARATE = 500;
// Received Data - Char Array
char c[32];

/** Program setup */
void setup()
{   
  // Begin wire functions as slave
  Wire.begin(SLAVE_ADDRESS);      
  // On i2c request run following 
  Wire.onRequest(replyData);
  // Setup receive pin as pin 2
  vw_set_rx_pin(DATAPIN);      
  // Bits per sec   
  vw_setup(DATARATE);
  // Start the receiver PLL running  	  
  vw_rx_start();         
}

/* Program Main Loop */
void loop()
{
  // setup array with maximum data length
  uint8_t buf[VW_MAX_MESSAGE_LEN]; 
  // define buffer length
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  // if received data
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    // For Loop to go through each char receieved
    for (int i = 0; i < buflen; i++)
    {
      // grab each received char and load for transmission
      c[i] = char(buf[i]);  
    }   
  }
}

/* On i2c request receive Code */
void replyData()
{
  // convert char array of temperature to float
  float temperatureRead = atof(c);
  // Create 4 byte array for transmission data
  byte buffer[4];
  // load float number into byte array
  byte* b = (byte*) &temperatureRead;
  // For loop
  for (int i = 0 ; i < 4 ; i++)
  {
    // load data from byte array to transmission array
    buffer[i] = b[i];
  }
  // load data to i2c 
  Wire.write(buffer,4);
}
