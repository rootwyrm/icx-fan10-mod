# ICX-FAN10-MOD

This project is for the ICX-FAN10-MOD controller, a (mostly) drop-in replacement to make your homelab ICX switch using the FAN10 module a whole lot quieter.

NOTE: this project is very much in development and it is NOT recommended that you order PCBs or components at this time.

## Key Features
- Works with all switches that use the FAN10 module
- Works *cooperatively* with the switch instead of simply lying to it
- Works with nearly all 4-pin fans (40/60/80/120/140/200) 
- Plugs directly into the FAN10 module headers (minimum 1, but 2 is usually better)
- Provides a TTL 3-wire serial interface for monitoring and debugging
- Provides simple LED monitoring for power rails and general fault status
- (Optionally) takes an ESP01-S module
- (Optionally) takes a Teensy Ethernet module
- (Optionally) takes both at once (PSRAM expansion required)
- (Optionally) provides a web-accessible interface for monitoring
- (Optionally) provides a web-accessible interface for your ICX serial console (already RS232, just add cable!)

## Coming Mechanical Features
- Pluggable enclosure that slides into the FAN10 module slot 

## (Hopefully) Coming Features
- Current sensing of modification fans
- Independent temperature sense and control ("don't lie to me" mode.)
- Expanded GPIO
- Software sucking less
- More compact and power efficient components on the PCB
- Interface with PSU control to reduce fan speeds
- Interface with switch JTAG for debugging and recovery
- Premade top-plates for 1x140 and 2x140 fan configurations (maybe)

## What it DOESN'T or WON'T do
- Does NOT require any irreversible modifications. Plugs right in, and comes right back out. No cutting is required (or recommended!)
- Does NOT require any software modifications to the switch. It's designed to cooperate with existing software.
- Does NOT connect to PSU fans. PSUs contain hazardous voltages. You are not a professional. Stay out of them.