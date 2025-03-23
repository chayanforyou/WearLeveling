#ifndef EEPROM_WEAR_LEVEL_H
#define EEPROM_WEAR_LEVEL_H

#include <EEPROM.h>

#define QUEUE_SIZE sizeof(EEPROMWearLevel::QUEUE)

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
      writes one byte if it is not the same as the last one.
    */
    void update(const uint8_t idx, const uint16_t val);

    /**
       writes one byte no matter what value was written before.
    */
    void write(const uint8_t idx, const uint16_t val);

  private:
    struct QUEUE {
      uint32_t sequenceNo;
      uint16_t data;
    };

    QUEUE* queueEntries = nullptr;
    uint32_t* lastSequenceNo = nullptr;
    uint8_t* queueTail = nullptr;
    uint16_t* lastValue = nullptr;
    uint16_t queueEntriesCount = 0;

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

#endif  // #ifndef EEPROM_WEAR_LEVEL_H