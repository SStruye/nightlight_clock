#include "DMC40457_clock.h"
#include "LiquidCrystal_I2C_edit.h"
#include <string>

DMC40457_clock::DMC40457_clock(uint8_t addr, uint8_t cols, uint8_t rows) : lcd(addr, cols, rows){
	lcd.begin();
}

void DMC40457_clock::begin(){
	lcd.setCursor(0,0);
    lcd.clear();
	store_custom_chars();
}

void DMC40457_clock::store_custom_chars(){
	for(int i = 1; i < 5; i++){
		lcd.createChar(i, _custom_chars[i-1]);
	}
}

void DMC40457_clock::update(int hour, int minutes, int temperature){
	uint8_t hour_d0 = hour % 10;
	uint8_t min_d0 = minutes % 10;
	uint8_t digits[4] = {(hour - hour_d0)/10, hour_d0,  (minutes - min_d0)/10, min_d0};
	
	uint8_t new_mask[4][40] = {0};
	
	for(int row = 0; row < 4; row++){
		for(int new_char_idx = 0; new_char_idx < 4; new_char_idx++){
			for(int new_digit = 0; new_digit < 4;new_digit++){
				new_mask[row][_time_digit_offsets[new_digit] + new_char_idx] = _numbers[digits[new_digit]][new_char_idx + row*4];
				Serial.println(_numbers[digits[new_digit]][new_char_idx + row*4]);
			}
		}
	}
	
	for(int i = 0; i < 4; i++){
		new_mask[i][_colon_offset] = _map_colon[i];
	}
	
	std::string s = std::to_string(temperature);
	char const *temp = s.c_str(); 
	
	update_display(new_mask);
	lcd.setCursor(30, 2);
	lcd.print("temp:");
	lcd.setCursor(35, 2);
	lcd.print(temp);
	lcd.setCursor(37, 2);
	lcd.printLCD("dC");
}

void DMC40457_clock::update_display(uint8_t new_mask[4][40], bool additive){
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 40; col++){
			uint8_t new_value = new_mask[row][col];
			if (new_value != _display_mask[row][col]){
				lcd.setCursor(col, row);
				if(new_value == 0){
					lcd.print(" ");
				}else{
					lcd.write(new_value);
				}
				//delay(50);
				_display_mask[row][col] = new_value;
			}
		}
	}
	lcd.home();
}
