
//  ============================================================
//
//  Program: AC_DA02_Wavefolder
//
//  Description: A waveshaper-esque program based on Dirty Mirror by The Tuesday Night Machines #TTNM
//
//  Demo video: 
//
//  I/O Usage:
//    Knob A0: not used
//    Knob A1: not used
//
//    Knob A2: Attenuator for Wave 1 input
//    Analog In 2: Input for Wave 1 (unipolar, scaled to 0-5V, input above 5V will fold over)
//    Knob A3: not used
//    Analog In 3: not used
//
//    Digital Out D0: not used
//    Digital Out D1: not used
//
//    Clock In: not used
//    Analog Out: Waveform output (unipolar, 0-5V)
//
//
//  Input Expander: unused
//  Output Expander: unused
//
//  Created:   2021-09-16 (Based on Dirty Mirror by The Tuesday Night Machines and using 20 Objects' Ardcore template)
// 
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
//  commercial work, please contact anorangenotepad. 
//
//  For more information on the Creative Commons CC BY-NC license,
//  visit http://creativecommons.org/licenses/
//
//  ================= start of global section ==================

//  constants related to the Arduino Nano pin use
const int clkIn = 2;           // the digital (clock) input
const int digPin[2] = {3, 4};  // the digital output pins
const int pinOffset = 5;       // the first DAC pin (from 5-12)
const int trigTime = 25;       // 25 ms trigger timing


//  variables used to control the current DIO output states
int digState[2] = {LOW, LOW};        // start with both set low
unsigned long digMilli[2] = {0, 0};  // a place to store millis()

int dacPreOut = 0;


//  =================== convenience routines ===================

//  These routines are some things you will need to use for
//  various functions of the hardware. Explanations are provided
//  to help you know when to use them.



//  dacOutput(long) - deal with the DAC output
//  ------------------------------------------
void 
dacOutput(byte v)
{
  
  // replacement routine as suggested by Alphonso Alba
  // this code accomplishes the same thing as the original
  // code from above, but is approx 4x faster
  PORTB = (PORTB & B11100000) | (v >> 3);
  PORTD = (PORTD & B00011111) | ((v & B00000111) << 5);
}

//  deJitter(int, int) - smooth jitter input
//  ----------------------------------------
int 
deJitter(int v, int test)
{
  // this routine just make sure we have a significant value
  // change before we bother implementing it. This is useful
  // for cleaning up jittery analog inputs.
  if (abs(v - test) > 8) {
    return v;
  }
  return test;
}


//  ==================== start of setup() ======================

//  This setup routine should be used in any ArdCore sketch that
//  you choose to write; it sets up the pin usage, and sets up
//  initial state. Failure to properly set up the pin usage may
//  lead to damaging the Arduino hardware, or at the very least
//  cause your program to be unstable.

void 
setup() 
{
 
  //set up the digital (clock) input
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
  
  
}

//  ==================== start of loop() =======================

void 
loop() 
{

   dacPreOut = analogRead(2);

  //convert to 8bit and fold/clip anything above/below
  if (dacPreOut > 255){
    dacPreOut = (255-(dacPreOut-255));
  }
  if(dacPreOut < 0){
    dacPreOut = dacPreOut*-1;
  }
  if (dacPreOut > 255){
    dacPreOut = (255-(dacPreOut-255));
  }

  
   dacOutput(dacPreOut);
  

}



//  ===================== end of program =======================
