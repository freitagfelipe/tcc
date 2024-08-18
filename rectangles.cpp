#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
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

    for (int i {}; i < (int) edges.size(); ++i) {
        int a {edges[i].first}, b {edges[i].second};

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

template <typename T>
struct Point2D {
    T x;
    T y;

	Point2D() = default;
    
    Point2D(T x, T y) {
	    this->x = x;
	    this->y = y;
    }

    bool operator<(const Point2D &t) const {
        if (this->x == t.x) {
            return this->y < t.y;
        }

        return this->x < t.x;
    }
};

template <typename T>
using Segment = pair<Point2D<T>, Point2D<T>>;

template <typename T>
int orientation(const Point2D<T> &a, const Point2D<T> &b, const Point2D<T> &c) {
    T result {a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)};

    if (result < 0) {
        return -1;
    } else if (result > 0) {
        return 1;
    }

    return 0;
}

template <typename T>
bool intersect_aux(T a, T b, T c, T d) {
    if (a > b) {
        swap(a, b);
    }

    if (c > d) {
        swap(c, d);
    }

    return max(a, c) <= min(b, d);
}

template <typename T>
bool intersect(const Point2D<T> &a, const Point2D<T> &b, const Point2D<T> &c, const Point2D<T> &d) {
    if (orientation(c, a, d) == 0 && orientation(c, b, d) == 0) {
        return intersect_aux(a.x, b.x, c.x, d.x) && intersect_aux(a.y, b.y, c.y, d.y);
    }

    bool first_condition(orientation(a, b, c) != orientation(a, b, d));
    bool second_condition(orientation(c, d, a) != orientation(c, d, b));

    return first_condition && second_condition;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;

    while (cin >> n && n != 0) {
        vector<Segment<ll>> segments;

        for (int i {}; i < n; ++i) {
            vector<Point2D<ll>> points;

            for (int j {}; j < 4; ++j) {
                int x, y;

                cin >> x >> y;

                points.emplace_back(x, y);
            }

            sort(points.begin(), points.end());

            segments.emplace_back(points[0], points[3]);
            segments.emplace_back(points[1], points[2]);
        }

        SAT sat(segments.size());

        for (int i {}; i < (int) segments.size(); ++i) {
            for (int j {i + 1}; j < (int) segments.size(); ++j) {
                int ia {i * 2}, ja {j * 2};

                if (i % 2 == 0 && j == i + 1) {
                    sat.add_xor(ia, ja);
                } else if (intersect(segments[i].first, segments[i].second, segments[j].first, segments[j].second)) {
                    sat.add_or(ia ^ 1, ja ^ 1);
                }
            }
        }

        sat.has_ans() ? cout << "YES\n" : cout << "NO\n";
    }

    return 0;
}
