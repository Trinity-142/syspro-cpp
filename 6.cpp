#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class SquareMatrix {
    size_t _size;
    vector<T> _data;
public:
    SquareMatrix(size_t size): _size(size), _data(vector<T>(size * size, 0)) {}
    SquareMatrix(vector<T>& diag): _size(diag.size()), _data(vector<T>(_size * _size, 0)) {
        for (size_t i = 0; i < _size; i++) {
            (*this)[i][i] = diag[i];
        }
    }

    SquareMatrix& operator+=(const SquareMatrix& other) {
        assert(_size == other._size);
        for (size_t i = 0; i < _size; i++) {
            for (size_t j = 0; j < _size; j++) {
                (*this)[i][j] += other[i][j];
            }
        }
        return *this;
    }
    SquareMatrix& operator+=(const double scalar) {
        for (size_t i = 0; i < _size; i++) {
            for (size_t j = 0; j < _size; j++) {
                (*this)[i][j] += scalar;
            }
        }
        return *this;
    }

    SquareMatrix operator+(const SquareMatrix& other) const {
        assert(_size == other._size);
        SquareMatrix result(*this);
        result += other;
        return result;
    }
    SquareMatrix operator+(const double scalar) const {
        SquareMatrix result(*this);
        result += scalar;
        return result;
    }

    SquareMatrix& operator*=(const SquareMatrix& other) {
        assert(_size == other._size);
        SquareMatrix result(_size);
        for (size_t i = 0; i < _size; i++) {
            for (size_t j = 0; j < _size; j++) {
                for (size_t k = 0; k < _size; k++) {
                    result[i][j] += (*this)[i][k] * other[k][j];
                }
            }
        }
        *this = result;
        return *this;
    }
    SquareMatrix& operator*=(const double scalar) {
        for (size_t i = 0; i < _size; i++) {
            for (size_t j = 0; j < _size; j++) {
                (*this)[i][j] *= scalar;
            }
        }
        return *this;
    }

    SquareMatrix operator*(const SquareMatrix& other) const {
        assert(_size == other._size);
        SquareMatrix result(*this);
        result *= other;
        return result;
    }
    SquareMatrix operator*(const double scalar) const {
        SquareMatrix result(*this);
        result *= scalar;
        return result;
    }

    bool operator==(const SquareMatrix& other) const {
        return _data == other._data;
    }

    bool operator!=(const SquareMatrix& other) const {
        return !(*this == other);
    }

    T* operator[](size_t i) {
        return &_data[i * _size];
    }
    const T* operator[](size_t i) const {
        return &_data[i * _size];
    }

    explicit operator double() const {
        T result = 0;
        for (size_t i = 0; i < _size; i++) {
            for (size_t j = 0; j < _size; j++) {
                result += (*this)[i][j];
            }
        }
        return result;
    }
};

int main() {
    // diag ctr
    vector<double> diag = {2, 3};
    SquareMatrix<double> m1(diag);
    assert(m1[0][0] == 2 && m1[0][1] == 0 && m1[1][0] == 0 && m1[1][1] == 3);

    // add
    SquareMatrix<double> m2(2);
    m2[0][0] = 1; m2[0][1] = 1;
    m2[1][0] = 1; m2[1][1] = 1;

    SquareMatrix<double> sum = m1 + m2;
    assert(sum[0][0] == 3 && sum[0][1] == 1 && sum[1][0] == 1 && sum[1][1] == 4);

    m2 += 5;
    assert(m2[0][0] == 6 && m2[0][1] == 6 && m2[1][0] == 6 && m2[1][1] == 6);

    // mul
    SquareMatrix<double> A(2);
    A[0][0] = 1; A[0][1] = 2;
    A[1][0] = 3; A[1][1] = 4;
    SquareMatrix<double> B(2);
    B[0][0] = 2; B[0][1] = 0;
    B[1][0] = 1; B[1][1] = 2;
    SquareMatrix<double> C = A * B;
    assert(C[0][0] == 4 && C[0][1] == 4 && C[1][0] == 10 && C[1][1] == 8);

    SquareMatrix<double> D(2);
    D[0][0] = 1; D[0][1] = 1;
    D[1][0] = 1; D[1][1] = 1;
    D *= D;
    assert(D[0][0] == 2 && D[0][1] == 2 && D[1][0] == 2 && D[1][1] == 2);

    D *= 0.5;
    assert(D[0][0] == 1);

    // comparisons
    assert(D == m2 + (-5));
    assert(D != C);

    // type conversion to double
    auto total = static_cast<double>(C);
    assert(total == 26);

    // chain add
    SquareMatrix<double> X(2), Y(2), Z(2), res(2);
    X += 1;
    Y += 2;
    Z += 3;
    res += 6;
    SquareMatrix<double> chain_sum = X + Y + Z;
    assert(chain_sum == res);

    X += Y += Z;
    assert(Y[0][0] == 5);
    assert(X[0][0] == 6);

    // chain mul
    SquareMatrix<double> M_A(2), M_B(2), M_C(2);
    M_A[0][0] = 1; M_A[0][1] = 2; M_A[1][0] = 0; M_A[1][1] = 1;
    M_B[0][0] = 1; M_B[0][1] = 3; M_B[1][0] = 0; M_B[1][1] = 1;
    M_C[0][0] = 1; M_C[0][1] = 4; M_C[1][0] = 0; M_C[1][1] = 1;

    SquareMatrix<double> chain_mul = M_A * M_B * M_C;
    assert(chain_mul[0][0] == 1 && chain_mul[0][1] == 9);
    assert(chain_mul[1][0] == 0 && chain_mul[1][1] == 1);

    (M_A *= M_B) *= M_C;
    assert(M_A == chain_mul);
}
