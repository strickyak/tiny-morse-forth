// Copyright (c) 2020 Strick Yak / Henry Strickland.  MIT License.

#include <assert.h>
#include <stdio.h>

int Panic(const char* msg, int num) {
  fprintf(stderr, "\n****** PANIC: ``%s'' (%d)\n", msg, num);
  assert(0);
  return 0;
}
typedef unsigned char byte;

#define IS_DIGIT(C) ('0' <= (C) && (C) <= '9')
#define DIGIT_VALUE(C) ((C) - '0')

#define FIRST_CC 120
#define FIRST_BI 128

// Numbered Opcodes
byte NumberedChars[] = {
  'g', // Get global var
  'p', // Put global var
  'd', // Define user function
  'c', // Call user function
  'n', // literal Number
  'm', // More digit for literal number
};

enum NumberedEnum { nGet, nPut, nDef, nCall, nNum, nMore, };

enum BigraphEnum {
  // The bigraph opcodes on this first line have a 1-byte operand:
  bWh = FIRST_BI, bRe, bIf, bEl,
  // The rest do not.
#define FIRST_BI_NO_OPERAND bBe // The first bigraph opcode with no operand.
  bBe, bTh,
  bPl, bPn,
  bSt,
};

byte Bigraphs[] = {
  'w', 'h', // WHile
  'r', 'e', // REpeat
  'i', 'f', // IF
  'e', 'l', // ELse
  'b', 'e', // BEgin
  't', 'h', // THen
  'p', 'l', // PLus
  'p', 'n', // Print Number
  's', 't', // Stop
};

// Control Characters
enum ControlEnum { ccLF = FIRST_CC, ccBS, };

#define SS 16 // Stack Size
int Globals[10];
byte Dp;
int Ds[SS];  // Data stack & stack pointer
byte Rp;
int Rs[SS];  // Return stack & stack pointer
int Pc;

void Push(int x) {
  --Dp;
  if (Dp >= SS) Panic("Push Overflow", Dp);
  Ds[Dp] = x;
}
int Pop() {
  if (Dp >= SS) Panic("Pop Underflow", Dp);
  int z = Ds[Dp];
  Ds[Dp] = -98;
  ++Dp;
  return z;
}

void RPush(int x) {
  --Rp;
  if (Rp >= SS) Panic("RPush Overflow", Rp);
  Rs[Rp] = x;
}
int RPop() {
  if (Rp >= SS) Panic("RPop Underflow", Rp);
  int z = Rs[Rp];
  Rs[Rp] = -98;
  ++Rp;
  return z;
}


#define BS 50 // Buffer Size
byte Bp;
byte Buf[BS];        // Input buffer.
byte Progs[10][BS];  // Program storage.

void BufAdd(byte b) {
  Buf[Bp++] = b;
  if (Bp>=BS) Panic("input buffer overflow", Bp);
}

void DoGet(byte a) { Push(Globals[a]); }
void DoPut(byte a) { int b = Pop(); Globals[a] = b; }
void DoDef(byte a) { Panic("TODO Def", a); }
void DoCall(byte a) { Panic("TODO Call", a); }
void DoNum(byte a) { Push(a); }
void DoMore(byte a) { int b = Pop(); Push(b*10+a); }

byte GetOp(int pc) {
  if (pc<0 || pc>=11*BS) Panic("Bad pc", pc);
  // The first BS count is the Buffer.
  if (pc<BS) return Buf[pc];
  // The next 10 are in Progs.
  int prog = (pc/BS) - 1;
  int offset = pc%BS;
  return Progs[prog][offset];
}

bool Stop;

void Dump() {
  return;
  printf("\n\nDp=%d [", Dp);
  for (int i = 0; i < SS; i++) printf("%d ", Ds[i]);
  printf("] \nRp=%d [", Rp);
  for (int i = 0; i < SS; i++) printf("%d ", Rs[i]);
  printf("]\n Buf:");
  for (int i = 0; i < BS; i++) printf("%d ", Buf[i]);
  printf("\nStop: %d\n\n", Stop);
}

void Step() {
  byte code = GetOp(Pc++);
  if (Pc%BS == 0) Panic("Pc overflow", Pc);

  printf(" (Step[%d]=%d) ", Pc-1, code);

  // Numbered Ops:
  if (code < FIRST_CC) {
    int arg = code % 10;
    switch (code/10) {
      case nGet: DoGet(arg); return;
      case nPut: DoPut(arg); return;
      case nDef: DoDef(arg); return;
      case nCall: DoCall(arg); return;
      case nNum: DoNum(arg); return;
      case nMore: DoMore(arg); return;
      default: Panic("bad numbered op", code); return;
    }
  }
  // Bigraph Ops:
  switch (code) {
    case bPl: {
                int b=Pop(); int a=Pop(); Push(a+b);
              } return;
    case bPn: {
                int b=Pop(); printf("<%d> ", b);
              } return;
    case bSt: {
                Stop = true;
              } return;
    default: Panic("bad bigraph op", code); return;
  }
}

void onInput(byte code) {
  // Control Codes:
  switch (code) {
    case ccLF: {
                 BufAdd(bSt);
                 Stop = false;
                 Pc = 0;
                 do {
                   Step();
                   Dump();
                 } while (!Stop);
               }; return;
    case ccBS: {
                 if (Bp>0) {Buf[Bp--] = 255;}
               }; return;
  }

  BufAdd(code);
  if (FIRST_BI <= code && code < FIRST_BI_NO_OPERAND) {
    // These have an operands; reserve it with a 0.
    BufAdd(0);
  }
}

void Reset() {
  Pc = 0;
  Bp = 0;
  Dp = SS;
  Rp = SS;
  for (int i = 0; i < SS; i++) {
    Ds[i] = -99; Rs[i] = -99;
  }
  for (int i = 0; i < BS; i++) {
    Buf[i] = 252;
    for (int j = 0; j<10; j++) Progs[j][i] = 252;
  }
}

int main(int argc, char *argv[]) {
  Reset();
  onInput(nNum*10 + 3);
  Dump();
  onInput(nNum*10 + 5);
  Dump();
  onInput(bPl);
  Dump();
  onInput(bPn);
  Dump();
  onInput(ccLF);
  Dump();
  printf("*END*\n");
}
