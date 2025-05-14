#include <cmath>
#include <cstdint>

// === Parameters (Adjust to fit RAM constraints) ===
constexpr int N_FFT = 256;
constexpr int HOP_LENGTH = 64;
constexpr int WIN_LENGTH = 256;
constexpr int MAX_FRAMES = 32;  // Depends on input size

// === Precomputed window ===
float hanning_window[WIN_LENGTH];

void init_hanning_window() {
    for (int i = 0; i < WIN_LENGTH; ++i)
        hanning_window[i] = 0.5f - 0.5f * std::cos(2 * M_PI * i / (WIN_LENGTH - 1));
}

// === Placeholder FFT (replace with actual implementation) ===
void fft_real(const float* input, std::complex<float>* output, int n_fft) {
    // Replace with a real FFT (e.g., KissFFT, CMSIS-DSP, or your own)
    // Stub: zero output
    for (int i = 0; i < n_fft / 2 + 1; ++i)
        output[i] = {0.0f, 0.0f};
}

// === STFT implementation ===
void stft(const float* signal, int signal_len,
          std::complex<float> output[][MAX_FRAMES], int& num_frames_out) {

    int num_frames = (signal_len - N_FFT) / HOP_LENGTH + 1;
    if (num_frames > MAX_FRAMES) num_frames = MAX_FRAMES;

    static float frame[N_FFT];
    static float windowed[N_FFT];

    for (int frame_idx = 0; frame_idx < num_frames; ++frame_idx) {
        int start = frame_idx * HOP_LENGTH;

        // Load frame and zero-pad
        for (int i = 0; i < N_FFT; ++i) {
            frame[i] = (start + i < signal_len) ? signal[start + i] : 0.0f;
        }

        // Apply window
        for (int i = 0; i < N_FFT; ++i) {
            if (i < WIN_LENGTH)
                windowed[i] = frame[i] * hanning_window[i];
            else
                windowed[i] = 0.0f;
        }

        // Perform FFT
        fft_real(windowed, output[frame_idx], N_FFT);
    }

    num_frames_out = num_frames;
}





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