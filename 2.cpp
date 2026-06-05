#include <cassert>
#include <iostream>
#include <optional>

const double eps = 1e-5;

template <typename T, int N>
struct Counter {
    static int objects_alive;
    bool counted = true;

    Counter() {
        check_limit();
        ++objects_alive;
    }

    Counter(const Counter&) {
        check_limit();
        ++objects_alive;
    }

    Counter(Counter&& other)  noexcept {
        this->counted = other.counted;
        other.counted = false;
    }

protected:
    ~Counter() {
        if (counted) {
            --objects_alive;
        }
    }
private:
    void check_limit() {
        if (objects_alive >= N) {
            throw std::logic_error("Maximum number of alive objects exceeded!");
        }
    }
};

template <typename T, int N> int Counter<T, N>::objects_alive(0);

struct Point: Counter<Point, 4> {
    double x, y;
    Point(double x, double y): x(x), y(y) {}

public:
    friend bool operator==(const Point& p1, const Point& p2) {
        return std::abs(p1.x - p2.x) < eps && std::abs(p1.y - p2.y) < eps;
    }
};

class Line: Counter<Line, 3> {
    double a, b, c;
    Line(const Point& p1, const Point& p2) :
        a(p1.y - p2.y),
        b(p2.x - p1.x),
        c(p1.x * p2.y - p1.y * p2.x)
    {}
public:
    Line(double a, double b, double c) : a(a), b(b), c(c) {}

    static std::optional<Line> fabric(const Point& p1, const Point& p2) {
        if (p1 == p2) return {};
        return Line{p1, p2};
    }

    std::optional<Point> intersection(const Line& other) const {
        double det = a * other.b - b * other.a;
        if (std::abs(det) < eps) return std::nullopt;

        double x = (b * other.c - other.b * c) / det;
        double y = (c * other.a - other.c * a) / det;

        return Point{x, y};
    }

    Line perpendicular(const Point& point) const {
        double a = -this->b;
        double b = this->a;
        double c = -(b * point.x + a * point.y);
        return {a, b, c};
    }

    friend std::ostream& operator<<(std::ostream& os, const Line& line) {
        if (std::abs(line.b) < eps) {
            os << "x = " << -line.c / line.a << "\n";
            return os;
        }
        double k = -line.a / line.b + 0.0f;
        double b = -line.c / line.b;
        os << "y = " << k << "x " << (std::abs(b) < eps ? "- " : "+ ") << std::abs(b) << "\n";
        return os;
    }
};


int main() {
    Point p1{42, 17}; Point p2{42, 17};
    std::cout << Counter<Point, 4>::objects_alive << "\n";
    //Point p3{42, 2};
    Line vertical = Line::fabric({2, 0}, {2, 10}).value();
    Line line = Line::fabric({5, 0}, {3.34, -2.34}).value();
    Line perp = line.perpendicular({1, 1});
    std::cout << Counter<Line, 3>::objects_alive << "\n";
    Line abc = std::move(perp);
    std::cout << Counter<Line, 3>::objects_alive << "\n";
    //Line line2{1, 2 ,3};
}