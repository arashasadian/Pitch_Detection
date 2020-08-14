#include "stm32f4xx.h"

#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
//#include <stm32f4xxx_hal.h>


#define l_max 600
#define window_size 600
#define index_step1   14816.55
#define index_step	14816.65 
#define sample_rate 0x7
#define threshold 0
#define th_array_length 10





volatile int count = 0;
volatile uint64_t window[window_size];
volatile float freq_detected;
volatile int founded_l = 20;
volatile int sum[l_max];
float l_note_max = INT_MIN;
float l_note_min = INT_MAX;

struct pitch{
  char* note;
  float freq;
  
};
struct pitch notes[12];
float diffrent_ls[108];




//hal

/* String to draw on LCD */















//hal









void init(void);
void search_and_print_result(float finded_l);
int c_in = 0;

int auto_correlation(volatile uint64_t* window_array, int l, int window_array_size)
{
  int sum_result = 0;
  for(int window_i = 0; window_i < window_array_size-l; window_i++)
  {
    sum_result += window_array[window_i]* window_array[window_i + l];
  }
  return sum_result;
  
}
/*
void smoother(volatile uint64_t* arr, int arr_size) // smoothes $arr and saves on itself
{
  for(int i = 0; i < arr_size - 4; i++)
  {
    arr
  }
}
*/

void normalize(){
  int min = INT_MAX;
  int max = INT_MIN;
  for(int i = 0 ; i < window_size ; i++){
    if(window[i] > max){
      max = window[i];
    }
    if(window[i] < min){
      min = window[i];
    }
  }
  for(int i = 0 ; i < window_size ; i++){
    //window[i] = (window[i] - min) / (max - min);
    window[i] *= 100;
  }
}

void pda(){
 c_in ++;
 //normalize();
 int l_start[th_array_length] = {0,10,20,30,40,50,60,70,80,90};
 int max_sum_th[th_array_length] = {0,0,0,0,0,0,0,0,0,0};
 int max_index_occured_th[th_array_length] = {0,0,0,0,0,0,0,0,0,0};
 int founded_l = 0;
 /*
 for(int l = 0 ; l < l_max ; l++){
  sum[l] = auto_correlation(window,l,window_size);
 
 } */
 for (int i = 0 ; i < l_max;i++){
   sum[i] = 0;
   for(int j = i; j < window_size - i;j++){
     sum[i] += window[j - i] * window[j];
   }
 }

 int max1 = INT_MIN;
 int max2 = INT_MIN;
 int index1 = 0;
 int index2 = 0;
 /*
 for(int i = 0; i < window_size ; i++){
	 if(sum[i] > max1){
		 max2 = max1;
		 index2 = index1;
		 max1 = sum[i];
		 index1 = i;
	 while(sum[i+1] < sum[i] && i < window_size)
		 i++;
	 }
	 else if (sum[i] > max2){
		max2 = sum[i];
		index2 = i;
		while(sum[i+1] < sum[i] && i < window_size)
			i++;
	 }
 }*/
 
 for (int i = 0; i < l_max ; i++){
   if(sum[i] > max1){
     max1 = sum[i];
     index1 = i;
 }
   }
	
 
   
   for (int i = index1 + 1 ; i < l_max ; i++){
   if(sum[i] > max2){
     max2 = sum[i];
     index2 = i;
 }
   }
	
   //if(index2 == index1){
   search_and_print_result(abs(index2 - index1)*1000);
   /*
  GPIOA->ODR =  (int)max2<< 8;
  GPIOC->ODR = ((int)max2 >> 4) << 6;
  GPIOB->ODR = ((int)max2 >> 8 ) << 12;
  for(int i = 0; i < 10000000; i++);
  GPIOA->ODR =  (int)max1<< 8;
  GPIOC->ODR = ((int)max1 >> 4) << 6;
  GPIOB->ODR = ((int)max1 >> 8 ) << 12;
  for(int i = 0; i < 10000000; i++);
*/
 }  
  
  
  
  
void init_notes(){
  
  notes[0].note = "C";
  notes[0].freq = 16.35;
  
  notes[1].note = "C#";
  notes[1].freq = 17.32;
  
  notes[2].note = "D";
  notes[2].freq = 18.35;
  
  notes[3].note = "D#";
  notes[3].freq = 19.45;
  
  notes[4].note = "E";
  notes[4].freq = 20.60;
  
  notes[5].note = "F";
  notes[5].freq = 21.83;
  
  notes[6].note = "F#";
  notes[6].freq = 23.12;
  
  notes[7].note = "G";
  notes[7].freq = 24.50;
  
  notes[8].note = "G#";
  notes[8].freq = 25.96;
  
  notes[9].note = "A";
  notes[9].freq = 27.50;
  
  notes[10].note = "A#";
  notes[10].freq = 29.14;
  
  notes[11].note = "B";
  notes[11].freq = 30.87;
  
  
  
  for(int power = 0; power < 9; power++){

    for(int i = power * 12; i < (power + 1)*12; i++){
      float frequency = (notes[i % 12].freq) * pow(2, power);
      diffrent_ls[i] = index_step / frequency;
			diffrent_ls[i] *= 1000;
      /*
      if (diffrent_ls[i] > l_note_max)
        l_note_max = diffrent_ls[i];
      if(diffrent_ls[i] < l_note_min)
        l_note_min = diffrent_ls[i];
      
      GPIOA->ODR =  (int)diffrent_ls[i] << 8;
      GPIOC->ODR = ((int)diffrent_ls[i] >> 4) << 6;
      GPIOB->ODR = ((int)diffrent_ls[i]>> 8 ) << 12;
      for(int j = 0; j < 1000000;j++);
      */
    }
  }
  
  
  
}

  
  



int main(){
  init_notes();
  init();
  while(1){
  }
}
void init(){
  
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  
  //pin A1 for analog audio input
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  
  
  //ADC init channel 1 continues
  
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  
  ADC1->CR2  = 0;                /* Enable ADC1 */ 
  ADC1->SQR1 = 0;                /* conversion sequence length 1 */ 
  ADC1->SQR3 = 1;          /*channel 1*/
	
  ADC1->SMPR2 = sample_rate << 3;
  ADC1->CR1 |= ADC_CR1_EOCIE;
  
  

  
  
  
  
  //LCD INIT

  //GPIOA->MODER &= ~0x0c000000;
  //GPIOB->MODER &= ~0x00000280;
  //GPIOC->MODER &= ~0x00000000;
  //GPIOA->MODER |= 1 << 2 | 1 << 3| 1 << 10 | 1<<8 | 1 << 12 | 1<<14;
	GPIOA->MODER |= 0x5555555C;
  GPIOB->MODER |= 0x55555555;
  GPIOC->MODER |= 0x55555555;
	
  
  
  
  
  __enable_irq();
  NVIC_ClearPendingIRQ(ADC_IRQn);
  NVIC_SetPriority(ADC_IRQn, 0);
  NVIC_EnableIRQ(ADC_IRQn);
  
  ADC1->CR2 |= ADC_CR2_ADON;
  ADC1->CR2 |= ADC_CR2_SWSTART;
  
  
}



void ADC_IRQHandler(){
ADC1->SR = 0;
  //GPIOB->ODR = 0xffffffff;
  
  window[count] = ADC1->DR;
  /*GPIOA->ODR = count << 8;
  GPIOC->ODR = (count >> 4) << 6;
  GPIOB->ODR = (count >> 8 ) << 12;
  for(int i = 0; i< 100000;i++);*/
  
  count++;
  if (count == window_size){
    count = 0;
    //GPIOA->ODR = 0xeeeeeeee << 8;
    //GPIOC->ODR = (0xeeeeeeee >> 4) << 6;
    //GPIOB->ODR = (0xeeeeeeee >> 8 ) << 12;
    //GPIOB->ODR = 0xffffffff;
    //for(int i = 0; i< 1000;i++);
    //normalize();
    pda();
  }
  ADC1->CR2 |= ADC_CR2_SWSTART;
  
}

void search_and_print_result(float finded_l){
  /*
  GPIOA->ODR =   0xffffffff << 8;
  GPIOC->ODR = (0xffffffff >> 4) << 6;
  GPIOB->ODR = (0xffffffff >> 8 ) << 12; 
  */
  float min_sub = INT_MAX;
  int min_index = 0;
  for(int i = 0; i < 108;i++){
    float sub = fabs(finded_l - diffrent_ls[i]);
    if (sub < min_sub){
      min_sub = sub;
      min_index = i;
    }
  }
  int freq_index_start = min_index / 12;
  int freq_offset = min_index % 12;
  int notechar = 0;
  switch (notes[freq_offset].note[0]){
    case 'A':notechar = 0xAAAAAAAA;  break;
    case 'B':notechar = 0xBBBBBBBB;  break;
    case 'C':notechar = 0xCCCCCCCC;  break;
    case 'D':notechar = 0xDDDDDDDD;  break;
    case 'E':notechar = 0xEEEEEEEE;  break;
    case 'F':notechar = 0xFFFFFFFF;  break;
    case 'G':notechar = 0x0;  break;
    default:notechar = 0x1;    break;
  }
  min_sub *= pow(2, 16);
  int sharp = 0;
  if (strlen(notes[freq_offset].note) == 2){
    sharp = 0xFFFFFFFF;
  }
  int freq_to_print = 0;
  
  for(int i = 0 ; i < 8; i++){
    freq_to_print |= freq_index_start << (i * 4);
  }
  /*
  GPIOA->ODR =  (int)finded_l<< 8;
  GPIOC->ODR = ((int)finded_l >> 4) << 6;
  GPIOB->ODR = ((int)finded_l >> 8 ) << 12;
	*/
  
  
  GPIOA->ODR =  sharp<< 8;
  GPIOC->ODR = ((freq_to_print) >> 4) << 6;
  GPIOB->ODR = (notechar >> 8 ) << 12; 
  
}