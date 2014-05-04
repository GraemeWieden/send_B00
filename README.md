B00 Test Packet sender utility
==============================

Testing and evaluation sketch for Arduino
-----------------------------------------

We have defined our own protocol for sending sensor data via RF between Arduinos. The protocol is called B00 as it says Boo! to announce itself.

The packet is a total of 50 bits made up of:

- 12 bits: B00 to announce and content descriptor (see below)
- 5 bits: 2 bit house code 3 bit channel code
- 32 bits: content
- 1 bit: even parity

Content Descriptor
------------------
The content descriptor currently defines these content types:
B00: 
  32 bits: double precision floating point value
B01: 
  32 bits: signed long integer value
B02: 
  32 bits: unsigned long integer value
B03: 
  16 bits: signed integer value 1
  16 bits: signed integer value 2
B04: 
  16 bits: unsigned integer value 1
  16 bits: unsigned integer value 2
B05: 
  8 bits: byte value 1
  8 bits: byte value 2
  8 bits: byte value 3
  8 bits: byte value 4
  

The RF encoding scheme used by the B00 protocol is a reasonably 'standard' typpe where zero is represented as short high - long low and one is represented as long high - short low. 

The short pulse is one quarter of the bit duration and the long pulse is three quarters. The actual pulse durations are derived from the trigger pulse length which is 10 milliseconds. The short pulse is 1/31 of the trigger pulse.

This produces pulse durations of

- 10 ms trigger
- 0.967 ms long pulse
- 0.322 ms short pulse

The B00 RF protocol mirrors the system defined as 'Protocol 1' in the Arduino RCSwitch library, however the base version of the RCSwitch library can't be used as it only supports up to 32 bit packets.

If you intend using RCSwitch to decode the B00 packet, be sure to use the modified version which supports up to 64 bit packets. It can be found here:
https://github.com/GraemeWieden/RCSwitch

For a complete example of how to receive and decode the B00 packet, see the example here:
https://github.com/GraemeWieden/receive_B00


