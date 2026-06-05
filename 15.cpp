#include <vector>

#include "14.cpp"
#include "2.cpp"

template<typename... Types>
class Container {
    size_t size{};
    char* data{};
    std::vector<size_t> types;

public:
    Container(Types... args): size((sizeof(Types) + ... + 0)), data(new char[size]), types{sizeof(Types)...} {
        allocate<(sizeof(Types) + ... + 0)>(data, args...);
    }
    ~Container() { delete[] data; }

    Container(const Container& other) = delete;
    Container& operator=(const Container& other) = delete;

    Container(Container&& other) {
        std::swap(size, other.size);
        std::swap(data, other.data);
        std::swap(types, other.types);
    }
    Container& operator=(Container&& other) {
        std::swap(size, other.size);
        std::swap(data, other.data);
        std::swap(types, other.types);
        return *this;
    }

    template<typename T>
    T getElement(const size_t idx) {
        assert(sizeof(T) == types[idx]);
        size_t offset = 0;
        for (size_t i = 0; i < idx; ++i) offset += types[i];
        return *reinterpret_cast<T*>(data + offset);
    }
};

int main() {
    Container c(12, 'c', Point{3, 4});
    Container c2;
    Container c3((std::move(c)));
    std::cout << c3.getElement<int>(0) << std::endl;
    std::cout << c3.getElement<char>(1) << std::endl;
    std::cout << c3.getElement<Point>(2) << std::endl;
}
