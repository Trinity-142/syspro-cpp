#include <cassert>
#include <iostream>
#include <cstring>
#include <type_traits>

template<size_t SIZE, typename... Types>
concept EnoughMemory = (sizeof(Types) + ...) <= SIZE;

template<typename... Types>
concept AllCopyConstructable = (std::is_copy_constructible_v<Types> && ...);

template<size_t SIZE, typename... Types>
requires EnoughMemory<SIZE, Types...> && AllCopyConstructable<Types...>
void allocate(void* memory, Types... args) {
    auto ptr = static_cast<char *>(memory);
    auto copy = [&ptr]<typename T>(const T& arg) {
        new (ptr) T(arg);
        ptr += sizeof(T);
    };
    (copy(args), ...);
}

int main() {
    constexpr size_t SIZE = 17;
    char static_arr[SIZE];
    int a;
    std::cin >> a;
    allocate<SIZE>(static_arr, a, 4, 'a', "hi");

    int val1 = *reinterpret_cast<int*>(static_arr);
    assert(val1 == a);
    int val2 = *reinterpret_cast<int*>(static_arr + sizeof(int));
    assert(val2 == 4);
    char val3 = static_arr[sizeof(int) * 2];
    assert(val3 == 'a');
    const char* val4 = *reinterpret_cast<const char**>(static_arr + sizeof(int) * 2 + sizeof(char));
    assert(std::strcmp(val4, "hi") == 0);
}
