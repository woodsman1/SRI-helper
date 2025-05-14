#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

std::vector<std::complex<double>> rfft(const std::vector<double>& signal) {
    size_t N = signal.size();
    std::vector<std::complex<double>> output;
    output.reserve(N / 2 + 1);

    for (size_t k = 0; k <= N / 2; ++k) {
        double re = 0.0;
        double im = 0.0;
        for (size_t n = 0; n < N; ++n) {
            double angle = 2 * M_PI * k * n / N;
            re += signal[n] * std::cos(-angle);
            im += signal[n] * std::sin(-angle);
        }
        output.emplace_back(re, im);
    }
    return output;
}

// Example usage
int main() {
    std::vector<double> x = {0.0, 1.0, 0.0, -1.0};  // A 4-point real signal
    std::vector<std::complex<double>> result = rfft(x);

    for (const auto& val : result) {
        std::cout << val << std::endl;
    }

    return 0;
}