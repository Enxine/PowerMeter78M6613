/*
* PowerMeter78M6613.h 
* Version 1.0 October, 2015
* Copyright 2015 Enxine Dev S.C.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
* The latest version of this library can always be found at
*
* http://github.com/Enxine/PowerMeter78M6613
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#ifndef _PowerMeter78M6613_h
#define _PowerMeter78M6613_h

#include "Arduino.h"
#include <SoftwareSerial.h>

#define CR 13

class PowerMeter78M6613 {

  public:
    PowerMeter78M6613(int TxPin, int RxPin);
	//Start and initialize serial interface
    void startSerialInterface();
	//Close serial interface
    void stopSerialInterface();
	//Perform hard reset of the 78M6612
	void reset();
	//Read data value. See examples below
	long readData(char *cmd);
	long readData(const char *cmd) { return readData((char *)cmd); };
	long readData(const __FlashStringHelper *cmd); 
	//Get line power usage in mW
	long readPower() { return readData(F(")27$")); };
	//Get frequency of the line in Hz*100
	long readLineFrequency() { return readData(F(")21$")); };
	//Get line voltage in mVrms 
	long readVoltage() { return readData(F(")26$")); };
	//Get acumulated energy usage in mWh
	long readEnergy() { return readData(F(")28$")); };
	//Get line current in mArms 
	long readCurrent() { return readData(F(")2A$")); };
	//Get line reactive power per second in VARs * 1000 
	long readReactivePower() { return readData(F(")2B$")); };
	//Get line apparent power per second in VAs * 1000 
	long readApparentPower() { return readData(F(")2C$")); };
	//Get line power factor * 1000 
	long readPowerFactor() { return readData(F(")2D$")); };
	//Get line phase angle in degrees * 1000
	long readPhaseAngle() { return readData(F(")2E$")); };
	//Under development. Chip always return 0 for minimum and maximum voltage values. We must keep investigating about this.
	//Get minimum voltage measured in the line since last reset in mVrms 
	//long readMinimumVoltage() { return readData(")30$"); };
	//Get maximum voltage measured in the line since last reset in mVrms 
	//long readMaximumVoltage() { return readData(")31$"); };
    //There are more measurements available from the 78M6613 chip. Please review full documentation for reference
  private:
    static bool serialStarted;
    static SoftwareSerial *serial78M6613;
    void sendCommand(char*);
    long readRegister(char*);
    byte readData(char*, int);
    char getValueHexChar(char c);
    byte getHexDataIndex(char* buf);
    boolean isValidStringHexData(char* buf);
    long decodeStringHexData(char* buf);

};
#endif //_PowerMeter78M6613_h
