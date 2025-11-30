#include <cmath>

#include "../include/contract.h"

extern "C" {

bool isPrime(int n) {
    if (n < 2)
        return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    return true;
}

int PrimeCount(int A, int B) {
    int cnt = 0;
    for (int i = A; i <= B; i++)
        if (isPrime(i))
            cnt++;
    return cnt;
}

float Pi(int K) {
    double sum = 0.0;
    for (int i = 0; i < K; i++)
        sum += (i % 2 == 0 ? 1.0 : -1.0) / (2.0 * i + 1);
    return 4.0 * sum;
}
}
