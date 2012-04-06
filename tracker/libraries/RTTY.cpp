/*
 * RTTY.cpp
 * Version 0.1 - http://www.timzaman.com/
 * Copyright (c) 2011 Tim Zaman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Code based in part on information from the following:
 * [J. Coxon, D.Richman]
 */
 
#include "RTTY.h"
#define ASCII_BIT 7
#define BAUD_RATE 20150     // 10000 = 100 BAUD 20150
//Dont mind these two, they'll be overwritten.
int pa=2;
int pb=3;
/* PUBLIC METHODS */

RTTY5::RTTY5()
{
}

void RTTY5::attach(int pina, int pinb)
{
	pa=pina;
	pb=pinb;
	pinMode(pa, OUTPUT);
	pinMode(pb, OUTPUT);

}


void RTTY5::tx(char * string)
{
	/* Simple function to sent a char at a time to 
	** rtty_txbyte function. 
	** NB Each char is one byte (8 Bits)
	*/

	char c;

	c = *string++;

	while ( c != '\0')
	{
		rtty_txbyte (c);
		c = *string++;
	}
}


void RTTY5::rtty_txbyte (char c)
{
	int i;

	rtty_txbit (0); // Start bit

	// Send bits for for char LSB first	

	for (i=0;i<ASCII_BIT;i++)
	{
		if (c & 1) rtty_txbit(1); 

			else rtty_txbit(0);	

		c = c >> 1;

	}

	rtty_txbit (1); // Stop bit
}

void RTTY5::rtty_txbit (int bit)
{
		if (bit)
		{
		  // high
                    digitalWrite(pa, HIGH);
                    digitalWrite(pb, LOW);
                    
		}
		else
		{
		  // low
                    digitalWrite(pb, HIGH);
                    digitalWrite(pa, LOW);
		}
		delayMicroseconds(BAUD_RATE); 
}












// Set the default object
RTTY5 RTTY = RTTY5();
