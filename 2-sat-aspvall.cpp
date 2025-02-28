#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;

// tg is the transpose graph
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

// The graph must be 0-indexed
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

    void add_xor(int p, int q) {
        this->add_or(p, q);
        this->add_or(p ^ 1, q ^ 1);
    }

    void add_equal(int p, int q) {
        this->add_or(p, q ^ 1);
        this->add_or(p ^ 1, q);
    }

    void add_or(int p, int q) {
        this->edges.emplace_back(p ^ 1, q);
        this->edges.emplace_back(q ^ 1, p);
    }

    void add_diff(int p, int q) {
        this->add_equal(p, q ^ 1);
    }
    
    // Using maxterms
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
};
