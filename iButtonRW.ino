// Based on https://danman.eu/blog/cloning-ibutton-using-rw1990-and-avr/
// and: http://elimelecsarduinoprojects.blogspot.com/2013/06/read-dallas-ibutton-arduino.html
// By Swift Geek 28-08-2015
// Used 4.8kΩ Pull-up and 3.1 Vcc for arduino/pull-up

#include <OneWire.h>

OneWire ibutton (8); // I button connected on PIN 2.

byte addr[8]; //array to store the Ibutton ID.

void setup(){
 Serial.begin(115200); 
}

void loop(){
  if (!ibutton.search (addr)){//read attached ibutton and asign value to buffer
    ibutton.reset_search();
    delay(200);
    return;
 }
 
  Serial.print(millis()/1000);
  Serial.print("> ");
  for (byte x = 0; x<8; x++){  
    Serial.print(addr[x],HEX); //print the buffer content in LSB. For MSB: for (int x = 8; x>0; x--) 
     Serial.print(" "); // print a space
  }
 
  //compute crc//
  byte crc;
  crc = ibutton.crc8(addr, 7);
  Serial.print("CRC: ");
  Serial.println(crc,HEX);
  if ( Serial.read() == 'w' ){
    ibutton.skip();
    ibutton.reset();
    ibutton.write(0x33);
    Serial.print("  ID before write:");
    for (byte x=0; x<8; x++){
      Serial.print(' ');
      Serial.print(ibutton.read(), HEX);
    }
    Serial.print('\n');
    Serial.print("  Writing iButton ID:\n    ");
    // Hardcode here your desired ID //
    // 01 D5 9F DC 02 00 00 96
    byte newID[8] = {0x01, 0xD5, 0x9F, 0xDC, 0x02, 0x00, 0x00, 0x96};
    ibutton.skip();
    ibutton.reset();
    ibutton.write(0xD5);
    for (byte x = 0; x<8; x++){
      writeByte(newID[x]);
      Serial.print('*');
    }
    Serial.print('\n');
    ibutton.reset();
  }
} 

int writeByte(byte data){
  int data_bit;
  for(data_bit=0; data_bit<8; data_bit++){
    if (data & 1){
      pinMode(8, OUTPUT); digitalWrite(8, LOW);
      delayMicroseconds(60);
      pinMode(8, INPUT); digitalWrite(8, HIGH);
      delay(10);
    } else {
      pinMode(8, OUTPUT); digitalWrite(8, LOW);
      pinMode(8, INPUT); digitalWrite(8, HIGH);
      delay(10);
    }
    data = data >> 1;
  }
  return 0;
}
