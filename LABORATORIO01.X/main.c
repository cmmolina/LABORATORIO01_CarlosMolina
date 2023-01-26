/*
 * Universidad del Valle de Guatemala
 * Electrónica Digital 2
 * Carlos Mauricio Molina López (#21253)
 * PRELABORATORIO 01 - Interrupciones y Uso de Librerías
 * Created on 19 de enero de 2023, 11:05 PM
 */

//******************************************************************************
// Palabra de Configuración
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#include <stdint.h>
#include "adc.h"
#include "display.h"

#define _XTAL_FREQ 500000
#define tmr0_value 179

//******************************************************************************
// Variables 
//******************************************************************************
int ADC_Value;
unsigned int i;

//******************************************************************************
// Prototipos de Funciones
//******************************************************************************
void setup(void);
void setupADC(void);
void comparacion(void);

//******************************************************************************
// Interrupción
//******************************************************************************
void __interrupt() isr (void){    
    
    //Interrupción de Envío
    if (PIR1bits.TXIF){
        PIR1bits.TXIF = 0;
    }
    
    //Interrupción de Recepción
    if (PIR1bits.RCIF){
        PIR1bits.RCIF = 0;
    }
    
    //Interrupción del ADC cuando la lectura termina
    if (PIR1bits.ADIF){ 
        ADC_Value = readADC();
        PIR1bits.ADIF=0; 
    }
    
    //Interrupción del TMR0 (PWM Manual)
    if (INTCONbits.T0IF){
        
        if (i==1){
            PORTEbits.RE1=0;
            PORTD = writeDisplayDecenas(ADC_Value);
            PORTEbits.RE0=1; 
            i=0;
        }
        
        else if (i==0){
            PORTEbits.RE0=0; 
            PORTD = writeDisplayUnidades(ADC_Value);
            PORTEbits.RE1=1; 
            i=1;
        }
        /*
        PORTD = writeDisplayDecenas(ADC_Value);
        PORTD = writeDisplayUnidades(ADC_Value);
        */
                
        TMR0 = tmr0_value;          // Cargamos 5ms de nuevo al TMR0
        INTCONbits.T0IF = 0;
    }
    
    //Interrupción del Puerto B 
    if (INTCONbits.RBIF){ 
        if (PORTBbits.RB0 == 1){
            while (PORTBbits.RB0 ==1){
                ;
            }
            PORTC++;
        }
        else if (PORTBbits.RB1 == 1){
            while (PORTBbits.RB1 == 1){
                ;
            }
            PORTC--; 
        }   
        INTCONbits.RBIF = 0;
    }
}

//******************************************************************************
// Código Principal 
//******************************************************************************
void main(void) {
    setup();
    setupADC();
    i=1; 
    
    //Loop Principal
    while(1){
        
        comparacion();
        
        //--Lectura Canal AN0
        ADCON0bits.CHS = 0b0000;
        __delay_us(100);
        ADCON0bits.GO = 1;
        
        
    }
}

//******************************************************************************
//Funciones
//******************************************************************************

void setup(void){
    //Configuración de I/O 
    
    ANSEL = 0b00000001;             // RA0 analógico
    ANSELH = 0; 

            //76543210
    TRISA = 0b00000000;             // 
    TRISB = 0b00000011;             // RB0, RB1 como inputs
    TRISC = 0b00000000;             // 
    TRISD = 0b00000000;             //
    TRISE = 0b00000000;             // 
    
    PORTA = 0b00000000; 
    PORTB = 0b00000000; 
    PORTC = 0b00000000; 
    PORTD = 0b00000000; 
    PORTE = 0b00000000;
    
    //Configuración del Oscilador
    OSCCONbits.IRCF = 0b011;        // 500kHz
    OSCCONbits.SCS = 1;             // Oscilador Interno
    
     
    /*Guía para interrupción/config. del Puerto B
    IOCBbits.IOCB7 = 1;             // RB7 con Interrupción
    WPUBbits.WPUB7 = 0;             // Pull-up enabled
    INTCONbits.RBIE = 1;            // Se habilitan las interrupciones del Puerto B
    INTCONbits.RBIF = 0;            // Flag del Puerto B en 0
    */
    
    //Configuración del Puerto B 
    IOCB = 0b00000011;              // Pines de Puerto B con Interrupción
    OPTION_REGbits.nRBPU = 0;       // Pull-Up/Pull-Down
    INTCONbits.RBIE = 1;            // Se habilitan las interrupciones del Puerto B
    
    //Configuración de las Interrupciones
    INTCONbits.GIE = 1;             // Se habilitan las interrupciones globales
    //INTCONbits.PEIE = 1;
    
    PIE1bits.ADIE = 1;              // Se habilita la interrupción del ADC
    INTCONbits.TMR0IE = 1;          // Se habilitan las interrupciones del TMR0    
    
    PIR1bits.ADIF = 0;              // Flag de ADC en 0
    INTCONbits.RBIF = 0;            // Flag de Interrupciones del Puerto B en 0

    
    //Configuración del TMR0
    OPTION_REGbits.T0CS = 0;        // Fosc/4
    OPTION_REGbits.PSA = 0;         // Prescaler para el TMR0
    OPTION_REGbits.PS = 0b011;      // Prescaler 1:16
    TMR0 = tmr0_value;              // Asignamos valor al TMR0 para 4ms
    INTCONbits.T0IF = 0;            // Flag de TMR0 en 0
}

void setupADC(void){
    //Módulo de ADC
    ADCON0bits.ADCS = 0b01;         // Fosc/8
    
    ADCON1bits.VCFG1 = 0;           // Voltaje de Referencia + - VSS
    ADCON1bits.VCFG0 = 0;           // Voltaje de Referencia - - VDD
    
    //Formato de Resultado 
    ADCON1bits.ADFM = 0;            // Justificado a la Izquierda
    
    //Canal
    ADCON0bits.CHS = 0b0000;        // Canal AN0 (Para empezar)
    
    ADCON0bits.ADON = 1;            // Se habilita el ADC
    
    PIR1bits.ADIF = 0;              // Apagamos la bandera
    
    //Delay (Ejemplo)
    __delay_us(100);
}

void comparacion(void){
    if (ADC_Value>PORTD){
        PORTEbits.RE2=1; 
    }
    
    else if(ADC_Value<=PORTD){
        PORTEbits.RE2=0;
    }
}