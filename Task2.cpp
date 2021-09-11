#include <iostream>
#include <vector>

using namespace std;

vector<int> numVector(int n) {
  vector<int> vert;
  vert.reserve(n);
  for (int i = 0; i < n; i++) vert.push_back(i);
  return vert;
}

class Igraph {
public:
    Igraph() = default;
    Igraph(const Igraph& _oth) = default;
    virtual void print() = 0;
    virtual int verticesCount() const = 0;
    virtual vector<int>* getAdj() const = 0;
    virtual int** getMatrix() const = 0;
    virtual bool vertexExists(int vertex) const = 0;
    virtual void addEdge(int from, int to) = 0;
    virtual vector<int> Vertices() const = 0;
    virtual vector<int> GetNextVertices(int vertex) const = 0;
    virtual vector<int> GetPrevVertices(int vertex) const = 0;
    virtual void reset() = 0;
    virtual ~Igraph() {
      cout << "Deleted graph!" << endl;
    }
};

class ListGraph: public Igraph {
private:
    int v;
    vector<int>* Adj;
public:
    ListGraph(): v(0) {Adj = nullptr;};
    explicit ListGraph(int n);
    explicit ListGraph(Igraph *g);
    ListGraph& operator=(const Igraph *g);
    void print() override;
    int verticesCount() const override {return v;};
    vector<int>* getAdj() const override;
    int** getMatrix() const override;
    bool vertexExists(int vertex) const override { return (vertex > 0 && vertex < v); }
    void addEdge(int from, int to) override;
    vector<int> Vertices() const override { return numVector(v); }
    vector<int> GetNextVertices(int vertex) const override;
    vector<int> GetPrevVertices(int vertex) const override;
    void reset() override {
      delete []Adj;
      cout << "Deleted list!" << endl;
    }
    ~ListGraph() override {
      delete []Adj;
      cout << "Deleted list!" << endl;
    }
};

class MatrixGraph: public Igraph {
private:
    int v;
    int** u;
public:
    MatrixGraph(): v(0) {u = nullptr;};
    explicit MatrixGraph(int n);
    explicit MatrixGraph(Igraph *g);
    MatrixGraph& operator=(const Igraph *g);
    void print() override;
    int verticesCount() const override {return v;};
    vector<int>* getAdj() const override;
    int** getMatrix() const override;
    bool vertexExists(int vertex) const override { return (vertex > 0 && vertex < v); }
    void addEdge(int from, int to) override;
    vector<int> Vertices() const override { return numVector(v); }
    vector<int> GetNextVertices(int vertex) const override;
    vector<int> GetPrevVertices(int vertex) const override;
    void reset() override {
      delete *u;
      delete u;
      cout << "Deleted matrix!" << endl;
    }
    ~MatrixGraph() override {
      delete *u;
      delete u;
      cout << "Deleted matrix!" << endl;
    }
};

ListGraph::ListGraph(int n): v(n) {
  Adj = new vector<int> [v];
  for (int i = 0; i < v; i++) {
    cout << "Enter vertices adjacent to " << i << ": " << endl;
    while (true) {
      int vert;
      cin >> vert;
      if (vert == -1) break;
      else Adj[i].push_back(vert);
    }
  }
}

MatrixGraph::MatrixGraph(int n): v(n) {
  u = new int* [n];
  for (int i = 0; i < n; i++) {
    u[i] = new int [n];
    for (int j = 0; j < n; j++) {
      cout << "(" << i << ", " << j << "): ";
      cin >> u[i][j];
    }
  }
}

vector<int> *ListGraph::getAdj() const {
  vector<int>* Edj;
  Edj = new vector<int> [v];
  for (int i = 0; i < v; i++) {
   for (int j : Adj[i]) Edj[i].push_back(j);
  }
  return Edj;
}

int **ListGraph::getMatrix() const {
  int** u = new int* [v];
  for (int i = 0; i < v; i++) {
    u[i] = new int [v];
    for (int j = 0; j < v; j++) u[i][j] = 0;
  }
  for (int i = 0; i < v; i++) {
    for (int j: Adj[i]) u[i][j] = 1;
  }
  return u;
}

int **MatrixGraph::getMatrix() const {
  int **e;
  e = new int* [v];
  for (int i = 0; i < v; i++) {
    e[i] = new int [v];
    for (int j = 0; j < v; j++) e[i][j] = u[i][j];
  }
  return e;
}

vector<int> *MatrixGraph::getAdj() const {
  vector<int>* Adj;
  Adj = new vector<int> [v];
  for (int i = 0; i < v; i++) {
    for (int j = 0; j < v; j++) {
      if (u[i][j] == 1) Adj[i].push_back(j);
    }
  }
  return Adj;
}

ListGraph::ListGraph(Igraph *g) {
  v = g->verticesCount();
  Adj = g->getAdj();
}

ListGraph &ListGraph::operator=(const Igraph *g) {
  if (this == g) return *this;
  else {
    this->reset();
    v = g->verticesCount();
    Adj = g->getAdj();
    return *this;
  }
}

void ListGraph::addEdge(int from, int to) {
  if (this->vertexExists(from) && this->vertexExists(to)) {
    bool ex = false;
    for (auto &i: Adj[from]) {
      if (i == to) cout << "Edge already exists!" << endl;
      ex = true;
      break;
    }
    if (!ex) Adj[from].push_back(to);
  } else cout << "Vertex does not exist!" << endl;
}

void MatrixGraph::addEdge(int from, int to) {
  if (this->vertexExists(from) && this->vertexExists(to)) {
    if (u[from][to] == 1) cout << "Edge already exists!" << endl;
    else u[from][to] = 1;
  } else cout << "Vertex does not exist!" << endl;
}

vector<int> ListGraph::GetNextVertices(int vertex) const {
  if (this->vertexExists(vertex)) return Adj[vertex];
  else {
    vector<int> vert;
    cout << "Vertex does not exist!" << endl;
    return vert;
  }
}

vector<int> MatrixGraph::GetNextVertices(int vertex) const {
  vector<int> vert;
  if (this->vertexExists(vertex)) {
    vector<int> *Edj = this->getAdj();
    for (auto &i: Edj[vertex]) vert.push_back(i);
    delete[]Edj;
  } else cout << "Vertex does not exist!" << endl;
  return vert;
}

vector<int> ListGraph::GetPrevVertices(int vertex) const {
  vector<int> prev;
  if (this->vertexExists(vertex)) {
    int **M = this->getMatrix();
    for (int i = 0; i < this->verticesCount(); i++) if (M[i][vertex] == 1) prev.push_back(i);
    delete *M;
    delete M;
  } else cout << "Vertex does not exist!" << endl;
  return prev;
}

vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
  vector<int> prev;
  if (this->vertexExists(vertex)) {
    for (int i = 0; i < this->verticesCount(); i++) if (u[i][vertex] == 1) prev.push_back(i);
  } else cout << "Vertex does not exist!" << endl;
  return prev;
}

void ListGraph::print() {
  for (int i = 0; i < v; i++) {
    cout << i << ": ";
    for (int j: Adj[i]) cout << j << " ";
    cout << endl;
  }
}

MatrixGraph::MatrixGraph(Igraph *g) {
  v = g->verticesCount();
  u = g->getMatrix();
}

MatrixGraph &MatrixGraph::operator=(const Igraph *g) {
  if (this == g) return *this;
  else {
    this->reset();
    v = g->verticesCount();
    u = g->getMatrix();
    return *this;
  }
}

void MatrixGraph::print() {
  cout << "  ";
  for (int i = 0; i < v; i++) cout << i << " ";
  cout << endl;
  for (int i = 0; i < v; i++) {
    cout << i << " ";
    for (int j = 0; j < v; j++) cout << u[i][j] << " ";
    cout << endl;
  }
}

int main() {
  Igraph *l = new ListGraph(6);
  Igraph *m = new MatrixGraph(6);
  cout << "Initial: " << endl;
  l->print();
  m->print();
  vector<int> vert = l->Vertices();
  for (auto &i: vert) cout << i << " ";
  cout << endl;

  vector<int> next = m->GetNextVertices(2);
  for (auto &i: next) cout << i << " ";
  l->addEdge(2, 3);
  next = l->GetNextVertices(2);
  for (auto &i: next) cout << i << " ";
  cout << endl;

  vector<int> prev = l->GetPrevVertices(1);
  for (auto &i: prev) cout << i << " ";
  m->addEdge(4, 1);
  prev = m->GetPrevVertices(1);
  for (auto &i: prev) cout << i << " ";
  cout << endl;

  MatrixGraph m1(l);
  MatrixGraph m2(m);
  ListGraph l1(m);
  ListGraph l2(l);
  cout << "New: " << endl;
  m1.print();
  l1.print();
  m2.print();
  l2.print();

  m1 = l;
  l1 = m;
  cout << "Changed: " << endl;
  m1.print();
  l1.print();

  delete l;
  delete m;
}
