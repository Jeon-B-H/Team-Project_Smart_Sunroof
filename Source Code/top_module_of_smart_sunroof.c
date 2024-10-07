/*
 * top_module_of_smart_sunroof.c
 * 
 * Copyright 2024  <pi@seonghunPi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

// ******** Include library ********
#include <wiringPiI2C.h>
#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>

// ******** Declare function prototype ********
// Sensor functions
void dht22_cntr (float *);
int cds_cntr();
void infrared_sunroof_cntr(char *);

// Control functions
void step_motor_cntr (char* sunroof_value, char* infrared_value);
void lcd_display_cntr(float* dht22_value);
void sunroof_cntr(char* flag_init, char* infrared_value, int sunlight_value, float* dht22_value, char* sunroof_value);

// ******** Top module of smart sunroof ********
int main(int argc, char **argv)
{
	// Decalre sensor variables.	
	float dht22_value [2];
	char infrared_value [2] = {0, 0};
	int sunlight_value, distance_cm;
	
	// Declare control step-motor value.
	char sunroof_value [2];
	
	// Flag of initialization.
	char flag_init = 0;
	
	// Control sun-roof
	while(1) {
		// Get sensor values.
		dht22_cntr(dht22_value);
		sunlight_value = cds_cntr();
		infrared_sunroof_cntr(infrared_value);
		
		// Control functions
		lcd_display_cntr(dht22_value);
		sunroof_cntr(&flag_init, infrared_value, sunlight_value, dht22_value, sunroof_value);
		step_motor_cntr (sunroof_value, infrared_value);
	}
	return 0;
}

