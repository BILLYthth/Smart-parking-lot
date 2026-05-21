#include "C:\Vu Thanh Dat\Study textbooks and lectures\Microprocessor\final program\code\refer\final.h"

#define rs  PIN_E0
#define rw  PIN_E1
#define en  PIN_E2

#define rs1 PIN_A5
#define rw1 PIN_A6
#define en1 PIN_A7

#define trg1 PIN_C0
#define trg2 PIN_C1
#define trg3 PIN_C2
#define trg4 PIN_C3

#define echo1 PIN_C4
#define echo2 PIN_C5
#define echo3 PIN_C6
#define echo4 PIN_C7

#define buzzer PIN_A0

unsigned int16 time1 = 0, time2 = 0, time3 = 0, time4 = 0;
int slot1 = 0, slot2 = 0, slot3 = 0, slot4 = 0;
int16 cnt;
int overflow_count = 0;
int minutes = 0;
void lcd1_command(unsigned int8 cmd) {
   output_low(rs);
   output_low(rw);
   output_d(cmd);
   output_low(en);
   delay_us(1);
   output_high(en);
   delay_ms(2);
   output_low(en);
}

void lcd1_data(unsigned int8 data) {
   output_high(rs);
   output_low(rw);
   output_d(data);
   output_low(en);
   delay_us(1);
   output_high(en);
   delay_ms(2);
   output_low(en);
}

void lcd2_command(unsigned int8 cmd) {
   output_low(rs1);
   output_low(rw1);
   output_d(cmd);
   output_low(en1);
   delay_us(1);
   output_high(en1);
   delay_ms(2);
   output_low(en1);
}

void lcd2_data(unsigned int8 data) {
   output_high(rs1);
   output_low(rw1);
   output_d(data);
   output_low(en1);
   delay_us(1);
   output_high(en1);
   delay_ms(2);
   output_low(en1);
}

void lcd1_init() {
   lcd1_command(0x38);
   lcd1_command(0x0C);
   lcd1_command(0x06);
   lcd1_command(0x01);
   delay_ms(5);
}

void lcd2_init() {
   lcd2_command(0x38);
   lcd2_command(0x0C);
   lcd2_command(0x06);
   lcd2_command(0x01);
   delay_ms(5);
}

// Display minutes as "Time: XXm"
void display_minutes(int min) {
   lcd2_command(0x80);
   lcd2_data('T');
   lcd2_data('i');
   lcd2_data('m');
   lcd2_data('e');
   lcd2_data(':');
   lcd2_data(' ');
   lcd2_data((min / 10) + '0'); // Tens
   lcd2_data((min % 10) + '0'); // Units
   lcd2_data('m');
   lcd2_data(' '); // Clear any leftover char
}

unsigned int16 measure_distance(pin_trigger, pin_echo) {
   unsigned int16 duration;
   output_low(pin_trigger);
   delay_us(2);
   output_high(pin_trigger);
   delay_us(10);
   output_low(pin_trigger);
   while (!input(pin_echo));
   set_timer1(0);
   while (input(pin_echo));
   int count = get_timer1();
   return (count*1.6)/58;
}

void main() {
   set_tris_d(0x00);
   set_tris_b(0x00);
   set_tris_e(0x00);
   set_tris_a(0x00);
   set_tris_c(0xF0); // RC0-3 output, RC4-7 input
   output_low(buzzer);

   lcd1_init();
   lcd2_init();

   setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
   set_timer1(0);

   while (TRUE) {
      
      unsigned int16 d1 = measure_distance(trg1, echo1);
      unsigned int16 d2 = measure_distance(trg2, echo2);
      unsigned int16 d3 = measure_distance(trg3, echo3);
      unsigned int16 d4 = measure_distance(trg4, echo4);
      
      if(d1 <20)slot1= 1;
      if(d2 <20) slot2=1;
      if(d3 >20) slot3=1;
      else if (d3<10) {lcd1_command(0xc0+05); lcd1_data("tooclose");}
      if(d4 <20) slot4=1;
      
      if (slot1) time1++; else time1 = 0;
      if (slot2) time2++; else time2 = 0;
      if (slot3) time3++; else time3= 0;
      if (slot4) time4++; else time4 = 0;

      int8 a = 4 - slot1 - slot2 - slot3 - slot4;

      lcd1_command(0x80);
      lcd1_data("Slots: ");
     
      lcd1_data(a + '0');
       // 
       


      

      delay_ms(60000); // Delay 1 phút
   }
}

