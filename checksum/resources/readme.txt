1. Checksum module

The module has methods for the popular checksumming algorithms. The module need to serve both on the fly and buffer calculating methods too.
The must have algorithms:
- none no checksum, but the empty functions exist.
- checksum all bytes added to an aggregation, and the 0-aggregation will be the checksum.
- crc16 16 bit crc
- crc modbus 16 bit crc for modbus

The differences between the last two crc  are, the shift direction, and xor value.

Module have to serve
- on the fly mode, which means aggregate one byte and
- block mode which means calculating a checksum value of a given buffer.



http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code

CRC calculation:

http://depa.usst.edu.cn/chenjq/www2/SDesign/JavaScript/CRCcalculation.htm