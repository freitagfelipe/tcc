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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;

    cin >> n >> m;

    SAT sat(n);

    for (int i {}; i < m; ++i) {
        int u, v;
        bool asphalted;

        cin >> u >> v >> asphalted;

        --u;
        --v;

        u *= 2;
        v *= 2;

        if (asphalted) {
            sat.add_equal(u, v);
        } else {
            sat.add_xor(u, v);
        }
    }

    vector<bool> sat_ans {sat.solve()};

    if (sat_ans.size() == 0) {
        cout << "Impossible\n";

        return 0;
    }

    vector<int> ans;

    for (int i {}; i < (int) sat_ans.size(); ++i) {
        if (sat_ans[i]) {
            ans.push_back(i + 1);
        }
    }

    cout << ans.size() << '\n';

    for (int i {}; i < (int) ans.size(); ++i) {
        cout << ans[i] << " \n"[i + 1 == (int) ans.size()];
    }

    return 0;
}
