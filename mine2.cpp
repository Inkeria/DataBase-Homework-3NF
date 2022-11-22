#include <bits/stdc++.h>
using namespace std;
map<int, int> mp;
map<char, int> P;
vector<pair<int, int>> R; 
map<int, int> closure;
#define fi first
#define se second
inline bool in(const int &a,const int &b) {return (a & b) == a;}
int getC(int l){
    int r = l;
    for(auto x : R) r |= in(x.fi, l) ? x.se : 0 ;
    return r == l ? r : getC(r);
}
int n, m;
char A[52],s[102];
void print(int x) {
    for(int i = 0; i < n; ++i) 
        printf(x >> i & 1 ? "%c" : "" , A[i]);
}
int main()
{
    //输入
    puts("请输入关系模式的属性：");
    scanf("%s",A);
    n = strlen(A);
    for(int i = 0;i < n;++i) P[A[i]] = i;
    puts("请输入函数依赖集大小：");
    scanf("%d",&m);
    puts("请输入函数依赖，每行两个字符串，形如 a->b");
    for(int i = 1;i <= m;++i)
    {
        scanf("%s",s);
        int len = strlen(s), lf = 0, rg = 0;
        for(int j = 0, now = 0;j < len;++j){
            if(s[j] == '-' || s[j] == '>') { now = 1; continue ;}
            if(!now) lf |= 1ll << P[s[j]];
            if( now) rg |= 1ll << P[s[j]];
        }
        R.push_back(make_pair(lf, rg ^ (lf & rg)));
    }
    // 去除左侧多余
    for(int i = 0;i < R.size(); ++i){
        if(!(R[i].fi & (R[i].fi - 1))) continue ;
        int &lf = R[i].fi, &rg = R[i].se, tmp;
        for(int j = lf & -lf, res = lf; res; res ^= j, j = res & -res){
            tmp = getC(lf ^ j);
            if(in(rg, tmp)) lf ^= j;
        }
    }
    // 去除右侧多余
    for(int i = 0;i < R.size(); ++i){
        int &lf = R[i].fi, &rg = R[i].se, tmp;
        for(int j = rg & -rg, res = rg; res; res ^= j, j = res & -res){
            rg ^= j; tmp = getC(lf);
            if(!in(j, tmp)) rg ^= j;
        } 
        if(rg) mp[lf] |= rg;
    }
    //求候选键
    int candidate = (1ll << n) - 1;
    for(int i = 0;i < 1ll << n;++i){
        if(__builtin_popcount(i) >= __builtin_popcount(candidate)) continue ;
        if(getC(i) ^ ((1ll << n) - 1)) continue ; 
        candidate = i;
    }
    for(auto x:mp) candidate = in(candidate, x.fi|x.se) ? 0 : candidate;
    if(candidate) mp[candidate] = candidate;
    //输出
    puts("将关系模式 R 无损分解且保持依赖地分解成 3NF 模式集，如下：");
    for(auto x:mp) print(x.fi|x.se), puts("");
    return 0;
}