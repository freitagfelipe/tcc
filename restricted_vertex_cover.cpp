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

vector<int> scc(int n) {
	component.assign(n, -1);
	visited.assign(n, false);

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

    bool has_ans() {
        vector<int> comp {scc(this->n)};

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

    void add_or(int a, int b) {
        this->add_edge(a ^ 1, b);
        this->add_edge(b ^ 1, a);
    }

private:
    int n;

    void add_edge(int a, int b) {
        g[a].push_back(b);
        tg[b].push_back(a);
    }
};

void solver() {
    int n, m;

    cin >> n >> m;

    g.assign(2 * n, vector<int>());
    tg.assign(2 * n, vector<int>());

    SAT sat(n);

    for (int i {}; i < m; ++i) {
        int u, v;
        bool marked;

        cin >> u >> v >> marked;

        --u;
        --v;

        u *= 2;
        v *= 2;

        if (marked) {
            sat.add_xor(u, v);
        } else {
            sat.add_or(u, v);
        }
    }

    sat.has_ans() ? cout << "YES\n" : cout << "NO\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int t;

    cin >> t;

    while (t--) {
        solver();
    }

    return 0;
}
