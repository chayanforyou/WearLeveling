# WearLeveling

The technique I am useing is to prefix the data with a 4-byte rolling sequence number where the largest number represents the lastest / current value.
In the case of storing 2 bytes of actual data that would give 6 (4-for sequence & 2-for data) bytes total and then I form into a circular queue arrangement so for 1024 bytes of EEPROM (if your EEPROM size is small or max you have to change the value from library file) it would contain 170 entries and increase endurance 170 times if you use one Segment.

Then when booting the largest sequence number can be used to determine both the next sequence number to be used and the current tail of the queue.
The following C-code demonstrates, this assumes that upon initial programming the EEPROM area has been erased to values of 0xFF so I ignore a sequence number of 0xFFFFFFFF (as 4-byte sequence no):


By default the `MAX_EEPROM_SEGMENTS` define is set to 2 this means you have two segments available, 0 or 1.
This allows you to define an area of EEPROM for one structure and a secondary area of EEPROM for another.
If you need more segments you have to change it.

## How it works:

![](https://github.com/user-attachments/assets/6f8c3c7d-0bb1-457e-8718-5b965cd19dcd)

The image illustrates a data storage optimization technique designed for EEPROM usage. This method involves:

- Prefixing each data entry with a 4-byte rolling sequence number.
- The highest sequence number represents the most recent or current value.
- Each entry in memory includes:
  - `4 bytes` for the sequence/step number (shown in yellow),
  - `2 bytes` for the actual data (shown in green),
  - Totaling `6 bytes` per entry.

In the visual:

- Memory is organized sequentially with each block marked by a 4-byte step followed by 2-byte data.
- Arrows show the flow of sequence numbers: `01`, `02`, `03`, etc.
- `FF FF FF FF FF FF` marks unused or erased EEPROM space.

This structure is used in a circular queue format. For example, in a 1024-byte EEPROM:

- With each entry using 6 bytes, you can store up to `170 entries` (`1024 / 6 ≈ 170`).
- Once the EEPROM is full, it wraps around and overwrites the oldest entries.


![](https://github.com/user-attachments/assets/48115dd2-dae1-41c3-b67c-da3fd96af804)

This screenshot shows a real EEPROM memory dump using an EEPROM viewer tool (Proteus debugger). It beautifully demonstrates how data is stored using the rolling sequence number technique.

Key Observations:

- The highlighted section shows three entries
- Each entry is 6 bytes (4 for the step, 2 for data), as per the technique.
- All other memory locations are still filled with `FF`, meaning they are unwritten/erased.
- This clearly shows that only 3 writes have occurred so far in this segment.
- The viewer also shows an ASCII representation on the right (which is mostly dots since these values don’t correspond to readable characters).

## How to use:

I designed this library to address a couple of issues when using EEPROM for data storage:

Data corruption is one of this big issues with EEPROM.  From what I've read, the most common reason is a power issue where power drops out and/or brown out detection is not properly configured.
The big problem with data corruption is that when the data is read back from the EEPROM, it may be corrupt and the uC will use corrupt data with can lead to all sorts of problems.
The second issue is that the EEPROM design limit of 100,000 write cycles. If you are trying to save values periodically, this will be a concern if you are writing the data to the same spot of EEPROM each time.
The Library solves both of these problems by using these techniques:


## It does all this with three functions:
```c
EEPROMwl.begin(const uint8_t amountOfIndexes);
// or
EEPROMwl.begin(const uint8_t amountOfIndexes, const uint16_t eepromLengthToUse);
EEPROMwl.read(const uint8_t idx)
EEPROMwl.write(const uint8_t idx, const uint16_t value);
```
The first IMPORTANT thing I want to mention is the ORDER these commands must be used.

You must call `EEPROMwl.begin()` first. After that you can call `EEPROMwl.read()` & `EEPROMwl.write()` as many times as you like.
The reason for this order is that Save requires variables that Load set to work properly.
This shouldn't be a problem because the general flow of usage for these commands should follow this order:

### Arduino example:
```c
#include <EEPROMWearLevel.h>

#define AMOUNT_OF_INDEXES 2

#define INDEX_VAL1 0
#define INDEX_VAL2 1

void setup() {
  Serial.begin(9600);
  while (!Serial);

  EEPROMwl.begin(AMOUNT_OF_INDEXES);

  writeConfiguration();
  readConfiguration();
}

void loop() {
}

void writeConfiguration() {
  // write value
  EEPROMwl.write(INDEX_VAL1, 123);
  EEPROMwl.write(INDEX_VAL2, 32768);
}

void readConfiguration() {
  // read value
  Serial.print(F("INDEX 1: "));
  Serial.println(EEPROMwl.read(INDEX_VAL1));

  Serial.print(F("INDEX 2: "));
  Serial.println(EEPROMwl.read(INDEX_VAL2));
}
```
Make sure you don't just call `EEPROMwl.write()` to save the `same data` as this is wasteful on EEPROM design life.
Keep a flag that indicates when the data has changed and also a flag indicating how long it has been since that last save.
Only save if the data has changed AND it has been a long enough period of time.

## Special thanks ##
To [Antor Ahmed](https://github.com/AntorOfficial)

## Contributions ##
Enhancements and improvements are welcome.

## License ##
```
Arduino EEPROMWearLevel Library
Copyright (c) 2020 Chayan Mistry (https://github.com/chayanforyou).

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
