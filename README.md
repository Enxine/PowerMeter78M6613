# PowerMeter78M6613 Arduino Library
This library enables the use of Maxim's 78M6613 AC power meter connected thorough serial port
to an Arduino. 

Check [here](https://www.maximintegrated.com/en/products/industries/metering-energy-measurement/78M6613.html) 
for more information about the 78M6613 chip.

Check [here](http://github.com/Enxine/PowerMeter78M6613) for tutorials and more information 
about this library.

##DISCLAIMER

78M6613 is an AC line power meter, and AC line electricity is dangerous. AC can damage you or your
equipment if not proper handling precautions are taken. Never touch electric boards while connected
to AC line, always disconnect them from AC before handling. AC connected boards are also dangerous 
for your computer and connecting at the same time USB connector and AC line to a board can damage 
your USB port or even blow up the whole computer. Please be very careful while using AC electricity
and consider the advice that you are using this software at your own risk. 

## Version - 1.00

## Installation
1. Navigate to the [Releases](http://github.com/Enxine/PowerMeter78M6613) page.
2. Download the latest release.
3. Extract the zip file
4. Move the "PowerMeter78M6613" folder that has been extracted to your libraries directory.

## Usage

PowerMeter78M6613 library uses internally the SoftwareSerial library, since there are no strong
real time requirements for the serial communications and this allows a flexible set up to connect
the serial por to any pin of the Arduino. But remember to include SoftwareSerial library in your
sketch. In addition to the PowerMeter78M6613 library itself, of course:

```#include <SoftwareSerial.h> 
#include <PowerMeter78M6613.h>
```

The library is very easy to use. Once installed in your Arduino development environment you
just need to create an instance of the PowerMeter78M6613 object passing as parameters the pins
of the Arduino connected to the serial port of the 78M6613:

```PowerMeter78M6613 myPowerMeter = PowerMeter78M6613(TX_PIN, RX_PIN);```

Once you have the PowerMeter78M6613 object created you need to initialize the module in the
Arduino's setup code section. It only needs the serial interface to be launched:

```void setup() {  
     myPowerMeter.startSerialInterface();    
  }```

Then you can freely use the command readPower to request the current power metering value at
any time in your loop section.    

```power_mW = myPowerMeter.readPower();``` 

There are more avilable data, like line current, voltage, apparent or reactive power, etc. Please
refer to the examples or to the library code for a complete reference of useable primitives.
   
Just rememeber always PowerMeter78M6613 library uses internally SoftwareSerial connection, 
with its own limitations. If you are using other SoftwareSerial instances in your project not all
them can communicate simultaneously. Turn off PowerMeter78M6613 serial communications before
using other software serial port.

```void loop() { 
     myPowerMeter.startSerialInterface();
	 power_mW = myPowerMeter.readPower();
	 myPowerMeter.stopSerialInterface();
	 /* Use other software serial port stuff */
  }```

See included code examples for more details on how to use the library
 
## Contributing
If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

## Copyright
Copyright 2015 Enxine Dev S.C.