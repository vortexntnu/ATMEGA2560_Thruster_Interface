#include <I2CSlave.h>

#define I2C_ADDR 0x8

volatile char data;

void I2C_received(uint8_t received_data)
{
  data = received_data;
  Serial.println(data);
}

void I2C_requested()
{
  I2C_transmitByte(data);
}

int main()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Serial init...");
  // set received/requested callbacks
  I2C_setCallbacks(I2C_received, I2C_requested);

  // init I2C
  I2C_init(I2C_ADDR);

  
  // Main program loop
  while(1) {
  }
}
