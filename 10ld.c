#include<msp430g2202.h>

char i;

unsigned char stundas = 0, minutes = 0, sekundes = 0;

unsigned char mode = 0, submode = 0;

unsigned char poga4 = 0;

unsigned char alarm_minutes = 0, alarm_hours = 0, alarm_on = 0;

void display(unsigned char digit);
void delay(unsigned long ms);
void indication(unsigned char digit12, unsigned char digit34, unsigned char which_digit);
void time_engine(unsigned char time_instant_in_seconds);
void digit_blink(unsigned char digit_position, unsigned char blink_position1, unsigned char blink_position2, unsigned int h2);
void long_press(void);
void short_press(void);
void indication2(unsigned char digit1, unsigned char digit2,unsigned char digit3,unsigned char digit4, unsigned char which_digit);

int main(void)
{
 WDTCTL = WDTPW + WDTHOLD;
 TACTL = TASSEL_2+MC_1+ID_0+TACLR;
 TACCTL0 = CCIE;
 CCR0 = 32768-1;
 BCSCTL2 = SELM_1+DIVM_1+SELS+DIVS_0;
 BCSCTL1 &= ~XTS;
 BCSCTL3 = (XT2S_0+XCAP_3);
 BCSCTL1 = CALBC1_12MHZ; //SET TIMER REQ TO 12MHZ
 DCOCTL = CALDCO_12MHZ; //-||-

 P2DIR &= ~0x30;
 P2IES |= 0x30;
 P2IFG &= ~0x30;
 P2IE |= 0x30;

  P2DIR |= 0xC;
   P2OUT |= 0xC; 
 __enable_interrupt();
 P1DIR |= 0xFF;
 P2DIR |= 0xF;
 P2OUT &= ~0xF;
 P2OUT |= 0x1;

 unsigned char j =0; //unsigned char veertiibas var buut no 0 lidz 255
 unsigned char digit_pos = 0;
 minutes =0;
 stundas =0;
 sekundes =0;
 for(unsigned int h = 0;;h++)
 {
  if(h==1000){
    h=0; 
  } 
  digit_pos = (digit_pos+1)%4;
  if (mode == 0)
  {
        
  indication(minutes,sekundes,digit_pos);

  if(h>500&&digit_pos==2)
  {
    P1OUT ^= 0b1000;
  }
  }else if(mode == 1)
  {
    indication(stundas, minutes,digit_pos);
    if(h>500&&digit_pos==2)
  {
    P1OUT ^= 0b1000;
  }
  }else if(mode ==2)
  {
    indication(stundas,minutes,digit_pos);
    if (submode == 0) digit_blink(digit_pos,0,1,h);
    if(submode ==1) digit_blink(digit_pos,2,3,h);
    if((h<500)&&(digit_pos ==2)) P1OUT |= 0x8;
  }else if(mode == 3 && submode==0 && alarm_on ==1)
  {
    indication2(0xA,0xB,0xC,0xD,digit_pos);
  }else if(mode == 3 &&submode==0 && alarm_on ==0)
  {
    indication2(0xA,0xB,0xC,0xE,digit_pos);
  }  
  else if(mode ==3)
  {
    indication(alarm_hours,alarm_minutes,digit_pos);
    if(submode == 1) digit_blink(digit_pos,0,1,h);
    if(submode ==2) digit_blink(digit_pos,2,3,h);
  }
  if(alarm_minutes == minutes && alarm_hours == stundas && alarm_on ==1)
  {
     play(1,1);
     play(1,2);
     play(3,2);
     play(1,1);
     play(1,4);
     play(3,2);
     play(3,4);
     play(1,1);
     play(2,4);
     play(3,2);
     play(1,2);
     play(1,1);

     
  }
  if(alarm_minutes == minutes && alarm_hours == stundas)
  {
    alarm_on =0;
  }
  delay(100);
 }
}

void display(unsigned char digit)
{
  if(digit == 0) P1OUT = 0xF5;

  if(digit == 1) P1OUT = 0x05;

  if(digit == 2) P1OUT = 0x73;

  if(digit == 3) P1OUT = 0x57;

  if(digit == 4) P1OUT = 0x87;

  if(digit == 5) P1OUT = 0xD6;

  if(digit == 6) P1OUT = 0xF6;

  if(digit == 7) P1OUT = 0x45;
  
  if(digit == 8) P1OUT = 0xF7;

  if(digit == 9) P1OUT = 0xD7;

  if(digit == 0xA) P1OUT = 0b11100111;

  if(digit == 0xB) P1OUT = 0b10110000;

  if(digit == 0xC) P1OUT = 0b00110110;

  if(digit == 0xD) P1OUT = 0b00100110;

  if(digit == 0xE) P1OUT = 0b11100010;

  if(digit == 0xF) P1OUT = 0x0;
  
}

void delay(unsigned long ms)
{
  unsigned long ims;
  for(ims = 0; ims<ms;ims++)
  {
    __delay_cycles(120);
  }
}

void indication(unsigned char digit12, unsigned char digit34, unsigned char which_digit)
{
  P2OUT &= ~0xF;
  if(which_digit == 0) 
  {
    P2OUT = 0b1000;
    display(digit34%10);
  }
  
  if(which_digit == 1)
  {
    P2OUT = 0b100;
    display(digit34/10);
  }
  if(which_digit == 2)
  {
    P2OUT = 0b10;
    display(digit12%10);

  }
  if(which_digit == 3) 
  {
    P2OUT = 0b1;
    display(digit12/10);
  }
}
void indication2(unsigned char digit1, unsigned char digit2,unsigned char digit3,unsigned char digit4, unsigned char which_digit)
{
  P2OUT &= ~0xF;
  if(which_digit == 0) 
  {
    P2OUT = 0b1000;
    display(digit4);
  }
  
  if(which_digit == 1)
  {
    P2OUT = 0b100;
    display(digit3);
  }
  if(which_digit == 2)
  {
    P2OUT = 0b10;
    display(digit2);

  }
  if(which_digit == 3) 
  {
    P2OUT = 0b1;
    display(digit1);
  }
}
void play(int n, int e)
{
   unsigned int del;
   unsigned long laiks;
   volatile unsigned long i;   
   volatile unsigned long j;

   if(n == 1) del = 47;
   if(n == 2) del = 42;
   if(n == 3) del = 37;
   if(n == 4) del = 35;
   if(n == 5) del = 31;
   if(n == 6) del = 28;
   if(n == 7) del = 25;

   if(e == 1) laiks = 100000;
   if(e == 2) laiks = 50000;
   if(e == 4) laiks = 25000;
   if(e == 8) laiks = 12500;
   if(e == 16) laiks = 6250;

   for(i=0; i<laiks; i=i+del+del)
   {
    delay(del);
    P2OUT |= 0xC;
    delay(del);
    P2OUT &= ~0xC; 
   }
   
}

void time_engine(unsigned char time_instant_in_seconds)
{
  sekundes += time_instant_in_seconds;

  if(sekundes == 60)
  {
    sekundes=0;minutes++;
  }

  if(minutes == 60)
  {
    minutes=0;stundas++;
  }

  if(stundas ==24)
  {
    stundas=0;sekundes=0;minutes=0;
  }
}

void digit_blink(unsigned char digit_position, unsigned char blink_position1, unsigned char blink_position2, unsigned int h2)
{
  if(h2>500&&digit_position == blink_position1)
  {
    P2OUT &= (0x01<<digit_position);
  }
  if(h2>500&&digit_position == blink_position2)
  {
    P2OUT &= (0x01<<digit_position);
  }
}

void long_press(void)
{
  submode++;
  if(submode>3) submode=0;
}
void short_press(void)
{
  if(mode==2)
  {
    if(submode==0)
    {
      minutes++;
      if(minutes>59) minutes = 0;
    }
    if(submode == 1)
    {
      stundas++;
      if(stundas>23) stundas=0;
    }
  }
  if(mode ==3)
  {
    if(submode == 0)
    {
      alarm_on = (alarm_on+1)%2;
    }
    if(submode == 1)
    {
      alarm_hours++;
      if(alarm_hours >23) alarm_hours =0;
    }
    if(submode == 2)
    {
      alarm_minutes++;
      if(alarm_minutes>59) alarm_minutes =0;
    }
  }
}

#pragma vector = PORT2_VECTOR

__interrupt void P2_ISR(void)
{
 if((P2IFG & BIT5) == BIT5)
 {
  P2IFG &= ~BIT5; 
  mode++;
  if(mode>3) mode=0;
 }

 if((P2IFG & BIT4) == BIT4)
 {
  P2IFG &= ~BIT4;
  if((P2IES%BIT4)==BIT4)
  {
    P2IES &= ~BIT4;
    poga4 = sekundes;
  }else
  {
    P2IES |= BIT4;
    if(sekundes - poga4>1)
    {
      long_press();
    }else
    {
      short_press();
    }
  }
 }
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) //divi undescori!!!
{
 time_engine(1);
} 
