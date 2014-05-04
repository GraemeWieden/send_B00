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

#define DEBUG

// note that arduino has a max of 16383 for the delayMicroseconds function
int TriggerPulse = 15000; // trigger time in microseconds
int LongPulse = 1000; // time in microseconds for a long pulse
int ShortPulse = LongPulse / 3; // time in microseconds for a short pulse
boolean parity; // parity bit is tracked as bits are sent

int txPort = 9; // digital pin for transmitter
int ledPort = 13; // digital pin for LED

byte house = 1;  // default house code
byte channel = 1;  // default channel code

void setup()
{
  Serial.begin(9600);
  pinMode(ledPort, OUTPUT);   
}

void loop()
{
  double testDouble = 123456.789;
  sendB00(house, channel, testDouble);
  blinkAndPause();
  
  testDouble = -9876.54321;
  sendB00(house, channel, testDouble);
  blinkAndPause();
  
  long testLong = 123456789;
  sendB01(house, channel, testLong);
  blinkAndPause();
  
  testLong = -987654321;
  sendB01(house, channel, testLong);
  blinkAndPause();
  
  unsigned long testUnsignedLong = 1234567890;
  sendB02(house, channel, testUnsignedLong);
  blinkAndPause();
  
  int testA = -20;
  int testB = 20;
  sendB03(house, channel, testA, testB);
  blinkAndPause();

  unsigned int testC = 0;
  unsigned int testD = 65530;  
  sendB04(house, channel, testC, testD++);
  blinkAndPause();
  
  sendB05(house, channel, B10101010, B11001100, B11110000, B11111111);
  blinkAndPause();  
  
  delay(10000);
}

void blinkAndPause()
{
  // blink LED
  digitalWrite(ledPort, HIGH);
  delay(50);   
  digitalWrite(ledPort, LOW);
  delay(1000);
}
void sendB00(byte house, byte channel, double value)
{
#ifdef DEBUG
  Serial.println("B00 : double");
#endif

  sendB00Packet(0, house, channel, value);
}

void sendB01(byte house, byte channel, long value)
{
#ifdef DEBUG
  Serial.println("B01 : long");
#endif

  sendB00Packet(1, house, channel, value);
}

void sendB02(byte house, byte channel, unsigned long value)
{
#ifdef DEBUG
  Serial.println("B02 : unsigned long");
#endif

  sendB00Packet(2, house, channel, value);
}

void sendB03(byte house, byte channel, int valueA, int valueB)
{
#ifdef DEBUG
  Serial.println("B03 : int int");
#endif

  unsigned long value = valueA;
  value = value << 16 | valueB;
  sendB00Packet(3, house, channel, value);
}

void sendB04(byte house, byte channel, unsigned int valueA, unsigned int valueB)
{
#ifdef DEBUG
  Serial.println("B04 : unsigned int unsigned int");
#endif

  unsigned long value = valueA;
  value = value << 16 | valueB;
  sendB00Packet(4, house, channel, value);
}

void sendB05(byte house, byte channel, byte b1, byte b2, byte b3, byte b4)
{
#ifdef DEBUG
  Serial.println("B05 : byte byte byte byte");
#endif

  unsigned long value = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
  sendB00Packet(5, house, channel, value);
}

void sendB00Packet(byte content, byte house, byte channel, unsigned long value)
{
  int repeats = 3;
  #ifdef DEBUG
    repeats = 1;
  #endif
  
  sendB00Trigger();

  for(int i = 0; i < repeats; i++)
  {
    // parity = 0; // don't need to do this as parity bit is reset to zero by writing the parity bit
    sendB00Bits(0xB, 4); // announce!
    sendB00Bits(content, 8); // default 00
    sendB00Bits(house, 2);
    sendB00Bits(channel, 2);
    sendB00Bits(value, 32);
    sendB00Bits(parity, 1);
    sendB00Trigger();
  }
#ifdef DEBUG
  Serial.println();
#endif
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
  // bits must be no more than 32
  unsigned long bitMask = 1;
  bitMask = bitMask << (bits - 1);
  for(int i = 0; i < bits; i++)
  {
#ifdef DEBUG
    debugB00Bit( (data&bitMask) == 0 ? 0 : 1);
#else
    sendB00Bit( (data&bitMask) == 0 ? 0 : 1);
#endif

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
  {
    Serial.print("1");
    parity = !parity;
  }
}



