/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DISPLAY_H
#define	DISPLAY_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
// TODO Insert appropriate #include <>


// Prototipo de función
unsigned int writeDisplayDecenas(unsigned int valorDec);
unsigned int writeDisplayUnidades (unsigned int ValorUni);

#endif	/* XC_HEADER_TEMPLATE_H */
