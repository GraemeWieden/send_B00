B00 Test Packet sender utility
==============================

Testing and evaluation sketch for Arduino
-----------------------------------------

We have defined our own protocol for sending sensor data via RF between Arduinos. The protocol is called B00 as it says Boo! to announce itself.

The packet is a total of 49 bits made up of:

- 12 bits: B00 to announce and content descriptor
- 4 bits: 2 bit house code 2 bit channel code
- 16 bits: integer value 1
- 16 bits: integer value 2
- 1 bit: even parity

Other content descriptors may be introduced in future to indicate other payload content within the packet. This can be handled by incrementing from B00 to B01 B02 etc. For the moment, we just have two 16 bit integers. That allows from 0 to 65535.

The RF encoding uses a reasonably 'standard' approach where zero is represented as short high - long low and one is represented as long high - short low.

The short pulse is one quarter of the bit duration and the long pulse is three quarters.

Timings are technically user definable as they can be determined from the trigger pulse duration with the long pulse being one 15th of the trigger and the short pulse being one third of the long pulse.

This evaluation sketch is using default values of:

- 15 ms trigger
- 1 ms long pulse
- 0.33 ms short pulse
