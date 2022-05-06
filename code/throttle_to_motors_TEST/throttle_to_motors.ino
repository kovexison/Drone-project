/** Program for reading pulse widths (HIGH duration) from  CH1-4 on receiver */
#include <Servo.h>

//Declaring variables
byte last_CH1, last_CH2, last_CH3, last_CH4;
int receiver_CH1, receiver_CH2, receiver_CH3, receiver_CH4;
int counter_CH1, counter_CH2, counter_CH3, counter_CH4, loop_counter, strt;
unsigned long timer_CH1, timer_CH2, timer_CH3, timer_CH4, esc_timer, esc_loop_timer;
unsigned long current_time, zero_timer, timer1, timer2, timer3, timer4;

Servo M1;
Servo M2;
Servo M3;
Servo M4;
  
/*ISR(PCINT0_vect) {  //interrupt subrutine if any pin state between 8 and 11 changes
  current_time = micros();
  
  //Channel 1 - 
  if(last_CH1 == 0 && PINB & B00000001){  //input 8 is changed from 0 to 1 (rising edge of the pulse) - PINB & B00000001 = digitalRead(8,HIGH) used to use less resources of the microcontroller
    last_CH1 = 1;
    timer_CH1 = current_time;                        //so: if input 8 changes from 0 to 1 (pulse is falling), remember it's current state and set timer_1 to current micros() for measuring pulse width in microseconds
  } else if(last_CH1 == 1 && !(PINB & B00000001)){  //if input 8 is changed from 1 to 0
    last_CH1 = 0;
    receiver_CH1 = current_time - timer_CH1;          //so: if input 8 is changed from 1 to 0, remember it's current state and calculate pulse width (pulse time) in microseconds to know the position on the transmitter's height stick
  }

  //Channel 2
  if(last_CH2 == 0 && PINB & B00000010) {
    last_CH2 = 1;
    timer_CH2 = current_time;
  } else if(last_CH2 == 1 && !(PINB & B00000010)) {
    last_CH2 = 0;
    receiver_CH2 = current_time - timer_CH2;
  }

  //Channel 3
  if(last_CH3 == 0 && PINB & B00000100) {
    last_CH3 = 1;
    timer_CH3 = current_time;
  } else if(last_CH3 == 1 && !(PINB & B00000100)) {
    last_CH3 = 0;
    receiver_CH3 = current_time - timer_CH3;
  }

  //Channel 4
  if(last_CH4 == 0 && PINB & B00001000) {
    last_CH4 = 1;
    timer_CH4 = current_time;
  } else if(last_CH4 == 1 && !(PINB & B00001000)) {
    last_CH4 = 0;
    receiver_CH4 = current_time - timer_CH4;
  }
}*/


void setup() {
  // put your setup code here, to run once:
  //Atmega pins are input pins as default, no need to specify these.
  DDRD |= B11110000;       //setting digital ports 4,5,6,7 as output (ESC outputs)
 // DDRB |= B00010000;       //configuring port 12 as output (LED for visible signals for the user)
 /* PCICR |= (1 << PCIE0);   //set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0); //set PCINT0 (corresponds to digital input 8 - CH1 of receiver) to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT1); //set PCINT1 (corresponds to digital input 9 - CH2 of receiver) to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT2); //set PCINT2 (corresponds to digital input 10 - CH3 of receiver) to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT3); //set PCINT3 (corresponds to digital input 11 - CH4 of receiver) to trigger an interrupt on state change*/
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  
  //M1.attach(4,1000,2000);
 // M2.attach(5,1000,2000);
  M3.attach(6,1000,2000);
 // M4.attach(7,1000,2000);*/
  
  
  
//  Serial.begin(115200);
 /* while(receiver_CH3 < 990 || receiver_CH3 > 1020){
    M1.writeMicroseconds(1000);
    M2.writeMicroseconds(1000);
    M3.writeMicroseconds(1000);
    M4.writeMicroseconds(1000);
    delayMicroseconds(19000);
  }*/
  //the drone waits until the receiver is active and the throttle stick is set to the lowest position
 /* while(receiver_CH3 < 990 || receiver_CH3 > 1020){
    strt++; //while waiting, increment the start variable in every loop
    //I don't want the ESC's to beep annoyingly, so while waiting, I'm sending a 1000us pulse to them
    PORTD |= B11110000;     //set digital ports 4,5,6,7 to HIGH
    delayMicroseconds(1000);
    PORTD &= B00001111;     //set ESC ports to LOW
    delay(3);               //wait 3 ms to the next loop
    if(strt == 125) {
      digitalWrite(12, !digitalRead(12));  //change the led status in every 125 loops (500ms)
      strt = 0;
    }
  }*/

  //strt = 0;
  //zero_timer = micros();   //set the zero_timer for the first loop
}

void loop() {
  // put your main code here, to run repeatedly:
 // while(20000 - zero_timer  > micros()); //start the pulse after 20000us //to make sure that the refresh rate is always 50Hz
  receiver_CH1 = pulseIn(8,HIGH);
  receiver_CH2 = pulseIn(9,HIGH);
  receiver_CH3 = pulseIn(10,HIGH);
  receiver_CH4 = pulseIn(11,HIGH);
  // delayMicroseconds(4000-timer_CH3);
  // zero_timer = micros();               //reset the zero_timer
  //receiver_CH3 = 1000;
 // PORTD |= B11110000;  //set ESC outputs high
  //calculate delay times - throttle input from the receiver
  /*timer_CH1 = receiver_CH3 + zero_timer;
  timer_CH2 = receiver_CH3 + zero_timer;
  timer_CH3 = receiver_CH3 + zero_timer;
  timer_CH4 = receiver_CH3 + zero_timer;*/
  

  timer_CH3 = receiver_CH3;
 /* while(PORTD >= 16){  //the while loop is executed until the 4 highest bits in PORTD data register are 0 (this equals the finishing of all the ESC pulses) (until all the ESC ports are low)
    esc_loop_timer = micros();
    if(timer_CH1 <= esc_loop_timer) PORTD &= B11101111;  //when the delay time expires, the corresponding port is set low
    if(timer_CH2 <= esc_loop_timer) PORTD &= B11011111;
    if(timer_CH3 <= esc_loop_timer) PORTD &= B10111111;
    if(timer_CH4 <= esc_loop_timer) PORTD &= B01111111;
    //when all the ports are low, the while loop is terminated
  }*/
  
  //M1.writeMicroseconds(timer_CH3);
 // M2.writeMicroseconds(timer_CH3);
  M3.writeMicroseconds(timer_CH3);
  //M4.writeMicroseconds(timer_CH3);*/
  //Serial.println(receiver_CH3);

  //PORTB |= B00010000;  //set port 12 to high
  //delayMicroseconds(1500);
  //PORTB &= B11101111;  //set port 12 to low

}
