#ifndef EEPROM_WEAR_LEVEL_H
#define EEPROM_WEAR_LEVEL_H

#include <EEPROM.h>

#define QUEUE_SIZE 6
#define EEPROM_SIZE EEPROMClass::length()

class EEPROMWearLevel: EEPROMClass {
  public:
    /**
        Initialises EEPROMWearLevel. One of the begin() methods must be called
        before any other method.
        This method uses the whole EEPROM for wear leveling.
        @param amountOfIndexes the amount of indexes you want to use.
    */
    void begin(const uint8_t amountOfIndexes);
	
    /**
        Initialises EEPROMWearLevel. One of the begin() methods must be called
        before any other method.
        @param amountOfIndexes the amount of indexes you want to use.
        @param eepromLengthToUse the length of the EEPROM to use for wear leveling in case you want
        to use parts of the EEPROM for other purpose.
    */
    void begin(const uint8_t amountOfIndexes, const uint16_t eepromLengthToUse);
	
    /**
       reads one byte from idx
    */
	uint16_t read(const uint8_t idx);

    /**
       writes one byte no matter what value was written before.
    */
	void write(const uint8_t idx, const uint16_t value);
	
  private:
	struct QUEUE {
		uint32_t sequence_no;
		uint16_t my_data;
	} *QUEUE_ENTRY;

	uint32_t *last_sequence_no;
	uint8_t  *queue_tail;
	uint16_t *lastValue;
	uint16_t QUEUE_ENTRIES;
	
    /**
       reads the last written value of idx or leaves t unchanged if no
       value written yet.
    */
	void getLastSavedValue(const uint8_t idx);
    /**
       returns the current queue tail.
       This method can be called to use EEPROMWEarLevel as a ring buffer.
    */
    uint8_t getQueueTail(const uint8_t idx);
};

/**
   the instance of EEPROMWearLevel
*/
extern EEPROMWearLevel EEPROMwl;

#endif	// #ifndef EEPROM_WEAR_LEVEL_H

