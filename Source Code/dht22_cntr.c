/*
 * dht22_cntr.c
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
#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>

// ******** Declare Global variables ********
#define DHT22_PIN 0
 
 // ******** Define Parameter ********
 #define MAX_TIME 100
 
 // ******** DHT22 Control module ********
void dht22_cntr (float * dht22_value) {
	// Declare necessary variables.
	uint8_t last_state = HIGH;
	uint8_t usec_counter = 0;
	uint8_t count_data = 0;
	
	// WiringPi Setup
	wiringPiSetup();
	
	// data[0] : 8bit integral RH data
	// data[1] : 8bit decimal RH data
	// data[2] : 8bit integral Temperature data
	// data[3] : 8bit decimal Temperature data
	// data[4] : 8bit check sum
	int dht22_data [5];
	dht22_data[0] = dht22_data[1] = dht22_data[2] = dht22_data[3] = dht22_data [4] = 0;
	
	// Declare pin of dht11
	pinMode(DHT22_PIN, OUTPUT);
	
	// stage 1 : dht11_inout wire has Low && Wait 18ms
	digitalWrite(DHT22_PIN, 0);
	delay(18);
	
	// stage 2 : dht11_inout wire has High && Wait_40us
	digitalWrite(DHT22_PIN, 1);
	delayMicroseconds(40);
	
	// stage 3 : dht11_inout wire change from output to input 
	pinMode(DHT22_PIN, INPUT);
	
	// stage 4: Read DHT11 Data & Save Data to Register.
	for(int i=0; i< MAX_TIME; i++) {
		usec_counter = 0;
		
		// Counting usec_counter while DHT11_PIN doesn't changed.
		while(digitalRead(DHT22_PIN) == last_state) {
			usec_counter++;
			delayMicroseconds(1);
			
			if(usec_counter >= 200) break;
		}
		
		// Update last_state
		last_state = digitalRead(DHT22_PIN);
		if(usec_counter == 200) break;
		
		// Discard the initial 3 bits of data, and consider the counter value only if the index i is even.
		if((i>=4) && (i%2 == 0)) {
			dht22_data[count_data/8] <<= 1;
			
			if(usec_counter > 40) {
                dht22_data[count_data/8] |= 1;
            }
            
            count_data++;
		}
	}
	
	// The Master module compares the data with the checksum.
	int humidity, temperature;
	
	if((count_data >= 40) && (dht22_data[4] == ((dht22_data[0] + dht22_data[1] + dht22_data[2] + dht22_data[3]) & 0xFF))) {
        float humidity = (float)((dht22_data[0] << 8) + dht22_data[1]) / 10;
        if (humidity > 100)
        {
            humidity = dht22_data[0];    // For DHT11
        }
        
        float temperature = (float)(((dht22_data[2] & 0x7F) << 8) + dht22_data[3]) / 10;
        if (temperature > 125)
        {
            temperature = dht22_data[2];    // For DHT11
        }
        if (dht22_data[2] & 0x80)
        {
            temperature = -temperature;
        }
        
        
        dht22_value [0] = temperature;
        dht22_value [1] = humidity;
    }
}
