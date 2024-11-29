#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

typedef pair<int, int> pii;

vector<vector<int>> g, tg;
vector<int> ts, component;
vector<bool> visited;

void ts_dfs(int u) {
	visited[u] = true;

    for (int to : g[u]) {
        if (!visited[to]) {
            ts_dfs(to);
        }
    }

    ts.push_back(u);
}

void component_dfs(int u, int c) {
    component[u] = c;

    for (int to : tg[u]) {
        if (component[to] == -1) {
            component_dfs(to, c);
        }
    }
}

vector<int> scc(int n, const vector<pii> &edges) {
    g.assign(n, vector<int>());
    tg.assign(n, vector<int>());
	component.assign(n, -1);
	visited.assign(n, false);

    for (auto [a, b] : edges) {
        g[a].push_back(b);
        tg[b].push_back(a);
    }

	for (int i {}; i < n; ++i) {
		if (!visited[i]) {
			ts_dfs(i);
		}
	}

	reverse(ts.begin(), ts.end());

    int comp {};

    for (int v : ts) {
        if (component[v] == -1) {
            component_dfs(v, comp++);
        }
    }

	ts.clear();

	return component;
}

class SAT {
public:
    SAT(int n) {
        this->n = 2 * n;
    }

    vector<bool> solve() {
        vector<bool> ans(this->n / 2);
        vector<int> comp {scc(this->n, this->edges)};

        for (int i {}; i < this->n; i += 2) {
            if (comp[i] == comp[i + 1]) {
                return {};
            }

            ans[i / 2] = (comp[i] > comp[i + 1]);
        }

        return ans;
    }

    bool has_ans() {
        vector<int> comp {scc(this->n, this->edges)};

        for (int i {}; i < this->n; i += 2) {
            if (comp[i] == comp[i + 1]) {
                return false;
            }
        }

        return true;
    }

    void add_xor(int a, int b) {
        this->add_or(a, b);
        this->add_or(a ^ 1, b ^ 1);
    }

    void add_equal(int a, int b) {
        this->add_or(a, b ^ 1);
        this->add_or(a ^ 1, b);
    }

    void add_or(int a, int b) {
        this->add_edge(a ^ 1, b);
        this->add_edge(b ^ 1, a);
    }

    void add_diff(int a, int b) {
        this->add_equal(a, b ^ 1);
    }
    
    void add_truth_table(int a1, int a2, bool v00, bool v01, bool v10, bool v11) {
        if (!v00) {
            this->add_or(a1, a2);
        }
        
        if (!v01) {
            this->add_or(a1, a2 ^ 1);
        }
        
        if (!v10) {
            this->add_or(a1 ^ 1, a2);
        }
        
        if (!v11) {
            this->add_or(a1 ^ 1, a2 ^ 1);
        }
    }

private:
    int n;
    vector<pii> edges;

    void add_edge(int a, int b) {
        edges.emplace_back(a, b);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, p, M, m;

    cin >> n >> p >> M >> m;

    int SAT_SIZE {p + 1 + M};
    // The p-th index will be used to f = 0
    int FREQUENCY_OFFSET {p};
    
    SAT sat(SAT_SIZE);

    for (int i {1}; i < M; ++i) {
        int cf {i + FREQUENCY_OFFSET};
        int ncf {i + FREQUENCY_OFFSET + 1};

        assert(cf < SAT_SIZE);
        assert(ncf < SAT_SIZE);

        sat.add_or(cf * 2, (ncf * 2) ^ 1);
    }

    // Complaints
    for (int i {}; i < n; ++i) {
        int xi, yi;

        cin >> xi >> yi;

        --xi; xi *= 2;
        --yi; yi *= 2;

        sat.add_or(xi, yi);
    }

    vector<pii> frequencies(p);

    for (int i {}; i < p; ++i) {
        int li, ri;

        cin >> li >> ri;

        li += FREQUENCY_OFFSET;

        sat.add_or((i * 2) ^ 1, li * 2);

        if (ri == M) {
            continue;
        }

        ri += FREQUENCY_OFFSET;

        sat.add_or((i * 2) ^ 1, ((ri + 1) * 2) ^ 1);
    }

    // Interfering radio stations
    for (int i {}; i < m; ++i) {
        int ui, vi;

        cin >> ui >> vi;

        --ui; ui *= 2;
        --vi; vi *= 2;

        sat.add_or(ui ^ 1, vi ^ 1);
    }

    auto ans {sat.solve()};

    if (ans.empty()) {
        cout << -1 << '\n';

        return 0;
    }

    int k {}, f {};
    vector<int> stations;

    for (int i {}; i < p; ++i) {
        if (ans[i] == 0) {
            continue;
        }

        ++k;
        
        stations.push_back(i + 1);
    }

    for (int i {SAT_SIZE - 1}; i > FREQUENCY_OFFSET; --i) {
        if (ans[i]) {
            f = i - FREQUENCY_OFFSET;

            break;
        }
    }

    if (f == 0) {
        cout << -1 << '\n';
    }

    cout << k << " " << f << '\n';

    for (int i {}; i < k; ++i) {
        cout << stations[i] << " \n"[i + 1 == k];
    }

    return 0;
}
