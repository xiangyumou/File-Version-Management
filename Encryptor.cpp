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

class Encryptor {
public:
    static const int N = 1 << 10;
    static const char PLACEHOLDER = '\0';

    struct Complex {
        double a, b;
        Complex() {}
        Complex(double a, double b) : a(a), b(b) {}
        Complex operator+(const Complex &r) const {
            return Complex(a + r.a, b + r.b);
        }
        Complex operator-(const Complex &r) const {
            return Complex(a - r.a, b - r.b);
        }
        Complex operator*(const Complex &r) const {
            return Complex(a * r.a - b * r.b, a * r.b + b * r.a);
        }
    } buf[N << 1], block[N];

    void fft(Complex a[], int n, int type) {
        const static double Pi = acos(-1.0);
        if (n == 1) return;
        int m = n >> 1;
        for (int i = 0; i < m; i++) {
            buf[i] = a[i << 1];
            buf[i + m] = a[i << 1 | 1];
        }
        memcpy(a, buf, sizeof(Complex) * n);
        Complex *a1 = a, *a2 = a + m;
        fft(a, m, type);
        fft(a + m, m, type);
        Complex wn = Complex(1, 0), u = Complex(cos(2 * Pi / n), type * sin(2 *Pi / n));
        for (int i = 0; i < m; i++) {
            Complex t = wn * a[m + i];
            wn = wn * u;
            buf[i] = a[i] + t;
            buf[i + m] = a[i] - t;
        }
        memcpy(a, buf, sizeof(Complex) * n);
    }

    bool encrypt_block(std::vector<std::pair<double, double>> &res) {
        fft(block, N, 1);
        res.clear();
        for (int i = 0; i < N; i++) {
            res.push_back(std::make_pair(block[i].a, block[i].b));
        }
        return true;
    }

    bool decrypt_block(std::vector<int> &res) {
        fft(block, N, -1);
        res.clear();
        for (int i = 0; i < N; i++) {
            res.push_back((int)(block[i].a / N + 0.5));
            if (block[i].a < 0.0 && std::abs(block[i].a) > 1e-2) res.back()--;
        }
        return true;
    }

    bool encrypt_sequence(std::vector<int> &sequence, std::vector<std::pair<double, double>> &res) {
        int len = sequence.size(), idx = 1;
        while (sequence.size() % N != 0) sequence.push_back(PLACEHOLDER);
        memset(block, 0, sizeof block);
        block[0].a = len;
        res.clear();
        std::vector<std::pair<double, double>> tmp;
        for (auto &it : sequence) {
            block[idx++].a = it;
            if (idx == N) {
                encrypt_block(tmp);
                res.insert(res.end(), tmp.begin(), tmp.end());
                idx = 0;
                memset(block, 0, sizeof block);
                tmp.clear();
            }
        }
        return true;
    }

    bool decrypt_sequence(std::vector<std::pair<double, double>> &sequence, std::vector<int> &res) {
        if (sequence.size() % N != 0) return false;
        memset(block, 0, sizeof block);
        int idx = 0;
        res.clear();
        int len = -1;
        std::vector<int> tmp;
        for (auto &it : sequence) {
            block[idx].a = it.first;
            block[idx++].b = it.second;
            if (idx == N) {
                decrypt_block(tmp);
                if (len != -1) {
                    res.insert(res.end(), tmp.begin(), tmp.end());
                } else {
                    len = tmp.front();
                    res.insert(res.end(), tmp.begin() + 1, tmp.end());
                }
                idx = 0;
                memset(block, 0, sizeof block);
                tmp.clear();
            }
        }
        res.erase(res.begin() + len, res.end());
        return true;
    }
} ec;

void test_fft() {
    Encryptor::Complex a[1 << 10];
    memset(a, 0, sizeof a);
    for (int i = 0; i < 10; i++) {
        a[i].a = i - 5;
    }
    ec.fft(a, 1 << 10, 1);
    ec.fft(a, 1 << 10, -1);
    for (int i = 0; i < 20; i++) {
        int t = a[i].a / (1 << 10) + 0.5;
        if (a[i].a < 0.0 && std::fabs(a[i].a) > 1e-2) t--;
        std::cout << t << ' ';
    }
}

int main() {
    srand(time(0));
    std::vector<int> a, aa;
    std::vector<std::pair<double, double>> b;
    for (int i = 0; i < 10; i++) {
        a.push_back(rand() - rand());
        aa.push_back(a.back());
    }
    ec.encrypt_sequence(a, b);
    ec.decrypt_sequence(b, a);
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != aa[i]) {
            std::cout << "wrong";
            return 0;
        }
    }
    return 0;
}