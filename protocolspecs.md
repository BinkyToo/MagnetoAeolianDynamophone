Specifications- for broadcast commands to note-playing slave devices
===

Electrical
---
* The entire device has a common 0V rail shared by all logic and processing devices. It is not, and must not be connected to any part of the 12V power system
* Inter-module signalling is achieved at 9600 baud (8N1) at 5V logic levels, relative to the system logic ground
* Modules must have an RX pin for recieving commands
* A RX pin may be useful for debugging, but is entirely optional and not used by the rest of the system


General signalling notes
---
* Timing is managed by the master module - slave devices should atempt to start playing a note as soon as possible
* Many packets will be recieved that are addressed to to other modules. These must be gracefully ignored

Command packet structure
---
* The first part is an address. This should be a single word describing the hardware controlled by the slave module. All lower case, all one word.
* Immediately after this is a colon
* Next a string indicating the note or tone to be played. Ideally very short, preferably just a single character.
* Then a comma
* Next an optional duration expressed as an ascii digit (In future possibly any decimal number). Devices that do not require a duration must accept one, those that require one should default to a non-zero duration
* Finally a newline character
 
Examples
---
stepper:B,3
percussion:*,
