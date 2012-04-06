/*
 * RTTY.cpp
 *
 * Version 1
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

 * RTTY USB Baud 50/300 XOR checksum code
 *
 * Borrowed from Project Horus (juxta)
 * This code is based on Robert Harrison's RTTY code as used in the Icarus project
 * http://pegasushabproject.org.uk/wiki/doku.php/ideas:notes?s=rtty
 * and based upon the initial work from TimZaman
 */

#include "RTTY.h"
// config for mark & space pins, ASCII mode, num stop bits & transmission counts
byte _RTTY_PIN_1=0,_RTTY_PIN_2=0;
int _RTTY_ASCII=0,_RTTY_STOP=0,_RTTY_BAUD,_txCount=0;

RTTY::RTTY(byte RTTY_mark,byte RTTY_space,int RTTY_ascii,int RTTY_stop,int RTTY_baud)
{
_RTTY_PIN_1=RTTY_mark;
_RTTY_PIN_2=RTTY_space;
_RTTY_ASCII=RTTY_ascii;
_RTTY_STOP=RTTY_stop;
_RTTY_BAUD=RTTY_baud;
}

int RTTY::txCount(){
  return _txCount;
}
// Transmit a string & produce debug output
void RTTY::TxString(char *string) {
  noInterrupts();
  // Checksum
  char txSum[4];
  int checkSum = getCheckSum(string);
  sprintf(txSum, "%02X", checkSum);
  
  Serial.print("RTTY: ");Serial.print(string);Serial.print("*");Serial.println(txSum);

  rtty_txstring(string);
  rtty_txstring("*");
  rtty_txstring(txSum);
  rtty_txstring("\r\n");
  _txCount++;

  interrupts();
}//end TxString

// Transmit a string, one char at a time
void RTTY::rtty_txstring (char *string) {
  for (int i = 0; i < strlen(string); i++) {
    rtty_txbyte(string[i]);
  }
}//end rtty_txstring

// Transmit a byte, bit by bit, LSB first
// ASCII_BIT can be either 7bit or 8bit
void RTTY::rtty_txbyte (char c) {
  int i;
  // Start bit
  rtty_txbit (0);
  // Send bits for for char LSB first
  for (i=0;i<_RTTY_ASCII;i++) {
    if (c & 1) rtty_txbit(1);
    else rtty_txbit(0);
    c = c >> 1;
  }
  // Stop bit(s)
  rtty_txbit(1);
  if (_RTTY_STOP==2)
    rtty_txbit(1);
}//end rtty_txbyte

// Transmit a bit as a mark or space
void RTTY::rtty_txbit (int bit) {
  if (bit) {
    // High - mark
    digitalWrite(_RTTY_PIN_1, HIGH);
    digitalWrite(_RTTY_PIN_2, LOW);
  } else {
    // Low - space
    digitalWrite(_RTTY_PIN_2, HIGH);
    digitalWrite(_RTTY_PIN_1, LOW);
  }
  if(_RTTY_BAUD==300){
    delayMicroseconds(3370); // 300 baud
  }
  else{
    delayMicroseconds(10000); // For 50 Baud uncomment this and the line below.
    delayMicroseconds(10150); // For some reason you can't do 20150 it just doesn't work.
  }
}//end rtty_txbit
  
int RTTY::getCheckSum(char *string) {
  int i; int XOR; int c;
  // Calculate checksum ignoring any $'s in the string
  for (XOR = 0, i = 0; i < strlen(string); i++) {
    c = (unsigned char)string[i];
    if (c == '*') break;
    if (c != '$') XOR ^= c;
  }
  return XOR;
}//end getchecksum

