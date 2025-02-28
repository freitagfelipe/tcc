#include <bits/stdc++.h>

using namespace std;

using Clause = deque<int>;
using Clauses = vector<Clause>;

struct Formula {
    int clauses_cnt {};
    unordered_map<int, unordered_set<int>> literals_to_clauses_idx {};
    unordered_set<int> remaining_literals;
    unordered_map<int, int> assignment;
    Clauses clauses {};

    void add_unit_clause(int literal) {
        this->clauses.emplace_back(deque<int> {literal});
        this->literals_to_clauses_idx[literal].insert(this->clauses.size() - 1);
        this->remaining_literals.insert(literal);
        this->clauses_cnt += 1;
    }

    void add_or(int literal1, int literal2) {
        this->clauses.emplace_back(deque<int> {literal1, literal2});

        if (literal1 == literal2) {
            this->literals_to_clauses_idx[literal1].insert(this->clauses.size() - 1);
            this->remaining_literals.insert(literal1);
        } else {
            this->literals_to_clauses_idx[literal1].insert(this->clauses.size() - 1);
            this->literals_to_clauses_idx[literal2].insert(this->clauses.size() - 1);
            this->remaining_literals.insert({literal1, literal2});
        }

        this->clauses_cnt += 1;
    }

    void build() {
        for (const Clause &clause : this->clauses) {
            for (const int literal : clause) {
                this->literals_to_clauses_idx[literal].insert(this->clauses.size() - 1);
            }
        }
    }
};

bool unit_propagation(Formula &formula) {
    unordered_set<int> unit_queue;

    int i {};

    for (Clause &clause : formula.clauses) {
        if (clause.size() == 1) {
            unit_queue.insert(clause[0]);
            clause.clear();
        }

        ++i;
    }

    while (!unit_queue.empty()) {
        int literal {*unit_queue.begin()};

        unit_queue.erase(unit_queue.begin());

        int atom {abs(literal)};
        bool value {literal > 0};

        // Se a variável já foi atribuída com um valor diferente, a fórmula é insatisfatível
        if (formula.assignment.find(atom) != formula.assignment.end() && formula.assignment[atom] != value) {
            return false;
        }

        formula.assignment[atom] = value;

        formula.remaining_literals.erase(literal);
        formula.remaining_literals.erase(-literal);

        // Englobamento unitário
        {
            for (int clause_idx : formula.literals_to_clauses_idx[literal]) {
                formula.clauses_cnt -= 1;
                // No máximo duas iterações
                for (int literal_to_remove_clause : formula.clauses[clause_idx]) {
                    if (literal_to_remove_clause == literal) {
                        continue;
                    }

                    formula.literals_to_clauses_idx[literal_to_remove_clause].erase(clause_idx);
                }
            }
        }

        // Resolução unitária
        {
            for (int clause_idx : formula.literals_to_clauses_idx[-literal]) {
                if (formula.clauses[clause_idx].size() == 1) {
                    return false;
                }

                if (formula.clauses[clause_idx][0] == formula.clauses[clause_idx][1]) {
                    return false;
                }

                if (formula.clauses[clause_idx][0] == -literal) {
                    unit_queue.insert(formula.clauses[clause_idx][1]);
                    formula.clauses[clause_idx].clear();
                } else {
                    unit_queue.insert(formula.clauses[clause_idx][0]);
                    formula.clauses[clause_idx].clear();
                }
            }
        }
    }

    return true;
}

bool solver(Formula &formula) {
    while (formula.clauses_cnt != 0) {
        int literal {*formula.remaining_literals.begin()};
        
        Formula f = formula;

        f.add_unit_clause(literal);

        bool unsatisfiable {!unit_propagation(f)};

        if (unsatisfiable) {
            f = formula;
            f.add_unit_clause(-literal);

            unsatisfiable = !unit_propagation(f);

            if (unsatisfiable) {
                return false;
            }
        }

        formula = f;
    }

    return true;
}