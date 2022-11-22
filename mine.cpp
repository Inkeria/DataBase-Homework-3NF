#include <bits/stdc++.h>
using namespace std;
typedef long long dnt;
map<dnt, dnt> mp;
map<char, int> P;
vector<pair<dnt, dnt>> R; 
map<dnt, dnt> closure;
#define fi first
#define se second
dnt getclosure(dnt l,const pair<dnt, dnt> &ban)
{
    if(!ban.fi && closure[l]) return closure[l];// 记忆化搜索
    dnt r = l;
    for(auto x : R) 
    {
        if((x.fi & l) ^ x.fi) continue ;
        r |= x.se ^ (x.fi == ban.fi ? ban.se & x.se : 0);
    }
    if(ban.fi) return r == l ? r : getclosure(r, ban);
    return closure[l] = r == l ? r : getclosure(r, ban);
}
int n, m;
char A[52],s[102];
void print(dnt x) {
    for(int i = 0; i < n; ++i) 
        printf(x & (1ll << i) ? "%c" : "" , A[i]);
    puts("");
}
int main()
{
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
        int len = strlen(s);
        dnt lf = 0, rg = 0 ;
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
        dnt lf = R[i].fi, rg = R[i].se;
        for(dnt j = lf & -lf, res = lf; res; res ^= j, j = res & -res){
            dnt tmp = getclosure(lf ^ j, make_pair(0, 0));
            if((rg & tmp) == rg) lf ^= j;
        }
        R[i].fi = lf;
    }
    // 去除右侧多余
    for(int i = 0;i < R.size(); ++i){
        dnt lf = R[i].fi, rg = R[i].se, nrg = 0;
        for(dnt j = rg & -rg, res = rg; res; res ^= j, j = res & -res){
            dnt tmp = getclosure(lf, make_pair(lf, j));
            if((tmp & j) ^ j) nrg |= j;
        } 
        if(nrg) mp[lf] |= nrg;
    }
    //删去右侧空式
    R.clear(); for(auto U:mp) R.push_back(U); 
    //求候选键, 复杂度瓶颈！！！
    dnt candidate = (1ll << n) - 1;
    for(dnt i = 0;i < 1ll << n;++i){
        if(__builtin_popcount(i) >= __builtin_popcount(candidate)) continue ;
        if(getclosure(i, make_pair(0, 0)) ^ ((1ll << n) - 1)) continue ; 
        candidate = i;
    }
    //输出
    mp.clear();
    for(auto x:R) { 
        mp[x.fi|x.se] = 1;
        if((candidate & (x.fi | x.se)) == candidate) candidate = 0;
    }
    if(candidate) mp[candidate] = 1;
    puts("将关系模式 R 无损分解且保持依赖地分解成 3NF 模式集，如下：");
    for(auto x:mp) print(x.fi);
    return 0;
}