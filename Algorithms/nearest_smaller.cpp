//   Compute nearest smaller values for all i:
//     nearest_smaller[i] = argmax { j : j < i, a[j] < a[i] }.

template <class T>
vector<int> nearest_smallers(const vector<T> &x) {
  vector<int> z;
  for (int i = 0; i < x.size(); ++i) {
    int j = i-1;
    while (j >= 0 && x[j] >= x[i]) j = z[j];
    z.push_back(j);
  }
  return z;
}
