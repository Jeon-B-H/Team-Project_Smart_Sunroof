/*
 * hc_sr04_cntr.c
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

#define TRIG 4        
#define ECHO 5        
#define LIGHTPIN 6    

void Trigger()
{
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    delayMicroseconds(200); 
}

// Get distance
int Dist()
{
    Trigger();
    
    // Wait for the ECHO pin to go HIGH
    while (digitalRead(ECHO) == LOW);
    
    int t1 = micros(); // Get the start time (in microseconds)
    
    // Wait for the ECHO pin to go LOW
    while (digitalRead(ECHO) == HIGH);
    
    int t2 = micros(); // Get the end time
    
    // Calculate the distance in cm and convert to integer
    double dist = (t2 - t1) * 0.017;  // Half the round trip time * speed of sound in cm/¥ìs
    return (int)dist;  // Convert to integer
}


// HC-SR04 Control module
int hc_sr04_cntr(void)
{
    // WiringPi setup
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed!\n");
        return 1;
    }

    // Pin mode setup
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // Initialize the TRIG pin to LOW
    digitalWrite(TRIG, LOW);
    delay(30); // Let the sensor settle

    // Measure distance
    int distance = Dist();
        
    // Delay before next measurement
    delay(1000); // 1 second delay

    return distance;
}
