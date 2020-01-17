#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}

using namespace std;
using LL = long long;

int read_int(int &);

int main()
{
    int N;
    int s1, s2, s3, s12, s13, s23, s123;
    int c1 = 0, c2 = 0, c3 = 0, c12 = 0, c23 = 0, c13 = 0, c123 = 0;

    read_int(N);
    read_int(s1); read_int(s2); read_int(s3);
    if ( s1 > s2 ) swap(s1, s2);
    if ( s2 > s3 ) swap(s2, s3);
    if ( s1 > s2 ) swap(s1, s2);
    
    s123 = s1 + s2 + s3;
    s12 = s1 + s2, s13 = s1 + s3, s23 = s2 + s3;

    int ans = 0;

    if ( s12 > s3 ) {
        bool fail = false;
        for ( int i = 1; i <= N; i++) {
            int a; read_int(a);
            if ( a <= s1 ) ++c1;
            else if ( a <= s2 ) ++c2;
            else if ( a <= s3 ) ++c3;
            else if ( a <= s12 ) ++c12;
            else if ( a <= s13 ) ++c13;
            else if ( a <= s23 ) ++c23;
            else if ( a <= s123 ) ++c123;
            else fail = true;
        }

        if ( fail ) { cout << "-1\n"; return 0; }

        ans = c123;

        while ( c23 > 0 || c13 > 0 || c12 > 0 || c3 > 0 || c2 > 0 || c1 > 0 ) {
            if ( c23 > 0 ) {
                --c23;
                if ( c1 > 0 )
                    --c1;
            } else if ( c13 > 0 ) {
                --c13;
                if ( c2 > 0 )
                    --c2;
                else if ( c1 > 0 )
                    --c1;
            } else if ( c12 > 0 ) {
                --c12;
                if ( c3 > 0 )
                    --c3;
                else if ( c2 > 0 )
                    --c2;
                else if ( c1 > 0 )
                    --c1;
            } else if ( c3 > 0 ) {
                --c3;
                if ( c2 > 0 ) {
                    --c2;
                    if ( c1 > 0 )
                        --c1;
                } else if ( c1 > 0 ) {
                    c1 -= 2;
                } else if ( c3 > 0 )
                    --c3; // s12!!
            } else if ( c2 > 0 ) {
                --c2; // s2
                if ( c2 > 0 )
                    --c2; // s3
                else if ( c1 > 0 )
                    --c1; // s3
                if ( c1 > 0 )
                    --c1; // s1
            } else if ( c1 > 0 ) {
                c1 -= 3;
            }
            ++ans;
        }
    } else {
        bool fail = false;
        for ( int i = 1; i <= N; i++) {
            int a; read_int(a);
            if ( a <= s1 ) ++c1;
            else if ( a <= s2 ) ++c2;
            else if ( a <= s12 ) ++c12;
            else if ( a <= s3 ) ++c3;
            else if ( a <= s13 ) ++c13;
            else if ( a <= s23 ) ++c23;
            else if ( a <= s123 ) ++c123;
            else fail = true;
        }

        if ( fail ) { cout << "-1\n"; return 0; }

        ans = c123;

        while ( c23 > 0 || c13 > 0 || c12 > 0 || c3 > 0 || c2 > 0 || c1 > 0 ) {
            if ( c23 > 0 ) {
                --c23;
                if ( c1 > 0 )
                    --c1;
            } else if ( c13 > 0 ) {
                --c13;
                if ( c2 > 0 )
                    --c2; // s2
                else if ( c1 > 0 )
                    --c1; // s2
            } else if ( c3 > 0 ) {
                --c3;
                if ( c2 > 0 ) {
                    --c2; // s2
                    if ( c1 > 0 )
                        --c1; // s1
                } else if ( c1 > 0 ) {
                    c1 -= 2; // s1 , s2
                } else if ( c12 > 0 )
                    --c12;
            } else if ( c12 > 0 ) {
                --c12; // s3
                if ( c2 > 0 ) {
                    --c2; // s2
                    if ( c1 > 0 ) --c1;
                } else if ( c1 > 0 ) {
                    c1 -= 2;
                } else if ( c12 > 0 )
                    --c12;
            } else if ( c2 > 0 ) {
                --c2; // s2
                if ( c2 > 0 )
                    --c2; // s3
                else if ( c1 > 0 )
                    --c1; // s3
                if ( c1 > 0 )
                    --c1; // s1
            } else if ( c1 > 0 ) {
                c1 -= 3;
            }
            ++ans;
        }
    }

    cout << ans << '\n';
    return 0;
}

// input optimize
inline int read_char() {
    constexpr int N = 1<<20;
    static char buf[N];
    static char *p = buf, *end = buf;
    if ( p == end ) {
        if ( (end = buf + fread_unlocked(buf, 1, N, stdin)) == buf ) return EOF;
        p = buf;
    }
    return *p++;
}
inline int read_int(int &x) {
    static char c, neg;
    while ( (c = read_char()) < '-' )
        if ( c == EOF ) return 0;
    neg = (c == '-') ? -1 : 1;
    x = (neg == 1) ? c - '0' : 0;
    while ( (c = read_char()) >= '0' )
        x = (x<<3) + (x<<1) + (c-'0');
    x *= neg;
    return 1;
}
// output optimize
#define _pc putchar_unlocked
char _buf[128];
inline void write_int(LL x) {
    if ( !x ) { _pc('0'); _pc('\n'); return; }
    int t = 0;
    while ( x ) {
        _buf[t++] = x % 10 + '0';
        x /= 10;
    }
    while ( t-- ) _pc(_buf[t]);
    _pc('\n');
}
#undef _pc

