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

void impossible_case() {
    cout << "No\n";

    exit(0);
}

void add_equal(int a1, int a2, int val, SAT &sat) {
    if (val == 0) {
        sat.add_truth_table(a1, a2, 1, 0, 0, 0);
    } else if (val == 1) {
        sat.add_truth_table(a1, a2, 0, 1, 1, 0);
    } else if (val == 2) {
        sat.add_truth_table(a1, a2, 0, 0, 0, 1);
    }
}

void add_difference(int a1, int a2, int val, SAT &sat) {
    if (val == 0) {
        sat.add_truth_table(a1, a2, 0, 1, 1, 1);
    } else if (val == 1) {
        sat.add_truth_table(a1, a2, 1, 0, 0, 1);
    } else if (val == 2) {
        sat.add_truth_table(a1, a2, 1, 1, 1, 0);
    }
}

void add_less_than(int a1, int a2, int val, SAT &sat) {
    if (val == 0) {
        impossible_case();
    } else if (val == 1) {
        sat.add_truth_table(a1, a2, 1, 0, 0, 0);
    } else if (val == 2) {
        sat.add_truth_table(a1, a2, 1, 1, 1, 0);
    }
}

void add_greater_than(int a1, int a2, int val, SAT &sat) {
    if (val == 0) {
        sat.add_truth_table(a1, a2, 0, 1, 1, 1);
    } else if (val == 1) {
        sat.add_truth_table(a1, a2, 0, 0, 0, 1);
    } else if (val == 2) {
        impossible_case();
    }
}

void add_less_or_equal_than(int a1, int a2, int val, SAT &sat) {
    if (val == 0) {
        sat.add_truth_table(a1, a2, 1, 0, 0, 0);
    } else if (val == 1) {
        sat.add_truth_table(a1, a2, 1, 1, 1, 0);
    } else if (val == 2) {
        sat.add_truth_table(a1, a2, 1, 1, 1, 1);
    }
}

void add_greater_or_equal_than(int a1, int a2, int val, SAT &sat) {
    if (val == 0) {
        sat.add_truth_table(a1, a2, 1, 1, 1, 1);
    } else if (val == 1) {
        sat.add_truth_table(a1, a2, 0, 1, 1, 1);
    } else if (val == 2) {
        sat.add_truth_table(a1, a2, 0, 0, 0, 1);
    }
}

void add_operation(int a1, int a2, string &op, int val, SAT &sat) {
    if (op == "=") {
        add_equal(a1, a2, val, sat);
    } else if (op == "!=") {
        add_difference(a1, a2, val, sat);
    } else if (op == "<") {
        add_less_than(a1, a2, val, sat);
    } else if (op == ">") {
        add_greater_than(a1, a2, val, sat);
    } else if (op == "<=") {
        add_less_or_equal_than(a1, a2, val, sat);
    } else if (op == ">=") {
        add_greater_or_equal_than(a1, a2, val, sat);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;

    cin >> n >> m;

    SAT sat(n);

    for (int i {}; i < m; ++i) {
        int a1, a2, val;
        string op;

        cin >> a1 >> a2 >> op >> val;

        add_operation(a1 * 2, a2 * 2, op, val, sat);
    }

    sat.has_ans() ? cout << "Yes\n" : cout << "No\n";

    return 0;
}
