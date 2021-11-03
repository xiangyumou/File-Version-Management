#include <cstring>
#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

void solve() {
    std::stringstream ss;
    ss << 123 << " 123123 123  123" << '\n';
    std::string s;
    std::getline(ss, s);
    std::cout << s.back() << '\n';
    std::cout << s << '\n';
}

int main() {
    solve();
    return 0;
}