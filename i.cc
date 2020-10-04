// i.cc -- input (for unix)

#include <stdio.h>
#include <cstring>

#include "defs.h"

bool UnixInput() {
  memset(Buf, 0, sizeof Buf);
  fflush(stdout);
  fflush(stderr);
  fprintf(stderr, " ok ");
  fflush(stderr);

  char inbuf[1000];
  memset(inbuf, 0, sizeof inbuf);
  char* p = fgets(inbuf, sizeof inbuf, stdin); 
  if (!p) {                 // if EOF
    return false;
  }

  bool odd = false;
  byte first;
  byte code;
  for (; *p; p++) {
    if (*p <= ' ') continue;  // skip white
    odd = !odd;
    if (odd) {
      first = *p;
      continue;
    }
    byte second = *p;

    if (IS_DIGIT(second)) {
      int i = 0;
      for (byte it : NumberedChars) {
        if (it == first) {
          code = 10*i + DIGIT_VALUE(second);
          goto GOT_code;
        }
        i++;
      }
      Panic("unknown NumberedChar prefix", first);
    } else {
      int i = 0;
      for (const auto& it: bigraph_info) {
        if (first==it.a && second==it.b) {
          code = FIRST_BI + i;
          goto GOT_code;
        }
        i++;
      }
      Panic("unknown bigraph", 256*first + second);
    }

GOT_code:
    OnInput(code);
  }  // next p
  OnInput(ccLF);
  return true;
}
