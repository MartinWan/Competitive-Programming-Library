#include<list>
#include <vector>
using namespace std;
typedef long long num;

/*
 * Solves the 2-SAT problem in linear time.
 * Represented as a class since it is common to "build up"
 * the boolean formula across loops, functions etc.
 * Implementation based off of solution to competition problem F
 * from ACM-ICPC Pacific NW 2016 http://acmicpc-pacnw.org/ProblemSet/2016/div1.pdf
 */
class two_sat {
  private:
    int V;
    vector<vector<int>> G;
    vector<vector<int>> G_T;

    void visit(int u, list<int>& order, vector<bool>& visited) {
      if (!visited[u]) {
        visited[u] = true;

        for (int v : G[u]) {
          visit(v, order, visited);
        }

        order.push_front(u);
      }
    }

    void assign(int u, int color, vector<int>& component) {
      component[u] = color;
      for (auto i = 0u; i < G_T[u].size(); i++) {
        int v = G_T[u][i];
        if (component[v] == -1) assign(v, color, component);
      }
    }

    void add(int x, int y) { G[x].push_back(y); };

  public:
    /*
     * initializes 2-SAT problem with a maximum of n conjunctions
     * e.g. (x0 or y0) && (x1 or y1) && ... && (x(n-1) or y(n-1))
     */
    two_sat(int n) {
      V = 2*n;
      G = vector<vector<int>>(V);
      G_T = vector<vector<int>>(V);
    }

		/*
     * Returns true if the formula can be evaluated to true,
     * storing value of x[i] in values vector. False if formula
     * cannot be evaluated to true.
     */
    bool satisfiable(vector<bool>& values) {
      list<int> L; // contains vertices in reverse post order i.e. "topological" order
      vector<bool> visited(V);
      for (int v = 0; v < V; v++) visit(v, L, visited);

      // generate transpose of G
      for (int v = 0; v < V; v++) for (int u = 0; u < G[v].size(); u++)
        G_T[G[v][u]].push_back(v);

      // considering vertices in topological order, 
      // label strongly connected components using DFS on G transposed
      int color = 0;
      vector<int> component = vector<int>(V, -1);
      for (int u : L) {
        if (component[u] == -1) assign(u, color++, component);
      }

      for (int v = 0; v < V; v++)
        if (component[v] == component[v^1])  // satisfiable iff all xi and !xi separate components
          return false;
      for (int v = 0; v < V; v+=2)
        values[v/2] = (component[v] > component[v^1]); // xi == 1, iff xi comes after !xi in topological order

      return true;
    }

    /*
     * Add disjunction of form (a or b) to the SAT formula
     * if type = 0, add (a or b)
     * if type = 1, add (!a or b)
     * if type = 2, add (a or !b)
     * if type = 3, add (!a or !b)
     */
    void add_disjunction(int x, int y, int type) {
      int a=2*x, na=2*x+1, b=2*y, nb=2*y+1;
      switch (type) {
        case 0: add(na, b); add(nb, a); break; // a or b <=> !a -> b <=> !b -> a
        case 1: add(a, b); add(nb, na); break; // !a or b <=> a -> b <=> !b -> !a
        case 2: add(na, nb); add(b, a); break; // a or !b <=> !a -> !b <=> b -> a
        case 3: add(a, nb); add(b, na); break; // !a or !b <=> a -> !b <=> b -> !a
        default: throw logic_error("add_disjunction() requires type to be 0, 1, 2, or 3");
      }
    }
};

