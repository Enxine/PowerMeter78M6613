/*
* PowerMeter78M6613.cpp 
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

#include "PowerMeter78M6613.h"

//Uncoment lines below to see debug lines through hardware serial port
//#define _DEBUG
//#define _DEBUG_2

bool PowerMeter78M6613::serialStarted = false;

SoftwareSerial *PowerMeter78M6613::serial78M6613 = NULL;

PowerMeter78M6613::PowerMeter78M6613(int TxPin, int RxPin) {
  this->serialStarted = false;
  this->serial78M6613 = new SoftwareSerial(TxPin, RxPin);
}

void PowerMeter78M6613::startSerialInterface() {
  serial78M6613->begin(4800);
  //while (!serial78M6613) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
  delay(50);
  serial78M6613->write(CR);
  serial78M6613->flush();
  while (serial78M6613->available() > 0) {
    serial78M6613->read();
  }
  this->serialStarted = true;
}

void PowerMeter78M6613::stopSerialInterface() {
  serial78M6613->flush();
  while (serial78M6613->available() > 0) {
    serial78M6613->read();
  }
  delay(50);
  this->serialStarted = false;
  serial78M6613->end();
}

void PowerMeter78M6613::sendCommand(char* cmd) {
  byte lineLength = strlen(cmd)+2;
  char line[lineLength];
  for (byte i=0; i<strlen(cmd); i++) {
    line[i] = cmd[i];
  }
  line[lineLength-2] = CR;
  line[lineLength-1] = '\0';

  serial78M6613->write(line);
  serial78M6613->flush();
  delay(50);
}

byte PowerMeter78M6613::readData(char* buf, int bufLength) {
#ifdef _DEBUG
    Serial.println(F("---readData"));
#endif    
  serial78M6613->listen();
  unsigned long startTime = millis();
  unsigned long timeoutTime = 1000;
  while (serial78M6613->available() <= 0) {
    delay(50);
    unsigned long nowTime = millis();
    if ((nowTime - startTime) > timeoutTime) break;
  }
  byte i = 0;
  while (serial78M6613->available() > 0) {
    buf[i++] = serial78M6613->read();
  }
  buf[i] = '\0';
#ifdef _DEBUG
  Serial.print(F("bytes read "));
  Serial.println(i);
  Serial.println(buf);
  Serial.println(F("------"));
#endif  
  return i;
}

char PowerMeter78M6613::getValueHexChar(char c) {
  char value = -1;
  if ((c >= '0') && (c <= '9')) {
    value = c - '0';
  } else if ((c >= 'A') && (c <= 'F')) {
    value = c - 'A' + 10;
  }
  return value;
}

byte PowerMeter78M6613::getHexDataIndex(char* buf) {
  byte index = 0;
  byte length = strlen(buf);
  while (this->getValueHexChar(buf[index]) < 0) {
    index++;
  }
  if (index+7 < length) {
    if (this->getValueHexChar(buf[index+7]) < 0) {
      index = -1;
    }
  } else {
   index = -1; 
  }
  return index;
}

boolean PowerMeter78M6613::isValidStringHexData(char* buf) {
#ifdef _DEBUG
  Serial.println(F("---isValidStringHexData"));
#endif    
  boolean valid = true;
  if (strlen(buf) != 14) return false;
  byte offset = 2;
  for (int i = 0; i < 8; i++) {
    char val = getValueHexChar(buf[offset+i]);
    if (val < 0) {
      valid = false;
    }
  }
#ifdef _DEBUG
  Serial.println(valid);
#endif
  return valid;
}

long PowerMeter78M6613::decodeStringHexData(char* buf) {
#ifdef _DEBUG
  Serial.println(F("---decodeStringHexData"));
#endif
  unsigned long value = 0;
  if (strlen(buf) != 14) {
#ifdef _DEBUG 
    Serial.println(F("ERROR: 'buf' must be 14 characters"));
#endif
    return 0;
  }
  byte offset = 2;
#ifdef _DEBUG
  Serial.print(F("offset "));
  Serial.println(offset);
#endif
  for (int i = 0; i < 8; i++) {
    value = value << 4;
    char val = getValueHexChar(buf[offset+i]);
#ifdef _DEBUG_2
    Serial.print(F("i "));
    Serial.print(i);
    Serial.print(F(" | val "));
    Serial.println((byte)val);
#endif
    if (val >= 0) {
      value += val;
    } else {
      value = 0;
      break;
    }
  }
  long v = *(long *)&value;
#ifdef _DEBUG
    Serial.print(F("value "));
    Serial.println(v);
    Serial.println(F("------"));
#endif    
  return v;
}

long PowerMeter78M6613::readRegister(char* cmd) {
  boolean valid = false;
  byte bytes = -1;
  int bufLength = 64;
  char buf[bufLength];
  
  do {
    this->sendCommand(cmd);
    bytes = this->readData(buf, bufLength);
    
    valid = this->isValidStringHexData(buf);
    
  } while ((bytes != 14) || !valid);
  long value = 0;
  value = this->decodeStringHexData(buf);
#ifdef _DEBUG
    Serial.print(F("---ReadRegister "));
    Serial.println(value);
#endif
  return value;
}

void PowerMeter78M6613::reset() {
  char cmd[] = "W";
  this->sendCommand(cmd);
}

long PowerMeter78M6613::readData(char *cmd) {
  unsigned long value;
  value = this->readRegister(cmd);
  return value;
}

long PowerMeter78M6613::readData(const __FlashStringHelper *cmd) {
  PGM_P p = reinterpret_cast<PGM_P>(cmd);
  char command[20];
  unsigned char i=0;
  do {
	command[i++] = pgm_read_byte_near(p++);
  } while(command[i-1]!='\0');
  unsigned long value;
  value = this->readRegister(command);
  return value;
}
