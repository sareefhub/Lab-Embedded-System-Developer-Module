#include "my_EEPROM.h"

void scan_EEPROM(void) {
    uint16_t data_location[1024];
    int number_data_found = 0;
    uint8_t d;

    Serial.println("Performing EEPROM scanning...");
    for (int i = 0; i < 1024; i++) {
        d = EEPROM_read1byte(i);
        if (d != 0xFF) {
            data_location[number_data_found] = i;
            number_data_found++;
            Serial.print("0x");
            Serial.print(d, HEX);
            Serial.print(" is found in EEPROM location[");
            Serial.print(i);
            Serial.println("]");
        }
    }

    if (number_data_found > 1) {
        Serial.println("More than one data is found in EEPROM");
        for (int i = 0; i < number_data_found; i++) {
            EEPROM_Erase_only(data_location[i]);
        }
        Serial.println("EEPROM Cleared.");
    } else if (number_data_found == 0) {
        Serial.println("No data found in EEPROM");
        EEPROM_Write_to_Empty_location(1021, 0xFA);
        EEPROM_Write_to_Empty_location(1022, 0xCE);
        EEPROM_Write_to_Empty_location(1023, 0x0F);
        Serial.println("0xFACE0F has been added to EEPROM at locations 1021-1023");
    } else if (number_data_found == 3) {
        if (EEPROM_read1byte(data_location[0]) == 0x0F && 
            EEPROM_read1byte(data_location[0] - 1) == 0xCE && 
            EEPROM_read1byte(data_location[0] - 2) == 0xFA) {

            EEPROM_Erase_only(data_location[0]);
            EEPROM_Erase_only(data_location[0] - 1);
            EEPROM_Erase_only(data_location[0] - 2);

            EEPROM_Write_to_Empty_location(data_location[0] - 3, 0xFA);
            EEPROM_Write_to_Empty_location(data_location[0] - 4, 0xCE);
            EEPROM_Write_to_Empty_location(data_location[0] - 5, 0x0F);

            Serial.println("EEPROM Updated.");
        }
    }
}

void setup() {
    Serial.begin(38400);
    display_all_data_in_EEPROM();
    scan_EEPROM();
}

void loop() {
    // Loop does nothing
}