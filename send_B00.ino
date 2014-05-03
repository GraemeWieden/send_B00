// The B00 RF protocol is called B00 as it says Boo! to announce itself 
// 
// by default, the descriptor is 0 indicating the following packet payload:
// 12 bits: B00 - the last byte is the content descriptor
// 4 bits: 2 bit house code - 2 bit channel code
// 32 bits: content - see below
// 1 bit: even parity bit
// total: 49 bits

// Content descriptor currently defines these content types:
// B00: 
// 32 bits: double precision floating point value
// B01: 
// 32 bits: long signed integer value
// B02: 
// 32 bits: long unsigned integer value
// B03: 
// 16 bits: signed integer value 1
// 16 bits: signed integer value 2
// B04: 
// 16 bits: unsigned integer value 1
// 16 bits: unsigned integer value 2
// B05: 
// 8 bits: byte value 1
// 8 bits: byte value 2
// 8 bits: byte value 3
// 8 bits: byte value 4

// note that arduino has a max of 16383 for the delayMicroseconds function
int TriggerPulse = 15000; // trigger time in microseconds
int LongPulse = 1000; // time in microseconds for a long pulse
int ShortPulse = LongPulse / 3; // time in microseconds for a short pulse
boolean parity; // parity bit is tracked as bits are sent

int txPort = 9; // digital pin for transmitter
int ledPort = 13; // digital pin for LED

int testA = 0;
int testB = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(ledPort, OUTPUT);   
}

void loop()
{
  // send: content, house, channel, value A, value B
  sendB00(1, 2, testA, testB);
  
  testA++;
  testB += 2;
  
  blinkLed();

  // wait for 5 seconds before next transmission
  delay(5000);
}
void blinkLed()
{
  digitalWrite(ledPort, HIGH);
  delay(50);   
  digitalWrite(ledPort, LOW);
}
void sendB00(byte house, byte channel, double value)
{
  sendB00Packet(0, house, channel, value);
}

void sendB0O(byte house, byte channel, long value)
{
  sendB00Packet(1, house, channel, value);
}

void sendB00(byte house, byte channel, unsigned long value)
{
  sendB00Packet(2, house, channel, value);
}

void sendB00(byte house, byte channel, int valueA, int valueB)
{
  unsigned long value = (valueA << 16) & valueB;
  sendB00Packet(3, house, channel, value);
}

void sendB00(byte house, byte channel, unsigned int valueA, unsigned int valueB)
{
  unsigned long value = (valueA << 16) & valueB;
  sendB00Packet(4, house, channel, value);
}

void sendB00(byte house, byte channel, byte b1, byte b2, byte b3, byte b4)
{
  unsigned long value = (b1 << 24) & (b2 << 16) & (b3 << 8) & b4;
  sendB00Packet(5, house, channel, value);
}

void sendB00Packet(byte content, byte house, byte channel, unsigned long value)
{
  int repeats = 2;

  sendB00Trigger();
  // Serial.println();
      
  for(int i = 0; i < repeats; i++)
  {
    boolean parity = 0;
    sendB00Bits(0xB, 4); // announce!
    sendB00Bits(content, 8); // default 00
    sendB00Bits(house, 2);
    sendB00Bits(channel, 2);
    sendB00Bits(value, 32);
    sendB00Bits(parity, 1);
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

void sendB00Bits(unsigned long data, unsigned int bits)
{
  unsigned int bitMask = 1;
  bitMask = bitMask << (bits - 1);
  for(int i = 0; i < bits; i++)
  {
    sendB00Bit( (data&bitMask) == 0 ? 0 : 1);
    //debugB00Bit( (data&bitMask) == 0 ? 0 : 1);
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
    parity = !parity;
    digitalWrite(txPort, HIGH);
    delayMicroseconds(LongPulse);
    digitalWrite(txPort, LOW);
    delayMicroseconds(ShortPulse);  
  }
}

void debugB00Bit(byte b)
{
  if(b == 0)
    Serial.print("0");
  else
    Serial.print("1");
}


