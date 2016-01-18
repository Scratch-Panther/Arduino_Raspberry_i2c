/* 433MHz Transmitter using Virtual Wire library to 
 * send a float to a remote arduino.
 * 
 * Connect 433MHz transmitter as follows:-
 * VCC to VIN
 * GND to GND
 * ATAD to pin 3
 *
 * Connect DS18B20 as follows:-
 * VCC to 5V
 * OUT to pin 5
 * GND to GND
 */

/* Libraries */
// for 433MHz transmitter
#include <Wire.h>        
// for 433mHZ transmitter
#include <VirtualWire.h> 
// for DS20b18 one wire temperature sensor
#include <OneWire.h>     

/* Program Variables */
// Transmission char array
char t[8];
// 433 MHz Data pin
const int DATAPIN = 3;
// DS20b18 Temperature Sensor data pin
const int TEMPPIN = 5;
// Tranmission data rate
const int DATARATE = 500;
// temperature data
float temperatureReading;
// temporary string 
String toBeSent;
// shortcut one wire device name
OneWire  ds(TEMPPIN);

/* Program Setup */
void setup()
{
  // setup transmit pin as pin 3
  vw_set_tx_pin(DATAPIN); 
  // Bits per sec
  vw_setup(DATARATE);	
  // wait for half a second 
  delay(500);          
}

/* Program Main Loop */
void loop()
{
  // get float temperature from sensor
  temperatureReading = getTemp();
  // convert float to string
  dtostrf(temperatureReading,4,2,t);
  //transmitted text
  char *msg1 = t; 
  // send message with string length
  vw_send((uint8_t*)msg1, strlen(msg1)); 
  // Wait until the whole message is gone
  vw_wait_tx(); 
  // Wait 30 seconds
  delay(30000);
}

/* gets DS20b18 temperature
float getTemp()
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8] = {0x28, 0xff, 0x18, 0x4c, 0x50, 0x14, 0x04, 0x35
};
  float celsius;
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  return celsius;
}
