struct StrHasher {
    int p, m;
    vector<int> p_pow;

    StrHasher(int max_len, int _p=256, int _m=1e9+7): p(_p), m(_m) {
        p_pow.resize(max_len+1);
        p_pow[0] = 1;
        for (int i = 1; i <= max_len; i++)
            p_pow[i] = (1ll * p_pow[i-1] * p) % m;
    }

    vector<int> hash(const string& S) {
        vector<int> H(S.length(), 0);
        H[0] = S[0];
        for (int i = 1; i < S.length(); i++)
            H[i] = (1ll * H[i-1] * p + S[i]) % m;
        return H;
    }

    int get_hash(vector<int>&H, int l, int r) {
        if (l == 0) return H[r];
        else return (H[r] - 1ll * H[l-1] * p_pow[r-l+1] % m + m) % m;
    }
};