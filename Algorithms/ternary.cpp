template<typename T>
T ternSearch(T a, T b, function<T(T)> f) {
	assert(a <= b);
	while (b - a >= 1e-8) {
		T mid = (b - a) / 3;
		if (f(a + mid) > f(a + mid * 2)) b = a + mid * 2;
		else a = a + mid;
	}
	return a;
}