#ifndef DMC40457_CLOCK_H
#define DMC40457_CLOCK_H

#include <Wire.h>
#include <Arduino.h>
#include "LiquidCrystal_I2C_edit.h"

class DMC40457_clock{
	LiquidCrystal_I2C lcd;
public:
	DMC40457_clock(uint8_t , uint8_t , uint8_t);
	void begin();
	void update(int hour, int minutes, int temperature);
	void store_custom_chars();
	void update_display(uint8_t new_mask[4][40], bool additive = false);
	
private:
	uint8_t _custom_chars[4][8] = {	{0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111},
									{0b11111,0b11111,0b11111,0b11111,0b00000,0b00000,0b00000,0b00000},
									{0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111,0b11111},
									{0b00000,0b00000,0b01110,0b01110,0b01110,0b01110,0b00000,0b00000}};
									
	uint8_t _display_mask[4][40] = {0};
	
	uint8_t _numbers[10][16] = {{1,1,1,1 , 1,0,0,1 , 1,0,0,1 , 1,1,1,1} , // 0
								{0,0,0,1 , 0,0,0,1 , 0,0,0,1 , 0,0,0,1} , // 1
								{1,1,1,1 , 3,3,3,1 , 1,2,2,2 , 1,1,1,1} , // 2
								{1,1,1,1 , 0,3,3,1 , 0,2,2,1 , 1,1,1,1} , // 3
								{1,0,0,1 , 1,3,3,1 , 2,2,2,1 , 0,0,0,1} , // 4
								{1,1,1,1 , 1,3,3,3 , 2,2,2,1 , 1,1,1,1} , // 5
								{1,1,1,1 , 1,3,3,3 , 1,2,2,1 , 1,1,1,1} , // 6
 								{1,1,1,1 , 0,0,0,1 , 0,0,1,0 , 0,0,1,0} , // 7
								{1,1,1,1 , 1,3,3,1 , 1,2,2,1 , 1,1,1,1} , // 8
								{1,1,1,1 , 1,3,3,1 , 2,2,2,1 , 1,1,1,1}}; // 9
	
	uint8_t _map_colon[4] = {0,4,4,0};
	uint8_t _colon_offset = 12;
	
	uint8_t _time_digit_offsets[4] = {2,7,14,19}; //hour d1, hour d0, min d1, min d0
};

#endif