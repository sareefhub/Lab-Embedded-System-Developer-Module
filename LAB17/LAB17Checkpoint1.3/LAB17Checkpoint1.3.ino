#include "my_EEPROM.h"

uint16_t scan_EEPROM(void)
{
  //array data change
  uint8_t addr_location[] = {} ;
  uint8_t data_location[] = {};
  int index,i, number_data_found;
  //value addr
  uint8_t d;

  number_data_found = 0;
  Serial.println("Performing EEPROM scanning...");
  //check addr 1024 position
  for(i=0;i<1024;i++)
  {
    d = EEPROM_read1byte(i);
    if (d != 0xFF)
    {
      addr_location[number_data_found] = i;
      data_location[number_data_found] = d;
      number_data_found++;
      Serial.print(d);
      Serial.print(" is found in EEPROM location[");
      Serial.print(i);
      Serial.println("]");     
    }
    
  }
  if (number_data_found > 1)
  {
    Serial.println("More than one data are found in EEPROM");

    for (int i = 0; i < number_data_found; i++) 
    {
      if (EEPROM_read1byte(addr_location[i]) == 0xFA)
        {
          index = i;
        }
    }
    bool data_adjacent = (data_location[index] == 0xFA) &(data_location[index+1] == 0xCE)&(data_location[index+2] == 0x0F);
    //if FA CE 0F adjacent
    if(data_adjacent)
    {
      for(int j = index; j<number_data_found; j++)
      {
        EEPROM_Erase_and_Write1B(addr_location[j]-3,data_location[j]);
        EEPROM_Erase_only(addr_location[j]);
      }
    }
    //not found data in 
    for(int i=0; i<number_data_found; i++)
    {
      EEPROM_Erase_only(addr_location[i]);
    }
  }
  else if(number_data_found ==0)
    Serial.println("no data found in EEPROM");
    EEPROM_Write_to_Empty_location(1021, 0xFA);
    EEPROM_Write_to_Empty_location(1022, 0xCE);
    EEPROM_Write_to_Empty_location(1023, 0x0F);
    Serial.println("0xFA,0xCE,0x0F has been add in addr EEPROM");
}

void setup() 
{
  Serial.begin(38400);
  display_all_data_in_EEPROM();
  scan_EEPROM();
}

void loop() {
  // put your main code here, to run repeatedly:

}