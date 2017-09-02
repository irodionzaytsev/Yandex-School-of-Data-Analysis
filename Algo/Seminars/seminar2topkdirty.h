#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<time.h>

using namespace std;

int x, y;
int n, k;
int need;
int minn = -1;
int A, B, C;
vector<int>g;
int IN = 2147483647;
int size[70001];


void Clearing(){

    sort(g.begin(), g.end());
    g.resize(k);
    minn = - g[k - 1];
}

void Init(){
    cin>>n>>k;
    cin>>x>>y>>A>>B>>C;
    for(int i = 0; i<=70000; ++i)size[i] = 0;
}

void Solve(){
    int a = x;
    int b = y;

    for(long i = 1; i<=n; ++i){
        int t = b;
        b = (A*a + B*b + C);
        a = t;
        if (b<0) b = b & IN;
        int c = b>>16;
        size[c] ++;
    }

    long v = 0;

    for( need = 70000; need>=0; --need){
        v += size[need];
        if (v >= k)break;
    }

    a = x;
    b = y;

    int i;

    for(i = 1; i<=n; ++i){
        int t = b;
        b = (A*a + B*b + C);
        a = t;
        if (b<0) b = b & IN;
        int c = b>>16;
        if (c>=need && b>minn){g.push_back(-b);};

        if (g.size() == 2*k)Clearing();
    }



    Clearing();

    //for(long i = 0; i<k; ++i) cout<<-g[i]<<" ";

}

int main(){

// time_t t = clock();

    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);

    Init();
    Solve();

    // cerr<<(clock() - t)/1.0/CLOCKS_PER_SEC;

    return 0;
}


