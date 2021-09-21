//  ============================================================
//
//  Program: ArdCore SampleHold
//
//  Description: This sketch 'holds'/captures a value from Analog In 1 
//               each time a trigger fires
//               and sends the voltage of the 'held'/captured value
//               to Analog output
//
//  Refs: AC13 (basic template)
//        AC06 (clock stuff)  
//        AC10 (potential reference?)
//        AC01 (general info, esp comments)
//        AC02 (a lot of the logic)
//            
//
//  I/O Usage:
//    Knob 1: unused
//    Knob 2: unused
//    Analog In 1: Signal to get value from
//    Analog In 2: unused
//    Digital Out 1: unused
//    Digital Out 2: unused
//    Clock In: Trigger
//    Analog Out: Sends held value on trigger
//
//  Input Expander: unused
//  Output Expander: unused
//
//  Created:  11 Sept 2021 (based on 20 Objects template and sketches listed in Refs above)
//  Modified: 
//
//  ============================================================
//
//  License:
//
//  This software is licensed under the Creative Commons
//  "Attribution-NonCommercial license. This license allows you
//  to tweak and build upon the code for non-commercial purposes,
//  without the requirement to license derivative works on the
//  same terms. If you wish to use this (or derived) work for
//  commercial work, please contact 20 Objects LLC at our website
//  (www.20objects.com).
//
//  For more information on the Creative Commons CC BY-NC license,
//  visit http://creativecommons.org/licenses/
//
//  ================= start of global section ==================

//  constants related to the Arduino Nano pin use
const int clkIn = 2;           // the digital (clock) input
const int digPin[2] = {3, 4};  // the digital output pins
const int pinOffset = 5;       // the first DAC pin (from 5-12)
//const int trigTime = 10;       // the default trigger timing (10 millisec? 25 ms?)

//  variables for interrupt handling of the clock input
volatile int clkState = LOW;

//  variables used to control the current DIO output states
int digState[2] = {LOW, LOW};  // start with both set low
unsigned long digMillis[2] = {0, 0}; //a place to store millis() (need?)
unsigned long digTime = 0;     // the delay time for a trigger (need?)

// variables of interest
//int transpose = 0;             // the transposition amount
int inValue = 0;               // the input value
byte outValue = 0;             // the DAC output value
int oldOut = -1;               // the last output value

int doHold = 0;               // flag to hold/capture value



//  =================== convenience routines ===================

//  isr() - quickly handle interrupts from the clock input
//  ------------------------------------------------------
void 
isr()
{ 
  clkState = HIGH;
}

//  dacOutput(byte) - deal with the DAC output
//  -----------------------------------------
void dacOutput(byte v)
{
  PORTB = (PORTB & B11100000) | (v >> 3);
  PORTD = (PORTD & B00011111) | ((v & B00000111) << 5);
}

//  deJitter(int, int) - smooth jitter input
//  ----------------------------------------

// this routine just make sure we have a significant value
// change before we bother implementing it. This is useful
// for cleaning up jittery analog inputs.
int 
deJitter(int v, int test)
{
  if (abs(v - test) > 8)
  {
    return v;
  }
  return test;
}

//  ==================== start of setup() ======================
//  Standard setup routine - see ArdCore_Template for info.

void 
setup() 
{
  
  // if you need to send data back to your computer, you need
  // to open the serial device. Otherwise, comment below line out.
  //Serial.begin(9600);         

  // set up the digital (clock) input
  pinMode(clkIn, INPUT);
  
  // set up the digital outputs
  for (int i=0; i<2; i++) {
    pinMode(digPin[i], OUTPUT);
    digitalWrite(digPin[i], LOW);
  }
  
  // set up the 8-bit DAC output pins
  for (int i=0; i<8; i++) {
    pinMode(pinOffset+i, OUTPUT);
    digitalWrite(pinOffset+i, LOW);
  }

  
  // set up an interrupt handler for the clock in. If you
  // aren't going to use clock input, you should probably
  // comment out this call.
  // Note: Interrupt 0 is for pin 2 (clkIn)
  attachInterrupt(0, isr, RISING);
}


//  ==================== start of loop() =======================

void 
loop()
{

  int tempVal = 0;
  
  // test for input change & get the current value
  tempVal = deJitter(analogRead(2), inValue);
  
  if (tempVal != inValue) 
  {
    inValue = tempVal;
  }
  


  // test for clock tick (we do hold on clock)
  if (clkState == HIGH) 
  {
    clkState = LOW;
    doHold = 1;
  }

  //do the hold if the flag is set to 1 (1 is 'true' 0 is 'false')
  if (doHold)
  {
    outValue = inValue;
    if (outValue != oldOut) 
    {
      dacOutput(outValue);
      oldOut = outValue;
    }
    doHold = 0;
  }

}


  




//  ===================== end of program =======================
