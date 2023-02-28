#include <bits/stdc++.h>

using namespace std;
#define eb emplace_back
//#define int long long
int k = 350;

vector<vector<int>> bsqrtd(vector<int> &a) {

    vector<vector<int>> sq((a.size() + k - 1) / k);
    for (int i = 0 ; i < a.size() ; i++) {
        sq[i / k].push_back(a[i]);
    }
    return sq;
}

vector <int> bblocks(vector<vector<int>> & sq) {
    vector <int> b(sq.size());
    for (int i = 0; i < sq.size(); i++) {
        b[i] = sq[i][max_element(sq[i].begin(), sq[i].end()) - sq[i].begin()];
    }
    return b;
}

int req(int l , int r, vector<vector<int>> & sq, vector <int> & b) {
    if (r / k == l / k) {
        int ans = -1e9;
        for (int i = l; i <= r; i++) {
            ans = max(ans, sq[i / k][i % k]);
        }
        return ans;
    } else {
        int ans = -1e9;
        for (int i = l % k; i < k; i++) {
            ans = max(ans, sq[l / k][i]);
        }
        for (int i = l / k + 1; i < r / k; i++) {
            ans = max(ans, b[i]);
        }
        for (int i = 0; i <= r % k; i++) {
            ans = max(ans, sq[r / k][i]);
        }
        return ans;
    }
}

int main() {
    int n;
    cin >> n;
    vector <int> v(n);
    for (auto &el: v) {
        cin >> el;
    }
    vector<vector<int>> sq = bsqrtd(v);
    vector <int> b = bblocks(sq);
    /*for (auto i : sq) {
        for (auto j : i) {
            cout << j << ' ';
        } cout << "\n";
    }
    for (auto i : b) {
        cout << i << ' ';
    }*/
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l  >> r;
        l--;
        r--;
        cout << req(l , r , sq, b) << ' ';
    }
}