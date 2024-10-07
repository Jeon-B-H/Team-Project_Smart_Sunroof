/*
 * sunroof_cntr.c
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
 
 // Declare library
#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void sunroof_cntr(char* flag_init, char* infrared_value, int sunlight_value, float* dht22_value, char* sunroof_value)
{
	// Declare motor control variables.
	char up_down, motor_enable;
	
	// Get temperature, humidity
	float temperature = dht22_value[0];
	float humidity = dht22_value[1]; 
	
	// Get top infrared value, bottom infrared value
	char top_infrared = infrared_value[0];
	char bottom_infrared = infrared_value[1];
	
	// Select down_up, motor_enable
	// Up : Top_infrared == 1 && Down_infrared == 0
	// Down : Top_infrared == 0 && Down_infrared == 1
	if(!(*flag_init)) {
		if(top_infrared && !bottom_infrared) {
			*flag_init = 1;
			up_down = 0;
			motor_enable = 0;
		}else {
			up_down = 1;
			motor_enable = 1;
		}
	}
	else if(sunlight_value > 150 || temperature >= 28)  {
			up_down = 0;
			motor_enable = 1;
		}
	else {
			up_down = 1;
			motor_enable = 1;
	}	
	
	printf("up_down = %d, motor_enable = %d, flag_init = %d\n", up_down, motor_enable, *flag_init);
	printf("sunlight_value = %d infrared_value_TOP = %d infrared_value_BOTTOM = %d \n", sunlight_value, infrared_value[0], infrared_value[1]);
	
	// Return down_up, motor_enable variables
	sunroof_value[0] = up_down;
	sunroof_value[1] = motor_enable;
}

