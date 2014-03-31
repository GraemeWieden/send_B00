// our own protocol is called B00 as it says Boo! to announce itself 
// the last byte (00) may eventually be used as a content descriptor
// by default, the descriptor is 0 indicating the following packet payload:
// 12 bits: B00
// 4 bits: 2 bit house code - 2 bit channel code
// 16 bits: integer value 1
// 16 bits: integer value 2
// total: 48 bits
// other content descriptors may be introduced in future for other packet payload needs  

// note that arduino has a max of 16383 for the delayMicroseconds function
int TriggerPulse = 15000; // trigger time in microseconds
int LongPulse = 1000; // time in microseconds for a long pulse
int ShortPulse = LongPulse / 3; // time in microseconds for a short pulse

int txPort = 9; // digital pin for transmitter
int ledPort = 13; // digital pin for LED

int testA = 0;
int testB = 0;

void setup()
{
}

void loop()
{
  // send: content, house, channel, value A, value B
  sendB00Packet(0, 1, 2, testA, testB);
  testA++;
  testB += 2;
  
  delay(2000);
  // blink led
  digitalWrite(ledPort, HIGH);
  delay(50);   
  digitalWrite(ledPort, LOW);
  // wait a bit longer before next transmission
  delay(2000);
}

void sendB00Packet(int content, int house, int channel, int valueA, int valueB)
{
  int repeats = 2;
  
  sendB00Trigger();
  for(int i = 0; i < repeats; i++)
  {
    sendB00Bits(0xB, 4); // announce!
    sendB00Bits(content, 8); // default 00
    sendB00Bits(house, 2);
    sendB00Bits(channel, 2);
    sendB00Bits(valueA, 16);
    sendB00Bits(valueB, 16);
    sendB00Trigger();
  }
}

void sendB00Trigger()
{
  digitalWrite(txPort, HIGH);
  delayMicroseconds(TriggerPulse);   
  digitalWrite(txPort, LOW);
  delayMicroseconds(TriggerPulse);    
}

void sendB00Bits(unsigned int data, int bits)
{
  unsigned int bitMask = 1;
  bitMask = bitMask << (bits - 1);
  for(int i = 0; i < bits; i++)
  {
    sendB00Bit( (data&bitMask) == 0 ? 0 : 1);
    bitMask = bitMask >> 1;
  }
}

void sendB00Bit(byte b)
{
  if(b == 0)
  {
    digitalWrite(txPort, HIGH);
    delayMicroseconds(ShortPulse);
    digitalWrite(txPort, LOW);
    delayMicroseconds(LongPulse);  
  }
  else // any bit in the byte is on
  {
    digitalWrite(txPort, HIGH);
    delayMicroseconds(LongPulse);
    digitalWrite(txPort, LOW);
    delayMicroseconds(ShortPulse);  
  }
}



