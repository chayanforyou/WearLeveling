#include <avr/io.h>
#include <avr/eeprom.h>
#include "EEPROMWearLevel.h"

EEPROMWearLevel EEPROMwl;

/**
   Initializes EEPROMWearLevel with full EEPROM size.
   @param amountOfIndexes Number of indexes.
*/
void EEPROMWearLevel::begin(const uint8_t amountOfIndexes) {
  begin(amountOfIndexes, EEPROMClass::length());  // Use full EEPROM size
}

/**
   Initializes EEPROMWearLevel with specified EEPROM size.
   @param amountOfIndexes Number of indexes.
   @param eepromLengthToUse EEPROM size to use.
*/
void EEPROMWearLevel::begin(const uint8_t amountOfIndexes, const uint16_t eepromLengthToUse) {
  queueEntries = new QUEUE[amountOfIndexes];
  lastSequenceNo = new uint32_t[amountOfIndexes]();
  queueTail = new uint8_t[amountOfIndexes]();
  lastValue = new uint16_t[amountOfIndexes]();

  queueEntriesCount = (eepromLengthToUse / QUEUE_SIZE) / amountOfIndexes;

  // Read last written values from EEPROM
  for (uint16_t index = 0; index < amountOfIndexes; index++) {
    getLastSavedValue(index);
  }
}

/**
   Gets queue tail for a given index.
   @param idx Index number.
   @return Queue tail position.
*/
uint8_t EEPROMWearLevel::getQueueTail(const uint8_t idx) {
  return idx * queueEntriesCount;
}

/**
   Retrieves the last saved value from EEPROM.
   @param idx Index number.
*/
void EEPROMWearLevel::getLastSavedValue(const uint8_t idx) {
  uint8_t currentQueueTail = getQueueTail(idx);
  queueTail[idx] = currentQueueTail;
  lastSequenceNo[idx] = 0;
  lastValue[idx] = 0;

  QUEUE tempQueue;

  for (uint16_t index = currentQueueTail; index < (currentQueueTail + queueEntriesCount); index++) {
    uint16_t offset = index * QUEUE_SIZE;

    // Read EEPROM data into temporary struct
    eeprom_read_block((void*)&tempQueue, (const void*)offset, QUEUE_SIZE);

    // Check for valid sequence number
    if (tempQueue.sequenceNo > lastSequenceNo[idx] && tempQueue.sequenceNo != 0xFFFFFFFF) {
      queueTail[idx] = index;
      lastSequenceNo[idx] = tempQueue.sequenceNo;
      lastValue[idx] = tempQueue.data;
    }
  }
}

/**
   Update a value to EEPROM with wear leveling.
   @param idx Index number.
   @param value Value to store.
*/
void EEPROMWearLevel::update(const uint8_t idx, const uint16_t val) {
  if (lastValue[idx] == val) return;
  write(idx, val);
}

/**
   Writes a value to EEPROM with wear leveling.
   @param idx Index number.
   @param value Value to store.
*/
void EEPROMWearLevel::write(const uint8_t idx, const uint16_t val) {
  uint8_t currentQueueTail = getQueueTail(idx);

  // Increment queue tail, wrapping around if necessary
  queueTail[idx]++;
  if (queueTail[idx] >= (currentQueueTail + queueEntriesCount)) {
    queueTail[idx] = currentQueueTail;
  }

  // Increment sequence number and prepare data
  lastSequenceNo[idx]++;
  QUEUE newQueue = { lastSequenceNo[idx], val };

  // Compute EEPROM address
  uint16_t offset = queueTail[idx] * QUEUE_SIZE;

  // Write new data to EEPROM
  eeprom_write_block((const void*)&newQueue, (void*)offset, QUEUE_SIZE);
  lastValue[idx] = val;
}

/**
   Reads the last stored value.
   @param idx Index number.
   @return Last stored value.
*/
uint16_t EEPROMWearLevel::read(const uint8_t idx) {
  return lastValue[idx];
}