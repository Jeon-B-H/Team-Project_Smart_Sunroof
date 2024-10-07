/*
 * step_motor_cntr.c
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
void motor_step_cntr (int down_up, int motor_enable, char* infrared_value);
void runStep(int value1, int value2, int value3, int value4);
void step_motor_cntr (char* sunroof_value, char* infrared_value);
void infrared_sunroof_cntr(char *);

/* void main(void) {
	char sunroof_value [2];
	sunroof_value[0] = 1;
	sunroof_value[1] = 1;
	
	char infrared_value[2];
	infrared_value[0] = 0;
	infrared_value[1] = 0;
	
	while(1) step_motor_cntr(sunroof_value, infrared_value);
} */

// Main method
void step_motor_cntr (char* sunroof_value, char* infrared_value)
{
	// Set wiringPi
	wiringPiSetup();
	
	// Get down_up, motor_enable
	char up_down = sunroof_value[0];
	char motor_enable = sunroof_value[1];
	
	// Set Pinmode
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);

	// Flag of Sunroof position
	static char sunroof_up_down = 0;
	
	// Infrared of top, bottom.
	char bottom_infrared = infrared_value[0];
	char top_infrared = infrared_value[1];
	
	if(up_down && !sunroof_up_down) {
		motor_step_cntr(up_down, motor_enable, infrared_value);
		sunroof_up_down = 1;
	}else if(!up_down && sunroof_up_down) {
		motor_step_cntr(up_down, motor_enable, infrared_value); 
		sunroof_up_down = 0;
	}
	
	printf("sunroof_up_down = %d\n",sunroof_up_down);
}


void motor_step_cntr(int up_down, int motor_enable, char* infrared_value) {
	static char step = 0;
	char flag_stop = 0;
    
	while(motor_enable) {
			flag_stop = 0;
			infrared_sunroof_cntr(infrared_value);
			
			char bottom_infrared = infrared_value[0];
			char top_infrared = infrared_value[1];
		
			switch (step) {
				case 0:  runStep(LOW, LOW, LOW, HIGH);   break;
				case 1:  runStep(LOW, LOW, HIGH, HIGH);  break;
				case 2:  runStep(LOW, LOW, HIGH, LOW);   break;
				case 3:  runStep(LOW, HIGH, HIGH, LOW);  break;
				case 4:  runStep(LOW, HIGH, LOW, LOW);   break;
				case 5:  runStep(HIGH, HIGH, LOW, LOW);  break;
				case 6:  runStep(HIGH, LOW, LOW, LOW);   break;
				case 7:  runStep(HIGH, LOW, LOW, HIGH);  break;
				default: runStep(LOW, LOW, LOW, LOW);    break;
			}
    
			if (up_down) {
				if(!bottom_infrared) flag_stop = 1;
				
				if (step >= 7) step = 0;
				else step = step + 1;
			} else {
				if(!top_infrared) flag_stop = 1;
				
				if (step <= 0) step = 7;
				else step = step - 1;
			}

			delayMicroseconds(800);
			
			if(flag_stop) break;
    }
    
    flag_stop = 0;
    printf("flag_stop %d \n",flag_stop);
} 

/* void motor_step_cntr (int down_up, int motor_enable) {
	static char step = 0;
	
	// msecond counter
	int counter_sequence = 0;
	int delay_of_motor = 800;
	
	while(motor_enable && counter_sequence < 4900) {
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
        
			if(down_up) {
				if(step >= 7) step = 0;
				else step = step + 1;
			}else {
				if(step <= 0) step = 7;
				else step = step - 1;
			}
		
			delayMicroseconds(delay_of_motor);
	}	
} */

void runStep(int value1, int value2, int value3, int value4) {
      digitalWrite(IN1, value1); 
      digitalWrite(IN2, value2);
      digitalWrite(IN3, value3);
      digitalWrite(IN4, value4);  
 }
