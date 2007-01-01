/*
  libco_win32 : version 0.08 ~byuu (10/21/06)
  win32-x86 implementation of libco
*/

#define WINVER       0x0400
#define _WIN32_WINNT 0x0400
#include <windows.h>

#include "libco_win32.h"

struct cothread_struct {
  void *cohandle;
  cothread_p coentry;
  cothread_t colink;
};

cothread_t __co_active = 0, __co_primary = 0;

void __stdcall co_entryproc(void *coentry) {
cothread_struct *s = static_cast<cothread_struct*>(coentry);
  s->coentry();
  co_exit(0);
}

cothread_t co_init() {
  ConvertThreadToFiber(0);
cothread_struct *s = static_cast<cothread_struct*>(malloc(sizeof(cothread_struct)));
  s->colink   = 0;
  s->coentry  = 0;
  s->cohandle = GetCurrentFiber();
  __co_active = __co_primary = static_cast<cothread_t>(s);
  return __co_active;
}

void co_term() {
//primary fiber cannot be deleted; free memory used by handle to fiber only
  free(__co_primary);
//ConvertFiberToThread(); //only exists on WinXP+
}

cothread_t co_active() {
  return __co_active;
}

cothread_t co_create(cothread_p coentry, unsigned int heapsize) {
cothread_struct *s = static_cast<cothread_struct*>(malloc(sizeof(cothread_struct)));
  s->colink   = co_active();
  s->coentry  = coentry;
  s->cohandle = CreateFiber(heapsize + 512, co_entryproc, static_cast<void*>(s));
  return static_cast<cothread_t>(s);
}

void co_delete(cothread_t cothread) {
cothread_struct *s = static_cast<cothread_struct*>(cothread);
  DeleteFiber(s->cohandle);
  free(cothread);
}

void co_switch(cothread_t cothread) {
  __co_active = cothread;
cothread_struct *s = static_cast<cothread_struct*>(cothread);
  SwitchToFiber(s->cohandle);
}

void co_exit(cothread_t cothread) {
  if(cothread != 0) { co_switch(cothread); }
cothread_struct *s = static_cast<cothread_struct*>(__co_active);
  co_switch(s->colink);
}
