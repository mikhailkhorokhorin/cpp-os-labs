#include <cmath>
#include <vector>

#include "../include/contract.h"

extern "C" {

int PrimeCount(int A, int B) {
    if (B < 2)
        return 0;
    std::vector<bool> sieve(B + 1, true);
    sieve[0] = sieve[1] = false;

    for (int i = 2; i * i <= B; i++)
        if (sieve[i])
            for (int j = i * i; j <= B; j += i)
                sieve[j] = false;

    int cnt = 0;
    for (int i = A; i <= B; i++)
        if (sieve[i])
            cnt++;

    return cnt;
}

float Pi(int K) {
    double prod = 1.0;
    for (int i = 1; i <= K; i++) {
        double num = 4.0 * i * i;
        prod *= num / (num - 1);
    }
    return 2.0 * prod;
}
}
