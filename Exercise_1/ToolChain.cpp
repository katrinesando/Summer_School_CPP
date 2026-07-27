#include <iostream>
#include <print>
using namespace std;

int main() {
    auto n = 9;
    println("Hello World!");
    
    auto fib = [&](this auto fib, int num, int prev1, int prev2) -> int {
        cout << prev1  << "  ";
        if (num <= 1) { return num ? prev2 : prev1; }
        auto current = prev1 + prev2;
       return fib(num - 1, prev2, current);
    };
    
    fib(n, 0, 1);
    return 0;
}