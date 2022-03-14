
Remote Contact Extender


Part: RCE-1

Date: 5/4/2002

Author: Anthony Kempka

------------------------

Initial client: ????


Assumptions
------------
Expander attached to DIG (invert data)
Power on default = OFF
Watchdog - If no communication within 1 SECOND, turn all outputs off.
Master sends - Slave responds
Any unit that has odd numbered address, is a Master.
Master units send continuously when DIP position 1 is ON.
Clock rate is 14.7456 Mhz


Warnings
---------
9600 BAUD is not supported


Master Unit (Module Address = 1)
--------------------------------
Even bits inputs
Odd bits outputs
(EEPROM settings Loc 0x00:0x01, Loc 0x01:0x02)
Aync Destination address = 2


Slave Unit (Module Address = 2)
-------------------------------
Even bits outputs
Odd bits inputs
(EEPROM settings Loc 0x00:0x02, Loc 0x02:N/A but set to 0x01)
Async Desitination address = N/A


DIP Switch
-----------
Positions 3/4 = Baud Rate. 115200, 57600 and 19200 only supported.
Position 1 = ON: Master Start tranmitting (Master unit only)


Commands:
----------
V - Version
W - Write EEPROM
R - Read EEPROM
Z - Reset CPU
K - Get receive error count
J - Clear receive error count
S - Master sends Sxxyy (Master unit current inputs xx=PORT1, yy=PORT2)
    Slave updates with new Port1/2 values from Master
    Salve responds Sxxyy (Slave unit current inputs xx=PORT1, yy=PORT2)
    Master updates with new Port1/2 values from Slave response
    If no communication within 1 second, both units go to default (all outputs OFF)

