#include <avr/io.h>
#include <avr/eeprom.h>
#include "EEPROMWearLevel.h"

EEPROMWearLevel EEPROMwl;

void EEPROMWearLevel::begin(const uint8_t amountOfIndexes) {
	QUEUE_ENTRY = new QUEUE[amountOfIndexes];
	last_sequence_no = new uint32_t[amountOfIndexes];
	queue_tail = new uint8_t[amountOfIndexes];
	lastValue = new uint16_t[amountOfIndexes];
	QUEUE_ENTRIES = ((EEPROM_SIZE / QUEUE_SIZE) / amountOfIndexes);
	
	// reads the last written value from all index
	int index;
	for (index = 0; index < amountOfIndexes; index++) {
		getLastSavedValue(index);
	}
}

void EEPROMWearLevel::begin(const uint8_t amountOfIndexes, const uint16_t eepromLengthToUse) {
	QUEUE_ENTRY = new QUEUE[amountOfIndexes];
	last_sequence_no = new uint32_t[amountOfIndexes];
	queue_tail = new uint8_t[amountOfIndexes];
	lastValue = new uint16_t[amountOfIndexes];
	QUEUE_ENTRIES = ((eepromLengthToUse / QUEUE_SIZE) / amountOfIndexes);
	
	// reads the last written value from all index
	int index;
	for (index = 0; index < amountOfIndexes; index++) {
		getLastSavedValue(index);
	}
}

uint8_t EEPROMWearLevel::getQueueTail(const uint8_t idx) {
  return (idx * QUEUE_ENTRIES);
}

void EEPROMWearLevel::getLastSavedValue(const uint8_t idx) {
	int index;
	uint8_t currentQueueTail = getQueueTail(idx);
	
	last_sequence_no[idx] = 0;
	lastValue[idx] = 0;
	queue_tail[idx] = currentQueueTail;
	
	for (index = currentQueueTail; index < (currentQueueTail + QUEUE_ENTRIES); index++) {
		// Following assumes you've written a function where the parameters
		// are data, location address, size of data
		eeprom_read_block((void*)&QUEUE_ENTRY[idx], (const void*)(index * QUEUE_SIZE), QUEUE_SIZE);
		if ((QUEUE_ENTRY[idx].sequence_no > last_sequence_no[idx]) && (QUEUE_ENTRY[idx].sequence_no != 0xFFFFFFFF))
		{
		  queue_tail[idx] = index;
		  last_sequence_no[idx] = QUEUE_ENTRY[idx].sequence_no;
		  lastValue[idx] = QUEUE_ENTRY[idx].my_data;
		}
	}
}

void EEPROMWearLevel::write(const uint8_t idx, const uint16_t value) {
  queue_tail[idx]++;
  uint8_t currentQueueTail = getQueueTail(idx);
  if (queue_tail[idx] >= (currentQueueTail + QUEUE_ENTRIES))
    queue_tail[idx] = currentQueueTail;
  last_sequence_no[idx]++;
  QUEUE_ENTRY[idx].sequence_no = last_sequence_no[idx];
  QUEUE_ENTRY[idx].my_data = value;
  // Following assumes you've written a function where the parameters
  // are data, location address, size of data
  eeprom_write_block((const void*)&QUEUE_ENTRY[idx], (void*)(queue_tail[idx] * QUEUE_SIZE), QUEUE_SIZE);
  lastValue[idx] = value;
}

uint16_t EEPROMWearLevel::read(const uint8_t idx) {
  return lastValue[idx];
}

