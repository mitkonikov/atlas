#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
 
const int mod=998244353;
const int primitive_root=3;

struct mi {};

void ntt(vector<mi> &a) {
    int n=a.size(),L=31-__builtin_clz(n);
    vector<int> rev(n);
    for(int i=0;i<n;i++) rev[i]=(rev[i/2]+((i&1)<<L))/2;
    for(int i=0;i<n;i++) if(i<rev[i]) swap(a[i],a[rev[i]]);
    static vector<mi> rt(2,1);
    for(static int k=2,s=2;k<n;k*=2,s++) {
        rt.resize(n);
        mi z=pow(mi(primitive_root),mod>>s);
        for(int i=k;i<2*k;i++) rt[i]=rt[i/2]*((i&1)?z:1);
    }
    for(int k=1;k<n;k*=2) {
        for(int i=0;i<n;i+=2*k) {
            for(int j=0;j<k;j++) {
                mi z=rt[j+k]*a[i+j+k];
                a[i+j+k]=a[i+j]-z;
                a[i+j]+=z;
            }
        }
    }
}
 
vector<mi> conv(vector<mi> &a, vector<mi> &b) {
    int sa=a.size(),sb=b.size();
    if(sa==0||sb==0) return {};
    int n=1<<(32-__builtin_clz(sa+sb-2));
    mi inv=1/mi(n);
    vector<mi> f(a),g(b),h(n);
    f.resize(n); g.resize(n);
    ntt(f); ntt(g);
    for(int i=0;i<n;i++) h[(-i)&(n-1)]=(f[i]*g[i]*inv);
    ntt(h);
    h.resize(sa+sb-1);
    return h;
}
 
vector<mi> multipoly(vector<vector<mi>> v) {
    auto cmp=[&](const vector<mi> &a,const vector<mi> &b){return (a.size()>b.size());};
    priority_queue<vector<mi>,vector<vector<mi>>,decltype(cmp)> q(cmp);
    for(auto &u:v) q.push(u);
    while(q.size()>=2) {
        auto a=q.top();
        q.pop();
        auto b=q.top();
        q.pop();
        q.push(conv(a,b));
    }
    return q.top();
}