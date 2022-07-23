#include <vector>
#include <string>
#include <algorithm>

#ifndef OCEANBASE_INTERN_
#define OCEANBASE_INTERN_

using namespace std;

class OceanbaseIntern {
public:
    static int32_t solution1(string ip);
    static void solution2(vector<vector<int>> &res, vector<int> &numbers, vector<int> &curNumber, int index, int remain);
};

#endif