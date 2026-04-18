#include <cassert>
#include <type_traits>

template<int P, int D>
struct _IsPrime: std::conditional_t<P % D == 0, std::integral_constant<bool, false>, _IsPrime<P, D - 1>>{};
template<int P>
struct _IsPrime<P, 1>: std::integral_constant<bool, true>{};

template<int P>
struct IsPrime: _IsPrime<P, P - 1> {};
template<>
struct IsPrime<1>: std::integral_constant<bool, false>{};
template<>
struct IsPrime<2>: std::integral_constant<bool, true>{};

template<int P>
struct NextPrime: std::conditional_t<IsPrime<P + 1>::value, std::integral_constant<int, P + 1>, NextPrime<P + 1>> {};

template<int N, int curr_prime = 2, int curr_N = 1>
struct PrimeNumber: std::conditional_t<N == curr_N, std::integral_constant<int, curr_prime>, PrimeNumber<N, NextPrime<curr_prime>::value, curr_N + 1>>{};

int main() {
    assert(PrimeNumber<1>{} == 2);
    assert(PrimeNumber<9>{} == 23);
}
