auto comp = a;
sort(comp.begin(), comp.end());
for (int i = 0; i < n; i++) {
    a[i] = lower_bound(comp.begin(), comp.end(), a[i]) - comp.begin() + 1;
}