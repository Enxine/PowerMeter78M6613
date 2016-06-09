/*
* PowerMeter.ino 
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

/* DISCLAIMER:
 * This is a use example of the PowerMeter78M6613 library to use 78M6613
 * chips through serial port from Arduino. 78M6613 is an AC line power meter,
 * and AC line electricity is dangerous. AC can damage you or your equipment
 * if not proper handling precautions are taken. Never touch electric boards
 * while connected to AC line, always disconnect them from AC before handling. 
 * AC connected boards are also dangerous for your computer and connecting at 
 * the same time USB connector and AC line to a board can damage your USB port 
 * or even blow up the whole computer. So the idea of logging AC line values 
 * through serial port to the Arduino IDE is a REALLY BAD IDEA. Never connect
 * your board to the USB connector while pluged to the mains. This example is
 * just provided as a simple reference on how to use the library for more 
 * complex projects, where, for example, Arduino logs AC values through 
 * wireless so there is no physical connection. Please do not use this example
 * as is, or use it at your own risk. 
 */
 
#include <SoftwareSerial.h> 
#include <PowerMeter78M6613.h>

//Power Meter 78M6613 serial connection pins
#define PM78M6613_TX_PIN 6
#define PM78M6613_RX_PIN 7

PowerMeter78M6613 myPowerMeter = PowerMeter78M6613(PM78M6613_TX_PIN, PM78M6613_RX_PIN);

void setup() 
{
  Serial.begin(9600);
  myPowerMeter.startSerialInterface();
  //myPowerMeter.reset();//Uncoment this line to reset device each time arduino is reset, but I prefer 78M6613 to keep monitoring data unless I unplug the board
}

void loop()
{
  Serial.println();
  Serial.println("---------"); 
  Serial.println();

  Serial.print("Voltage: "); 
  Serial.print(String(float(myPowerMeter.readVoltage()/(float)1000)));
  Serial.println(" Vrms");

  Serial.print("Current: "); 
  Serial.print(String(float(myPowerMeter.readCurrent()/(float)1000),4));
  Serial.println(" Arms");

  Serial.print("Frequency: "); 
  Serial.print(String(float(myPowerMeter.readLineFrequency()/(float)100)));
  Serial.println(" Hz");

  Serial.print("Power: ");
  Serial.print(String(float(myPowerMeter.readPower()/(float)1000)));
  Serial.println(" W");

  Serial.print("Acumulated energy: ");
  Serial.print(String(float(myPowerMeter.readEnergy()/(float)1000)));
  Serial.println(" Wh");
  
  Serial.print("Reactive power per second: ");
  Serial.print(String(float(myPowerMeter.readReactivePower()/(float)1000)));
  Serial.println(" VARs");
  
  Serial.print("Apparent power per second: ");
  Serial.print(String(float(myPowerMeter.readApparentPower()/(float)1000)));
  Serial.println(" VAs");

  Serial.print("Power factor: ");
  Serial.println(String(float(myPowerMeter.readPowerFactor()/(float)1000),4));

  Serial.print("Phase angle: ");
  Serial.print(String(float(myPowerMeter.readPhaseAngle()/(float)1000)));
  Serial.println(" deg");

  delay(1000);
}