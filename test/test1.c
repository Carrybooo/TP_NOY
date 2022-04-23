// test1.c

#include "userlib/libnachos.h"
#include "userlib/syscall.h"

SemId sem1, sem2;



void crosstest1()
{
  int cpt = 0;
  while (cpt<100) {
    P(sem1);
    n_printf("sem1 cpt = %d\n", cpt);
    V(sem2);
    cpt++;
    }
}

void crosstest2()
{
  int cpt = 0;
  while (cpt<100) {
    P(sem2);
    n_printf("sem2 cpt = %d\n", cpt);
    V(sem1);
    cpt++;
    }
}


int main(int argc, char const *argv[])
{
    n_printf("---------------------------------TEST 1 STARTING----------------------------------------\n");

    /* code */
    sem1 = SemCreate((char *) "sem1", 1);
    sem2 = SemCreate((char *) "sem2", 0);

    threadCreate((char*) "thread1", crosstest1);
    threadCreate((char*) "thread2", crosstest2);

    n_printf("----------------------------TEST 1 STARTED SUCCESSFULLY----------------------------------\n");
    return 0;
}
