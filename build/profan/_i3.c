unsigned long long __udivdi3(unsigned long long num, unsigned long long den) {
    unsigned long long quotient = 0;
    unsigned long long remainder = 0;
    unsigned long long mask = 1ULL << 63;

    for(int i = 0; i < 64; i++) {
        remainder = (remainder << 1) | ((num & mask) >> 63);
        num = num << 1;
        quotient = quotient << 1;

        if (remainder >= den) {
            remainder -= den;
            quotient |= 1;
        }
    }

    return quotient;
}

signed long long __divdi3(signed long long num, signed long long den) {
    int sign = 1;
    if ((num < 0 && den > 0) || (num > 0 && den < 0)) {
        sign = -1;
    }

    unsigned long long unsigned_num = (num < 0) ? -num : num;
    unsigned long long unsigned_den = (den < 0) ? -den : den;

    unsigned long long quotient = __udivdi3(unsigned_num, unsigned_den);

    return sign * quotient;
}

unsigned long long __umoddi3(unsigned long long num, unsigned long long den) {
    unsigned long long quotient = __udivdi3(num, den);
    unsigned long long remainder = num - den * quotient;
    return remainder;
}

long long __moddi3(long long a, long long b) {
    int sign = 1;
    if (a < 0) {
        sign = -1;
    }

    unsigned long long unsigned_a = (a < 0) ? -a : a;
    unsigned long long unsigned_b = (b < 0) ? -b : b;

    unsigned long long remainder = __umoddi3(unsigned_a, unsigned_b);

    return sign * remainder;
}
