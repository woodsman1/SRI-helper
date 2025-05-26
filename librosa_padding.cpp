std::vector<float> y_padded;
y_padded.reserve(48000 + 2 * pad);

// Reflect padding at start
for (int i = pad - 1; i >= 0; --i)
    y_padded.push_back(y[i]);

// Original samples
for (int i = 0; i < 48000; ++i)
    y_padded.push_back(y[i]);

// Reflect padding at end
for (int i = 47999; i > 47999 - pad; --i)
    y_padded.push_back(y[i]);


n_frames = 1 + (len(y_padded) - n_fft) / hop_length

int n_frames = 1 + (48512 - 512) / 256;  // 1 + 48000 / 256 = 1 + 187 = 188

for (int i = 0; i < 188; ++i) {
    int start = i * hop_length;
    float frame[n_fft];

    // Copy and window the frame
    for (int j = 0; j < n_fft; ++j) {
        frame[j] = y_padded[start + j] * hann_window[j];
    }

    // Compute FFT (e.g., using KissFFT or FFTW)
    // Get power spectrum: mag^2
    // Apply mel filterbank (64 filters)
    // Convert to log scale (optional)
    // Result: 64×1 vector for this frame
}

