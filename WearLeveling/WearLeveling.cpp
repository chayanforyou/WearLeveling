#include <avr/io.h>
#include <avr/eeprom.h>
#include "WearLeveling.h"

#define MAX_EEPROM_SEGMENTS 2

struct QUEUE
{
  uint32_t sequence_no;
  uint16_t my_data;
} QUEUE_ENTRY[MAX_EEPROM_SEGMENTS];

#define EEPROM_SIZE 1024
#define QUEUE_ENTRIES ((EEPROM_SIZE / sizeof(QUEUE_ENTRY[MAX_EEPROM_SEGMENTS])) / MAX_EEPROM_SEGMENTS)

uint32_t last_sequence_no[MAX_EEPROM_SEGMENTS];
uint16_t queue_tail[MAX_EEPROM_SEGMENTS];
uint16_t lastValue[MAX_EEPROM_SEGMENTS];

// Called at startup
void loadEeprom(uint8_t ASegment)
{
  int i;

  last_sequence_no[ASegment] = 0;
  lastValue[ASegment] = 0;
  queue_tail[ASegment] = (ASegment * QUEUE_ENTRIES);

  for (i = (ASegment * QUEUE_ENTRIES); i < ((ASegment * QUEUE_ENTRIES) + QUEUE_ENTRIES); i++)
  {
    // Following assumes you've written a function where the parameters
    // are address, pointer to data, bytes to read
    eeprom_read_block(&QUEUE_ENTRY[ASegment], (void*)(i * sizeof(QUEUE_ENTRY[ASegment])), sizeof(QUEUE_ENTRY[ASegment]));
    if ((QUEUE_ENTRY[ASegment].sequence_no > last_sequence_no[ASegment]) && (QUEUE_ENTRY[ASegment].sequence_no != 0xFFFFFFFF))
    {
      queue_tail[ASegment] = i;
      last_sequence_no[ASegment] = QUEUE_ENTRY[ASegment].sequence_no;
      lastValue[ASegment] = QUEUE_ENTRY[ASegment].my_data;
    }
  }
}

void writeValue(uint8_t ASegment, uint16_t value)
{
  queue_tail[ASegment]++;
  if (queue_tail[ASegment] >= (ASegment * QUEUE_ENTRIES) + QUEUE_ENTRIES)
    queue_tail[ASegment] = (ASegment * QUEUE_ENTRIES);
  last_sequence_no[ASegment]++;
  QUEUE_ENTRY[ASegment].sequence_no = last_sequence_no[ASegment];
  QUEUE_ENTRY[ASegment].my_data = value;
  // Following assumes you've written a function where the parameters
  // are address, pointer to data, bytes to write
  eeprom_write_block(&QUEUE_ENTRY[ASegment], (void*)(queue_tail[ASegment] * sizeof(QUEUE_ENTRY[ASegment])), sizeof(QUEUE_ENTRY[ASegment]));
  lastValue[ASegment] = value;
}

uint16_t getValue(uint8_t ASegment)
{
  return lastValue[ASegment];
}



