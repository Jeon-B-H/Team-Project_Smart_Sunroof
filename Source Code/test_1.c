/*
 * test_1.c
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
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

// Declare PIN
#define IN1 22
#define IN2 23
#define IN3 24
#define IN4 25

// Function prototype.
void step_motor_cntr (int up_down, int motor_enable);
void runStep(int value1, int value2, int value3, int value4);


// Main method
int main(int argc, char **argv)
{
	// Set wiringPi
	wiringPiSetup();
	
	// Set Pinmode
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	
	step_motor_cntr(1, 1);
	
	return 0;
}

void step_motor_cntr (int up_down, int motor_enable) {
	static char step = 0;
	
	while(motor_enable) {
		switch(step) {
            case 0:  runStep(LOW, LOW, LOW, HIGH);   break; 
            case 1:  runStep(LOW, LOW, HIGH, HIGH);   break; 
            case 2:  runStep(LOW, LOW, HIGH, LOW);   break; 
            case 3:  runStep(LOW, HIGH, HIGH, LOW);   break; 
            case 4:  runStep(LOW, HIGH, LOW, LOW);   break; 
            case 5:  runStep(HIGH, HIGH, LOW, LOW);   break; 
            case 6:  runStep(HIGH, LOW, LOW, LOW);   break; 
            case 7:  runStep(HIGH, LOW, LOW, HIGH);   break;     
            default:  runStep(LOW, LOW, LOW, LOW);   break; 
        }
        
        if(up_down) {
			if(step >= 7) step = 0;
			else step = step + 1;
		}else {
			if(step <= 0) step = 7;
			else step = step - 1;
		}
		
		delayMicroseconds(800);
	}
}

void runStep(int value1, int value2, int value3, int value4) {
      digitalWrite(IN1, value1); 
      digitalWrite(IN2, value2);
      digitalWrite(IN3, value3);
      digitalWrite(IN4, value4);  
 }
