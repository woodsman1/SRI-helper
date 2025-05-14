import numpy as np

def stft(y, n_fft=2048, hop_length=None, win_length=None, window='hann', center=True, pad_mode='reflect'):
    if hop_length is None:
        hop_length = n_fft // 4
    if win_length is None:
        win_length = n_fft

    # Create the window function
    if isinstance(window, str):
        window = np.hanning(win_length)
    elif callable(window):
        window = window(win_length)
    else:
        window = np.asarray(window)

    # Pad the signal if center=True
    if center:
        pad = n_fft // 2
        y = np.pad(y, pad, mode=pad_mode)

    # Compute the number of frames
    num_frames = 1 + (len(y) - n_fft) // hop_length

    # Pre-allocate STFT matrix
    stft_matrix = np.empty((n_fft // 2 + 1, num_frames), dtype=np.complex64)

    for i in range(num_frames):
        start = i * hop_length
        frame = y[start:start + n_fft]

        # Apply window
        frame_windowed = window * frame[:win_length]
        # Zero-pad if win_length < n_fft
        if win_length < n_fft:
            frame_windowed = np.pad(frame_windowed, (0, n_fft - win_length), mode='constant')

        # FFT and keep only the positive frequencies
        fft_result = np.fft.rfft(frame_windowed)
        stft_matrix[:, i] = fft_result

    return stft_matrix