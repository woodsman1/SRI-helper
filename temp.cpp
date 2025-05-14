#include <iostream>

int main() {
    init_hanning_window();

    // Example signal (sine wave)
    constexpr int SIGNAL_LEN = 1024;
    float signal[SIGNAL_LEN];
    for (int i = 0; i < SIGNAL_LEN; ++i)
        signal[i] = 0.5f * std::sin(2 * M_PI * 440 * i / 22050);

    // Output STFT matrix
    std::complex<float> stft_output[MAX_FRAMES][N_FFT / 2 + 1];
    int num_frames;

    stft(signal, SIGNAL_LEN, stft_output, num_frames);

    // Print magnitude of first frame
    for (int i = 0; i < N_FFT / 2 + 1; ++i)
        std::cout << std::abs(stft_output[0][i]) << "\n";

    return 0;
}