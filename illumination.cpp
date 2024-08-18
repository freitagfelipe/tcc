#include <bits/stdc++.h>

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

    void add_or(int a, int b) {
        this->add_edge(a ^ 1, b);
        this->add_edge(b ^ 1, a);
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

    int n, r, k;

    cin >> n >> r >> k;

    vector<pii> lamps(k);

    for (auto &[f, s] : lamps) {
        cin >> f >> s;
    }

    SAT sat(k);

    for (int i {}; i < k; ++i) {
        for (int j {i + 1}; j < k; ++j) {
            int ia {i * 2}, ja {j * 2};
            auto [ix, iy] {lamps[i]};
            auto [jx, jy] {lamps[j]};

            if (ix == jx && abs(iy - jy) < 2 * r) {
                sat.add_or(ia ^ 1, ja ^ 1);
            }

            if (iy == jy && abs(ix - jx) < 2 * r) {
                sat.add_or(ia, ja);
            }
        }
    }

    cout << sat.has_ans() << '\n';

    return 0;
}
