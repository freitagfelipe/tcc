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

private:
    int n;
    vector<pii> edges;

    void add_edge(int a, int b) {
        edges.emplace_back(a, b);
    }
};

void solver(int c, int p) {
    SAT sat(p);

    for (int i {}; i < c; ++i) {
        int s1, s2, r1, r2;

        cin >> s1 >> s2 >> r1 >> r2;

        if (s1 != 0 && s2 != 0) {
            --s1;
            --s2;

            sat.add_or(s1 * 2, s2 * 2);
        } else if (s1 + s2 != 0) {
            if (s1 == 0) {
                swap(s1, s2);
            }

            --s1;

            sat.add_or(s1 * 2, s1 * 2);
        }

        if (r1 != 0 && r2 != 0) {
            --r1;
            --r2;

            sat.add_or((r1 * 2) ^ 1, (r2 * 2) ^ 1);
        } else if (r1 + r2 != 0) {
            if (r1 == 0) {
                swap(r1, r2);
            }

            --r1;

            sat.add_or((r1 * 2) ^ 1, (r1 * 2) ^ 1);
        }
    }

    sat.has_ans() ? cout << "yes\n" : cout << "no\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int c, p;

    while (cin >> c >> p && c != 0 && p != 0) {
        solver(c, p);
    }

    return 0;
}
