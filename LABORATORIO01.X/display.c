/*
 * File:   display.c
 * Author: carlo
 *
 * Created on 20 de enero de 2023, 02:36 AM
 */

#include "display.h"
#include <stdint.h>

uint8_t unidades;
uint8_t decenas;
uint8_t display_sample1;
uint8_t display_sample2;

uint8_t tabla[16] = {0b00111111,   //0
                     0b00000110,   //1
                     0b01011011,   //2
                     0b01001111,   //3
                     0b01100110,   //4
                     0b01101101,   //5
                     0b01111101,   //6
                     0b00000111,   //7
                     0b01111111,   //8
                     0b01101111,   //9
                     0b01110111,   //10
                     0b01111100,   //11
                     0b00111001,   //12
                     0b01011110,   //13
                     0b01111001,   //14
                     0b01110001};  //15

//******************************************************************************
// Función de Lectura de ADC
//******************************************************************************
unsigned int writeDisplayDecenas(unsigned int valorDec){
    decenas = (uint8_t) (valorDec/16);  
    display_sample1 = tabla[decenas];
    return display_sample1;
}

unsigned int writeDisplayUnidades (unsigned int valorUni){
     unidades = (uint8_t) (valorUni%16);
     display_sample2 = tabla[unidades];
     return display_sample2;
}
