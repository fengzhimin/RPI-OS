#ifndef __TASK_H__
#define __TASK_H__

struct context{
  unsigned int r0;
  unsigned int r1;
  unsigned int r2;
  unsigned int r3;
  unsigned int r4;
  unsigned int r6;
  unsigned int r7;
  unsigned int r8;
  unsigned int r9;
  unsigned int r10;
  unsigned int r11;
  unsigned int r12;
  unsigned int sp;
  unsigned int lr;
  unsigned int pc;
  unsigned int spsr;
};

struct tcb{
  unsigned int kstack;
  unsigned int tid;
  int          state;
  struct tcb *next;
};

#endif
