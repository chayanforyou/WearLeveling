#include "EEPROMWearLevel.h"

#define NUM_INDEXES 3    // Number of logical indexes to manage
#define EEPROM_USAGE 512 // Length of EEPROM to use for wear leveling (optional)

void setup() {
    Serial.begin(9600);

    // Initialize the EEPROMWearLevel library
    //EEPROMwl.begin(NUM_INDEXES);
    EEPROMwl.begin(NUM_INDEXES, EEPROM_USAGE);

    // Write example values to the logical indexes
    Serial.println("Writing values to EEPROM...");
    EEPROMwl.write(0, 42);    // Write to index 0
    EEPROMwl.write(1, 1234);  // Write to index 1
    EEPROMwl.write(2, 56789); // Write to index 2

    // Read the values back
    Serial.println("Reading values from EEPROM...");
    uint16_t value0 = EEPROMwl.read(0);
    uint16_t value1 = EEPROMwl.read(1);
    uint16_t value2 = EEPROMwl.read(2);

    // Print the values
    Serial.print("Index 0: ");
    Serial.println(value0);
    Serial.print("Index 1: ");
    Serial.println(value1);
    Serial.print("Index 2: ");
    Serial.println(value2);

    // Demonstrate overwriting values
    Serial.println("Overwriting value at index 1...");
    EEPROMwl.write(1, 5678);
    uint16_t newValue1 = EEPROMwl.read(1);
    Serial.print("Updated Index 1: ");
    Serial.println(newValue1);

    // Example to demonstrate wear leveling
    Serial.println("Simulating multiple writes to test wear leveling...");
    for (uint16_t i = 0; i < 50; ++i) {
        EEPROMwl.write(0, i);
        delay(50); // Simulate a delay between writes
    }

    uint16_t finalValue0 = EEPROMwl.read(0);
    Serial.print("Final value at Index 0 after wear leveling test: ");
    Serial.println(finalValue0);
}

void loop() {
    // No actions in the loop for this example
}
