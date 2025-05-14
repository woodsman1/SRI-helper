import numpy as np

# === Parameters ===
N_FFT = 256
HOP_LENGTH = 64
WIN_LENGTH = 256
MAX_FRAMES = 32  # for resource-constrained simulation

# === Generate Hanning window ===
def init_hanning_window(win_length):
    return 0.5 - 0.5 * np.cos(2 * np.pi * np.arange(win_length) / (win_length - 1))

hanning_window = init_hanning_window(WIN_LENGTH)

# === STFT function (C++ logic translated) ===
def stft(signal, n_fft=N_FFT, hop_length=HOP_LENGTH, win_length=WIN_LENGTH, max_frames=MAX_FRAMES):
    signal_len = len(signal)
    num_frames = (signal_len - n_fft) // hop_length + 1
    num_frames = min(num_frames, max_frames)

    # Output: [frames][n_fft//2 + 1]
    stft_output = np.zeros((num_frames, n_fft // 2 + 1), dtype=np.complex64)

    for frame_idx in range(num_frames):
        start = frame_idx * hop_length
        frame = np.zeros(n_fft, dtype=np.float32)

        # Load frame and zero-pad
        end = min(start + n_fft, signal_len)
        frame[:end - start] = signal[start:end]

        # Apply window
        windowed = np.zeros_like(frame)
        windowed[:win_length] = frame[:win_length] * hanning_window

        # FFT and store positive frequencies
        fft_result = np.fft.rfft(windowed)
        stft_output[frame_idx, :] = fft_result

    return stft_output
    
    
if __name__ == "__main__":
    # Generate test signal: 440 Hz sine wave
    sr = 22050
    duration = 1024 / sr
    t = np.linspace(0, duration, 1024, endpoint=False)
    signal = 0.5 * np.sin(2 * np.pi * 440 * t)

    # Run STFT
    output = stft(signal)

    # Print magnitude of first frame
    print("Magnitude of first frame:")
    print(np.abs(output[0]))
