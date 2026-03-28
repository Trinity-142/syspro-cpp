#include <cassert>
#include <string>

template <typename T>
class ScopedPointer {
    T* pointer = nullptr;
public:
    ScopedPointer(T* raw): pointer(raw) {}
    ScopedPointer(const ScopedPointer& other) {
        if (other.pointer) pointer = new T(*other.pointer);
    }

    ScopedPointer(ScopedPointer&& other) {
        swap(pointer, other.pointer);
    }

    ScopedPointer& operator=(ScopedPointer other) {
        swap(pointer, other.pointer);
        return *this;
    }

    T& operator*() { return *pointer; }
    const T& operator*() const { return *pointer; }
    T* operator->() { return pointer; }
    const T* operator->() const { return pointer; }

    ~ScopedPointer() { delete pointer; }
};

template <typename T>
class ScopedPointerUnique {
    T* pointer = nullptr;
public:
    ScopedPointerUnique(T* raw): pointer(raw) {}

    ScopedPointerUnique(const ScopedPointerUnique& other) = delete;
    ScopedPointerUnique& operator=(const ScopedPointerUnique& other) = delete;

    ScopedPointerUnique(ScopedPointerUnique&& other) {
        swap(pointer, other.pointer);
    }

    ScopedPointerUnique& operator=(ScopedPointerUnique&& other) {
        swap(pointer, other.pointer);
        return *this;
    }

    T& operator*() { return *pointer; }
    const T& operator*() const { return *pointer; }
    T* operator->() { return pointer; }
    const T* operator->() const { return pointer; }

    ~ScopedPointerUnique() { delete pointer; }
};

int main() {
    // ==========================================
    // ScopedPointer
    // ==========================================
    ScopedPointer<std::string> p1(new std::string("hello"));

    // deep cpy ctr
    ScopedPointer<std::string> p2 = p1;
    assert(*p2 == "hello");
    assert(&*p1 != &*p2);

    // self cpy =
    p1 = p1;
    assert(*p1 == "hello");

    // deep cpy =
    ScopedPointer<std::string> p3(new std::string("world"));
    p3 = p1;
    assert(*p3 == "hello");
    assert(&*p3 != &*p1);

    // move ctr
    ScopedPointer<std::string> p4 = std::move(p2);
    assert(*p4 == "hello");

    // move =
    ScopedPointer<std::string> p5(new std::string("test"));
    p5 = std::move(p4);
    assert(*p5 == "hello");

    // self move =
    p5 = std::move(p5);
    assert(*p5 == "hello");

    // ==========================================
    // ScopedPointerUnique
    // ==========================================
    ScopedPointerUnique<std::string> u1(new std::string("unique"));

    // cpy ctr prohibited
    //ScopedPointerUnique<std::string> u_copy = u1;
    //ScopedPointerUnique<std::string> u_assign(new std::string(""));
    //u_assign = u1;

    // move ctr
    ScopedPointerUnique<std::string> u2 = std::move(u1);
    assert(*u2 == "unique");

    // move =
    ScopedPointerUnique<std::string> u3(new std::string("pointer"));
    u3 = std::move(u2);
    assert(*u3 == "unique");

    // self move =
    u3 = std::move(u3);
    assert(*u3 == "unique");
}
