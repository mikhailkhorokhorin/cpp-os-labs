#ifndef PRIMEPI_H
#define PRIMEPI_H

typedef int (*PrimeCountFunc)(int A, int B);

typedef float (*PiFunc)(int K);

extern "C" {
int PrimeCount(int A, int B);
float Pi(int K);
}

#endif
