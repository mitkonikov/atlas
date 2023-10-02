ll expo(ll base, ll power) {
    ll result = 1;

    while (power > 0) {
        if (power % 2 == 1) {
            result = result * base;
            power--;
        }

        base = base * base;
        power /= 2;
    }

    return result;
}