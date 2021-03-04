#include <iostream>
#include <type_traits>

int main()
{
    std::cout << std::is_trivially_copyable<int>::value << '\n';
}


