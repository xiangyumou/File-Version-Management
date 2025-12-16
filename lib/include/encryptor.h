/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <vector>
#include <utility>

/**
 * @brief Complex number structure for FFT operations
 */
struct Complex {
    double a, b;

    Complex();
    Complex(double a, double b);

    Complex operator+(const Complex& r) const;
    Complex operator-(const Complex& r) const;
    Complex operator*(const Complex& r) const;
};

/**
 * @brief Encryptor class using FFT for encryption/decryption
 * 
 * Converts polynomial coefficient representation to point-value representation
 * using discrete Fourier transform accelerated by FFT.
 */
class Encryptor {
public:
    static const int N = 1 << 10;

private:
    static const char PLACEHOLDER = '\0';
    Complex buf[N << 1], block[N];

    void fft(Complex a[], int n, int type);
    bool encrypt_block(std::vector<std::pair<double, double>>& res);
    bool decrypt_block(std::vector<int>& res);

protected:
    bool encrypt_sequence(std::vector<int>& sequence, std::vector<std::pair<double, double>>& res);
    bool decrypt_sequence(std::vector<std::pair<double, double>>& sequence, std::vector<int>& res);
};

#endif // ENCRYPTOR_H
