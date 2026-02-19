#include <iostream>
#include <optional>

struct Point {
    float x, y;
    Point(float x, float y): x(x), y(y) {}
};

class Line {
    float a, b, c;

public:
    Line(float a, float b, float c) : a(a), b(b), c(c) {}
    Line(const Point& p1, const Point& p2) :
        a(p1.y - p2.y),
        b(p2.x - p1.x),
        c(p1.x * p2.y - p1.y * p2.x)
    {}

    std::optional<Point> intersection(const Line& other) const {
        float det = a * other.b - b * other.a;
        if (det == 0) return std::nullopt;

        float x = (b * other.c - other.b * c) / det;
        float y = (c * other.a - other.c * a) / det;

        return Point{x, y};
    }

    Line perpendicular(const Point& point) const {
        float a = -this->b;
        float b = this->a;
        float c = -(b * point.x + a * point.y);
        return {a, b, c};
    }

    friend std::ostream& operator<<(std::ostream& os, const Line& line) {
        if (line.b == 0) {
            os << "x = " << -line.c / line.a << "\n";
            return os;
        }
        float k = -line.a / line.b + 0.0f;
        float b = -line.c / line.b;
        std::string kx = (k != 0) ? (std::to_string(k) + "x") : ("");
        os << "y = " << k << "x " << (b < 0 ? "- " : "+ ") << std::abs(b) << "\n";
        return os;
    }
};


int main() {
    Line vertical({2, 0}, {2, 10});
    Line horizontal(0, 1, -5);
    std::cout << vertical << horizontal;
    auto inter = vertical.intersection(horizontal);

    if (inter) {
        std::cout << "Intersection: (" << inter->x << ", " << inter->y << ")\n";
    }

    Line line({5, 0}, {3.34, -2.34});
    std::cout << line;
    Line perp = line.perpendicular({1, 1});
    std::cout << perp;
    auto inter_perp = line.intersection(perp);
    if (inter_perp) {
        std::cout << "Perpendicular intersection: (" << inter_perp->x << ", " << inter_perp->y << ")\n";
    }

    return 0;
}