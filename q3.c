#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_THREADS 3
uthread_t threads[NUM_THREADS];
uthread_mutex_t mx;
uthread_cond_t acond;
int letternums = NUM_THREADS;

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void waitForAllOtherThreads() {
  uthread_mutex_lock(mx);
  if(letternums != 0)
    uthread_cond_wait(acond);
  uthread_mutex_unlock(mx);
}

void printA(){
  uthread_mutex_lock(mx);
  letternums--;
  printf("a\n");
  if(letternums == 0)
    uthread_cond_broadcast(acond);
  uthread_mutex_unlock(mx);
}
void printB(){
  printf("b\n");
}
void* p(void* v) {
  randomStall();
  printA();
  waitForAllOtherThreads();
  printB();
  return NULL;
}

int main(int arg, char** arv) {
  mx = uthread_mutex_create();
  acond = uthread_cond_create(mx);
  uthread_init(4);
  for (int i=0; i<NUM_THREADS; i++)
    threads[i] = uthread_create(p, NULL);
  for (int i=0; i<NUM_THREADS; i++)
    uthread_join (threads[i], NULL);
  printf("------\n");
}