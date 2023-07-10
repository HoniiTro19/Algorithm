#include <string>
#include <vector>

#ifndef DEMO_H_
#define DEMO_H_

using namespace std;

class Demo {
public:
  Demo();
  ~Demo();
  vector<string> solution();
};

Demo::Demo() {}

Demo::~Demo() {}

vector<string> Demo::solution() {
  vector<string> msg{"Hello", "C++",     "World",
                     "from",  "VS Code", "and the C++ extension!"};
  return msg;
}
#endif