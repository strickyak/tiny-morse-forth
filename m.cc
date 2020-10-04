// Copyright (c) 2020 Strick Yak / Henry Strickland.  MIT License.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "defs.h"

int Globals[10];
byte Dp;
int Ds[SS];  // Data stack & stack pointer
byte Rp;
int Rs[SS];  // Return stack & stack pointer
int Pc;

byte Bp;
byte Buf[BS];        // Input buffer.
byte Progs[10][BS];  // Program storage.

void Push(int x) {
  --Dp;
  if (Dp < 0) Panic("Push Overflow", Dp);
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
  if (Rp < 0) Panic("RPush Overflow", Rp);
  Rs[Rp] = x;
}
int RPop() {
  if (Rp >= SS) Panic("RPop Underflow", Rp);
  int z = Rs[Rp];
  Rs[Rp] = -98;
  ++Rp;
  return z;
}

void BufAdd(byte b) {
  Buf[Bp++] = b;
  if (Bp>=BS) Panic("input buffer overflow", Bp);
}

void DoGet(byte a) { Push(Globals[a]); }
void DoPut(byte a) { int b = Pop(); Globals[a] = b; }
void DoNum(byte a) { Push(a); }
void DoMore(byte a) { int b = Pop(); Push(b*10+a); }
void DoCall(byte a) {
  RPush(Pc);
  Pc = BS*(a+1);
  printf("DoCall << arg=%d pc=%d\n", a, Pc);
}
void DoDef(byte a) {
  byte* p = &Progs[a][0];
  while (true) {
    byte x = GetOp(Pc++);
    if (x == t_st) { // if STOP
      *p++ = t_ex;   // append EXIT
      break;
    } else {
      *p++ = x;  // copy the opcode.
      if (FIRST_BI <= x && x < FIRST_BI_NO_OPERAND) {
        // also copy the arg.
        byte arg = GetOp(Pc++);
        *p++ = arg;
      }
    }
  }
}

byte GetOp(int pc) {
  if (pc<0 || pc>=11*BS) Panic("Bad pc", pc);
  // The first BS count is the Buffer.
  if (pc<BS) return Buf[pc];
  // The next 10 are in Progs.
  int prog = (pc/BS) - 1;
  int offset = pc%BS;
  return Progs[prog][offset];
}

void Dump() {
  //return;
  printf("\n\nDp=%d [", Dp);
  for (int i = Dp; i < SS; i++) printf("%d ", Ds[i]);
  printf("] \nRp=%d [", Rp);
  for (int i = Rp; i < SS; i++) printf("%d ", Rs[i]);
  printf("]\n Buf:");
  for (int i = 0; i < BS; i++) printf("%d ", Buf[i]);
  printf("\n");
  printf("\n");
}

bool Step() {
  int old_pc = Pc++;
  byte code = GetOp(old_pc);
  printf(" (Step[Pc=%d]=%d) ", old_pc, code);
  if (Pc%BS == 0) Panic("Pc overflow", Pc);

  // Numbered Ops:
  if (code < FIRST_CC) {
    int arg = code % 10;
    switch (code/10) {
      case nGet: DoGet(arg); break;
      case nPut: DoPut(arg); break;
      case nDef: DoDef(arg); return false;
      case nCall: DoCall(arg); break;
      case nNum: DoNum(arg); break;
      case nMore: DoMore(arg); break;
      default: Panic("bad numbered op", code); break;
    }
  } else switch (code) {
  // Bigraph Ops:
    case t_pl: {
                int b=Pop(); int a=Pop(); Push(a+b);
              } break;
    case t_pr: {
                int b=Pop(); printf(" <<<<< %d >>>>> ", b);
              } break;
    case t_st: {  // stop (stop reading buffer)
                return false;
              } break;
    case t_by: { // bye
                 fflush(stdout);
                 fflush(stderr);
                 fprintf(stderr, " *BYE* \n");
                exit(0);
              } break;
    case t_ex: { // exit (like return in most languages)
                Pc = RPop();
              } break;
    default: Panic("bad bigraph op", code);
  }
  return true;
}

void OnInput(byte code) {
  // Control Codes:
  switch (code) {
    case ccLF: {
                 // execute directly from input buffer.
                 BufAdd(t_st);  // append a stop op.
                 Pc = 0;
                 bool ok = true;
                 while (ok) {
                   ok = Step();
                 }
                 memset(Buf, 0, sizeof Buf);
                 Bp = 0;
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

int Panic(const char* msg, int num) {
  fprintf(stderr, "\n****** PANIC: ``%s'' (%d)\n", msg, num);
  assert(0);
  return 0;
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
    Buf[i] = 0;
    for (int j = 0; j<10; j++) Progs[j][i] = 0;
  }
}

int main(int argc, char *argv[]) {
  Reset();
  OnInput(nNum*10 + 3);
  Dump();
  OnInput(nNum*10 + 5);
  Dump();
  OnInput(t_pl);
  Dump();
  OnInput(t_pr);
  Dump();
  OnInput(ccLF);
  Dump();


  while (true) {
    bool ok = UnixInput();
    if (!ok) break;
    Dump();
  }

  printf("*END*\n");
}
