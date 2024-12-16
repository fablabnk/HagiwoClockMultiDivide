//Clock M_D module
//Original by Hagiwo
//Modifications by the members of synthlab berlin
#include <MsTimer2.h>

unsigned long ext_count = 400;//For timer count
unsigned long old_ext_pulse = 0;
unsigned long old_int_pulse = 0;
unsigned long ext_period = 0;//cycle
unsigned long ext_count_result = 0;//Timer count result
unsigned long old_ext_count_result = 0;//Timer count  old result
byte ext_pulse = 0;//External clock: 1 if present, 0 if not
byte int_pulse = 0;//Internal Clock
byte ext_injudge = 1;//Determines whether external input is present. If there is, it is 1; if not, it is 0. If the timer count exceeds the specified value, the judgment is switched.
byte old_ext_injudge = 2;

int AD_rate = 512;//Rate knob input
int rate = 1000;//Used when the internal clock is running

int AD_MD = 512;//MD knob input value
int out_width_ch1 = 10;//The output pulse width.
int out_width_ch2 = 10;//The output pulse width.
int old_AD_MD = 512;//Measures to prevent misreading of RATE value when switching SW
int MD_ch1 = 5;//Multiple, divider determination ch1
int MD_ch2 = 5;//Multiple, divider determination ch2
int M_period_ch1 = 0;//When multiple, the period of the output clock. If not multiple, set to 0.
int M_period_ch2 = 0;//When multiple, the period of the output clock. If not multiple, set to 0.
int M_count_ch1 = 1;//When multiple, it counts each pulse and returns to 0 when it reaches the set value.
int M_count_ch2 = 1;//When multiple, it counts each pulse and returns to 0 when it reaches the set value.
int D_count_ch1 = 1;//When an external pulse is received, the count position is increased.
int D_count_ch2 = 1;//When an external pulse is received, the count position is increased.
int D_full_ch1 = 1;//Count Upper Limit
int D_full_ch2 = 1;//Count Upper Limit
byte CH1out = 0;//0 is LOW output, 1 is HIGH output
byte CH2out = 0;//0 is LOW output, 1 is HIGH output
byte M_done_ch1 = 0; //To check multiple output. Without it, multiple pulses will be output with one clock.
byte M_done_ch2 = 0; //To check multiple output. Without it, multiple pulses will be output with one clock.


//---Mode switching-------------
byte mode_sw = 1;
byte mode = 1;//1=CH1,2=CH2
byte old_mode = 0;//Measures to prevent misreading of RATE value when switching SW
int old_MD_ch1 = 0;//Fixed a bug that caused the divider clock to go out of sync when switching the switch.
int old_MD_ch2 = 0;//Fixed a bug that caused the divider clock to go out of sync when switching the switch.

void setup() {
 pinMode(7, OUTPUT); //CH1out
 pinMode(8, OUTPUT); //CH2out
 pinMode(13, OUTPUT); //internal_clock_out
 pinMode(11, INPUT_PULLUP); //SW
 pinMode(3, INPUT); //ext_clock_in
 Serial.begin(9600);

 MsTimer2::set(1, timer_count); // Timer counts every 1ms
 MsTimer2::start();//When the external input becomes high, count to the next high
}

void loop() {
 AD_MD = analogRead(3);
 AD_rate = 1023 - analogRead(4);
 rate = AD_rate * 2 + 100;

 old_ext_pulse = ext_pulse;
 old_int_pulse = int_pulse;
 ext_pulse = digitalRead(3);

 mode_sw = digitalRead(11);
 old_mode = mode;

 old_ext_injudge = ext_injudge;

 //-----------Divider output count reset------------
 if (D_count_ch1 >= D_full_ch1) {
   D_count_ch1 = 0;
 }
 if (D_count_ch2 >= D_full_ch2) {
   D_count_ch2 = 0;
 }

 //-----------SW reading---------------

 old_MD_ch1 = MD_ch1;//Fixed a bug that caused the divider clock to go out of sync when switching the switch.
 old_MD_ch2 = MD_ch2;//Fixed a bug that caused the divider clock to go out of sync when switching the switch.

 if ( mode_sw == 1  ) {
   mode = 1;
 }
 else if (mode_sw == 0  ) {
   mode = 2;
 }

 if ( old_mode != mode ) {//Fixed a bug that caused the divider clock to go out of sync when switching the switch.
   old_AD_MD = AD_MD;
 }

 //---------MD knob determination-----------------------

 if ( mode == 1 && abs(old_AD_MD - AD_MD ) > 30) { //abs is a bug fix for the divider clock going haywire when switching between switches
   old_AD_MD = 1200;//abs(old_AD_MD - AD_MD ) > Because 30 is absolutely true

   if ( AD_MD >= 0 && AD_MD < 20) {
     MD_ch1 = 9;//*16
   }

   else if ( AD_MD >= 20 && AD_MD < 90) {
     MD_ch1 = 8;//*8
   }

   else if ( AD_MD >= 90 && AD_MD < 240) {
     MD_ch1 = 7;//*4
   }

   else if ( AD_MD >= 240 && AD_MD < 400) {
     MD_ch1 = 6;//*2
   }

   else if ( AD_MD >= 400 && AD_MD < 550) {
     MD_ch1 = 5;//*1
   }

   else if ( AD_MD >= 550 && AD_MD < 700) {
     MD_ch1 = 4;//1/2
   }

   else if ( AD_MD >= 700 && AD_MD < 820) {
     MD_ch1 = 3;//1/3
   }

   else if ( AD_MD >= 820 && AD_MD < 960) {
     MD_ch1 = 2;//1/4
   }

   else if ( AD_MD >= 960 && AD_MD < 1024) {
     MD_ch1 = 1;//1/8
   }
 }

 else if ( mode == 2 && abs(old_AD_MD - AD_MD ) > 30) {  //abs is a bug fix for the divider clock going haywire when switching between switches
   old_AD_MD = 1200;//abs(old_AD_MD - AD_MD ) > Because 30 is absolutely true

   if ( AD_MD >= 0 && AD_MD < 20) {
     MD_ch2 = 9;//*16
   }

   else if ( AD_MD >= 20 && AD_MD < 90) {
     MD_ch2 = 8;//*8
   }

   else if ( AD_MD >= 90 && AD_MD < 240) {
     MD_ch2 = 7;//*4
   }

   else if ( AD_MD >= 240 && AD_MD < 400) {
     MD_ch2 = 6;//*2
   }

   else if ( AD_MD >= 400 && AD_MD < 550) {
     MD_ch2 = 5;//*1
   }

   else if ( AD_MD >= 550 && AD_MD < 700) {
     MD_ch2 = 4;//1/2
   }

   else if ( AD_MD >= 700 && AD_MD < 820) {
     MD_ch2 = 3;//1/3
   }

   else if ( AD_MD >= 820 && AD_MD < 960) {
     MD_ch2 = 2;//1/4
   }

   else if ( AD_MD >= 960 && AD_MD < 1024) {
     MD_ch2 = 1;//1/8
   }
 }


 switch (MD_ch1) {
   case 1:
     out_width_ch1 = ext_period / 2 / 8;
     M_period_ch1 = ext_period / 8;
     break;

   case 2:
     out_width_ch1 = ext_period / 2 / 4;
     M_period_ch1 = ext_period / 4;
     break;

   case 3:
     out_width_ch1 = ext_period / 2 / 3;
     M_period_ch1 = ext_period / 3;
     break;

   case 4:
     out_width_ch1 = ext_period / 2 / 2;
     M_period_ch1 = ext_period / 2;
     break;

   case 5:
     out_width_ch1 = ext_period / 2 ;
     M_period_ch1 = 0;
     D_full_ch1 = 1;
     break;

   case 6:
     out_width_ch1 = ext_period / 2;
     M_period_ch1 = 0;
     D_full_ch1 = 2;
     break;

   case 7:
     out_width_ch1 = ext_period / 2;
     M_period_ch1 = 0;
     D_full_ch1 = 4;
     break;

   case 8:
     out_width_ch1 = ext_period / 2;
     M_period_ch1 = 0;
     D_full_ch1 = 8;
     break;

   case 9:
     out_width_ch1 = ext_period / 2;
     M_period_ch1 = 0;
     D_full_ch1 = 16;
     break;
 }


 switch (MD_ch2) {
   case 1:
     out_width_ch2 = ext_period / 2 / 8;
     M_period_ch2 = ext_period / 8;
     break;

   case 2:
     out_width_ch2 = ext_period / 2 / 4;
     M_period_ch2 = ext_period / 4;
     break;

   case 3:
     out_width_ch2 = ext_period / 2 / 3;
     M_period_ch2 = ext_period / 3;
     break;

   case 4:
     out_width_ch2 = ext_period / 2 / 2;
     M_period_ch2 = ext_period / 2;
     break;

   case 5:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 1;
     break;

   case 6:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 2;
     break;

   case 7:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 4;
     break;

   case 8:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 8;
     break;

   case 9:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 16;
     break;
 }

 if ( MD_ch1 != old_MD_ch1 || MD_ch2 != old_MD_ch2 ) { //スイッチ切り替え時にディバイダーのクロックが狂うバグ対策
   D_count_ch1 = 0;
   D_count_ch2 = 0;
 }

 //------------External input presence/absence determination-------------------------------
 if ( ext_count > 4000 ) { //If there is no count for 4 seconds or more, it is determined that there is no external input.
   ext_injudge = 0;
 }
 else if ( ext_count < 4000 && ext_pulse == 1 ) {
   ext_injudge = 1;
 }

 if ( old_ext_injudge == 1 && ext_injudge == 0 ) { //When external input is present → absent
   ext_count = 0;
 }


 //---------Clock Settings------------------------
 if ( ext_injudge != 0 ) { //Use an external clock
   if (ext_pulse == 1 && old_ext_pulse == 0) {
     old_ext_count_result = ext_count_result;//To take the average of two
     ext_count_result = ext_count;
     //            ext_count = 0;
     ext_period = (old_ext_count_result + ext_count_result) / 2;//External input period. Average value of two times to reduce variation.
     //      MsTimer2::start();//When the external input becomes high, count to the next high
   }
 }
 else if ( ext_injudge == 0) { //Uses internal clock
   ext_period = rate;
   if ( ext_count < 5 || ext_count >= ext_period) {
     int_pulse = 1;
   }
   else if (ext_count >= 5 ) {
     int_pulse = 0;
   }

 }
 //--------------INTERNAL CLOCK output (only when there is no external input)-----------

 if ( int_pulse == 1 ) {
   digitalWrite(13, HIGH);
 }
 else if ( ext_count >= ext_period / 2  ) {
   digitalWrite(13, LOW);
 }

 //-----------------OUT1 output------------------
 if (ext_pulse == 1 && old_ext_pulse == 0) {//For external clock
   D_count_ch1 ++;
   ext_count = 0;
   M_count_ch1 = 0;
   M_done_ch1 = 0;
   if ( MD_ch1 <= 4 ) {
     digitalWrite(7, HIGH);
     CH1out = 1;
   }
 }

 if (int_pulse == 1 && old_int_pulse == 0) {//For internal clock
   D_count_ch1 ++;
   ext_count = 0;
   M_count_ch1 = 0;
   M_done_ch1 = 0;
   if ( MD_ch1 <= 4 ) {
     digitalWrite(7, HIGH);
     CH1out = 1;
   }
 }

 if ( MD_ch1 < 5 ) {
   if ( ext_count  >= M_period_ch1  * M_count_ch1  && CH1out == 0) {
     CH1out = 1;
     digitalWrite(7, HIGH);
   }
   if ( ext_count >= M_period_ch1  * M_count_ch1 + out_width_ch1  && CH1out == 1 ) {
     digitalWrite(7, LOW);
     M_count_ch1 ++;
     CH1out = 0;
   }
 }

 else if ( MD_ch1 == 5 ) {
   if (D_count_ch1 == 1 && M_done_ch1 == 0) {
     CH1out = 1;
     M_done_ch1 = 1;
     digitalWrite(7, HIGH);
   }
   if ( ext_count >=  out_width_ch1   ) {
     digitalWrite(7, LOW);
     CH1out = 0;
   }
 }

 else if ( MD_ch1 > 5 ) {
   if (D_count_ch1 == 1 && M_done_ch1 == 0) {
     CH1out = 1;
     digitalWrite(7, HIGH);
     M_done_ch1 = 1;
   }
   if ( ext_count >=  out_width_ch1   ) {
     digitalWrite(7, LOW);
     CH1out = 0;
   }
 }


 //-----------------OUT2 output------------------
 if (ext_pulse == 1 && old_ext_pulse == 0) {
   D_count_ch2 ++;
   M_count_ch2 = 0;
   M_done_ch2 = 0;
   if ( MD_ch2 <= 4 ) {
     digitalWrite(8, HIGH);
     CH2out = 1;
   }
 }

 if (int_pulse == 1 && old_int_pulse == 0) {//For internal clock
   D_count_ch2 ++;
   M_count_ch2 = 0;
   M_done_ch2 = 0;
   if ( MD_ch2 <= 4 ) {
     digitalWrite(8, HIGH);
     CH2out = 1;
   }
 }

 if ( MD_ch2 < 5 ) {
   if ( ext_count  >= M_period_ch2  * M_count_ch2  && CH2out == 0) {
     CH2out = 1;
     digitalWrite(8, HIGH);
   }
   if ( ext_count >= M_period_ch2  * M_count_ch2 + out_width_ch2  && CH2out == 1 ) {
     digitalWrite(8, LOW);
     M_count_ch2 ++;
     CH2out = 0;
   }
 }

 else if ( MD_ch2 == 5 ) {
   if (D_count_ch2 == 1 && M_done_ch2 == 0) {
     CH2out = 1;
     M_done_ch2 = 1;
     digitalWrite(8, HIGH);
   }
   if ( ext_count >=  out_width_ch2   ) {
     digitalWrite(8, LOW);
     CH2out = 0;
   }
 }

 else if ( MD_ch2 > 5 ) {
   if (D_count_ch2 == 1 && M_done_ch2 == 0) {
     CH2out = 1;
     M_done_ch2 = 1;
     digitalWrite(8, HIGH);
   }
   if ( ext_count >=  out_width_ch2   ) {
     digitalWrite(8, LOW);
     CH2out = 0;
   }
 }


 //For development

 Serial.print(ext_count);
 Serial.print(",");
 Serial.print(CH1out * 500);
 Serial.print(",");
 Serial.print(D_count_ch1 * 80);
 Serial.println("");

}

//Timer count. Increment the count every 1ms.
void timer_count() {
 ext_count ++;
}