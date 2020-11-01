#include <EEPROMWearLevel.h>

#define AMOUNT_OF_INDEXES 2

#define INDEX_VAL1 0
#define INDEX_VAL2 1

void setup() {
  Serial.begin(9600);
  while (!Serial);

  EEPROMwl.begin(AMOUNT_OF_INDEXES);
  // Here 512 is length of the EEPROM used for wear leveling in case you want
  // to use parts of the EEPROM for other purpose.
  //EEPROMwl.begin(AMOUNT_OF_INDEXES, 512);

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