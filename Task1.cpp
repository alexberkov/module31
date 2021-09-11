#include <iostream>

using namespace std;

class Toy {
private:
    string name;
public:
    Toy() : name("Toy") {};
    string getName() {return name; };
    explicit Toy(string s) : name(std::move(s)) {};
};

class shared_ptr_toy {
private:
    Toy* ptr;
    int* count;
    void reset() {
      delete ptr;
      delete count;
      cout << "This toy has no refs." << endl;
    }
public:
    shared_ptr_toy() {
      ptr = nullptr;
      count = new int;
      *count = 0;
    }

    explicit shared_ptr_toy(const string &s) {
      ptr = new Toy(s);
      count = new int;
      *count = 1;
    }

    shared_ptr_toy(shared_ptr_toy &other) {
      ptr = other.ptr;
      count = other.count;
      ++(*count);
    }

    shared_ptr_toy& operator=(const shared_ptr_toy &other) {
      if (this == &other) return *this;
      else {
        if (ptr != nullptr) {
          if (--(*count) == 0) this->reset();
        }
        ptr = other.ptr;
        count = other.count;
        ++(*count);
        return *this;
      }
    }

    void print() {
      if (ptr == nullptr) cout << "Name: None, Refs: " << *count << endl;
      else cout << "Name: " << ptr->getName() << ", Refs: " << *count << endl;
    }

    ~shared_ptr_toy() {
      if (--(*count) < 1) this->reset();
    }
};

shared_ptr_toy make_shared_toy(const string &s) {
  shared_ptr_toy p(s);
  return p;
}

shared_ptr_toy make_shared_toy(Toy &t) {
  shared_ptr_toy p(t.getName());
  return p;
}

int main() {
  shared_ptr_toy ball, bone;
  ball.print();
  bone.print();

  ball = make_shared_toy("Ball");
  cout << endl;
  ball.print();

  shared_ptr_toy another_ball (ball);
  cout << endl;
  ball.print();
  another_ball.print();

  Toy t ("Bone");
  bone = make_shared_toy(t);
  cout << endl;
  bone.print();

  shared_ptr_toy another_bone = bone;
  cout << endl;
  bone.print();
  another_bone.print();

  another_bone = ball;
  cout << endl;
  ball.print();
  bone.print();
  another_bone.print();

  another_ball = bone;
  cout << endl;
  ball.print();
  bone.print();
  another_ball.print();
  cout << endl;
}