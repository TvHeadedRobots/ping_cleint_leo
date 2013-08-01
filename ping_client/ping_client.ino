/**
 * A Mirf example to test the latency between two Ardunio.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

// Read and print RF_SETUP 
byte rf_setup = 0;
byte reg_val = 0;
byte reg_wr = 01;
void setup(){
  Serial.begin(9600);
  delay(5000);
  /*
   * Setup pins / SPI.
   */
   
  /* To change CE / CSN Pins:
   * 
   * Mirf.csnPin = 9;
   * Mirf.cePin = 7;
   */

  Mirf.cePin = 8;
  Mirf.csnPin = 7;

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  /*
   * Configure reciving address.
   */
   
  Mirf.setRADDR((byte *)"clie1");
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  Mirf.payload = sizeof(unsigned long);
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  /*
   * To change channel:
   * 
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */
   
  Mirf.config();
  
  Mirf.readRegister( RF_SETUP, &rf_setup, sizeof(rf_setup) );
  Mirf.readRegister( RF_PWR, &reg_val, sizeof(reg_val) );
  Serial.print( "rf_setup = " );
  Serial.println( rf_setup, BIN );
  Serial.print("Register Value: ");
  Serial.println(reg_val, BIN);  
  
  Serial.println("Beginning ... "); 
}

void loop(){
  unsigned long time = millis();
  unsigned long data = 123456; 
  
  Mirf.setTADDR((byte *)"serv1");
  
  //Mirf.send((byte *)&data);
  Mirf.send((byte *)&time);
  
  while(Mirf.isSending()){
  }
  Serial.println("Finished sending");
  
  delay(10);
  while(!Mirf.dataReady()){
    Serial.println("Waiting");
    if ( ( millis() - time ) > 1000 ) {
      Serial.println("Timeout on response from server!");
      return;
    }
  }
  
  Mirf.getData((byte *) &data);
  Mirf.getData((byte *) &time);
  //Serial.print("data: ");
  //Serial.println(data);
  Serial.print("Ping: ");
  Serial.println((millis() - time));
  
  delay(1000);
}
  
  
  
