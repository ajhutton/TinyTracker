/*
 * RTTY.h
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

#ifndef RTTY_h
#define RTTY_h

#include "WProgram.h"

class RTTY
{
  public:
    RTTY(byte RTTY_mark,byte RTTY_space,int RTTY_ascii,int RTTY_stop, int RTTY_baud);
    void TxString(char *string);
    int txCount();
  private:
    byte _RTTY_PIN_1,_RTTY_PIN_2;
    int _RTTY_ASCII,_RTTY_STOP, _RTTY_baud;
    void rtty_txstring (char *string);
    void rtty_txbyte (char c);
    void rtty_txbit (int bit);
    int getCheckSum(char *string);
};

#endif
