#ifndef DEFS_H_

typedef unsigned char byte;

#define IS_DIGIT(C) ('0' <= (C) && (C) <= '9')
#define DIGIT_VALUE(C) ((C) - '0')

#define FIRST_CC 120
#define FIRST_BI 128

// Numbered Opcodes
inline constexpr byte NumberedChars[] = {
  '?', // UNUSED
  'g', // Get global var
  'p', // Put global var
  'd', // Define user function
  'c', // Call user function
  'n', // literal Number
  'm', // More digit for literal number
};

enum NumberedEnum { n____, nGet, nPut, nDef, nCall, nNum, nMore, };


// Control Characters
enum ControlEnum { ccLF = FIRST_CC, ccBS, };

#define SS 16 // Stack Size
extern int Globals[10];
extern byte Dp;
extern int Ds[SS];  // Data stack & stack pointer
extern byte Rp;
extern int Rs[SS];  // Return stack & stack pointer
extern int Pc;

#define BS 50 // Buffer Size
extern byte Bp;
extern byte Buf[BS];        // Input buffer.
extern byte Progs[10][BS];  // Program storage.


#define OPCODE(a, b, t, s) t,

enum BigraphEnum {
  t_______ = 127,  // first bigraph will be at 128
#include "ops.h"
};

#undef OPCODE
#define OPCODE(a, b, t, s) {a, b, s},

inline constexpr struct BigraphInfo {
  char a, b;
  const char* s;
} bigraph_info[] = {  // index offset 128
#include "ops.h"
};

int Panic(const char* msg, int num);
void BufAdd(byte b);
bool UnixInput();
void OnInput(byte code);
byte GetOp(int pc);

#define DEFS_H_
#endif
