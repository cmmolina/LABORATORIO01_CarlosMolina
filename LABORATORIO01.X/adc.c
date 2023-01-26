/*
 * File:   adc.c
 * Author: carlo
 *
 * Created on 19 de enero de 2023, 11:43 PM
 */

#include "adc.h"

//#define _XTAL_FREQ 4000000
float ADC_Voltaje;

//******************************************************************************
// Función de Lectura de ADC
//******************************************************************************
int readADC(void){
    ADC_Voltaje = ADRESH;
    //__delay_us(100);   
    return ADC_Voltaje;
}

//******************************************************************************
// Función de Conversión 
//******************************************************************************
unsigned int map(uint8_t value, int inputmin, 
                  int inputmax, int outmin, int outmax){
    return ((value - inputmin)*(outmax-outmin)) / ((inputmax-inputmin)+outmin);
}