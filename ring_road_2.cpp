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

private:
    int n;
    vector<pii> edges;

    void add_edge(int a, int b) {
        edges.emplace_back(a, b);
    }
};

bool has_intersection(const pii &edge_a, const pii &edge_b) {
    auto [au, av] {edge_a};
    auto [bu, bv] {edge_b};

    return (au < bu && bu < av && av < bv) || (bu < au && au < bv && bv < av);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;

    cin >> n >> m;

    vector<pii> edges(m);

    for (auto &[u, v] : edges) {
        cin >> u >> v;

        if (u > v) {
            swap(u, v);
        }
    }

    SAT sat(m);

    for (int i {}; i < m; ++i) {
        for (int j {i + 1}; j < m; ++j) {
            if (has_intersection(edges[i], edges[j])) {
                sat.add_xor(i * 2, j * 2);
            }
        }
    }

    vector<bool> ans {sat.solve()};

    if (ans.empty()) {
        cout << "Impossible\n";

        return 0;
    }

    for (int i {}; i < m; ++i) {
        ans[i] ? cout << "i" : cout << "o";
    }

    cout << '\n';

    return 0;
}
