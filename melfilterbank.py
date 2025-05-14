import math

def hz_to_mel(f):
    return 2595.0 * math.log10(1.0 + f / 700.0)

def mel_to_hz(m):
    return 700.0 * (10.0 ** (m / 2595.0) - 1.0)

def linspace(start, end, num):
    step = (end - start) / float(num - 1)
    return [start + step * i for i in range(num)]

def floor(x):
    return int(math.floor(x))

def mel_filterbank(sr=22050, n_fft=2048, n_mels=128, fmin=0.0, fmax=None):
    if fmax is None:
        fmax = sr / 2

    # Step 1: Compute mel-spaced points
    mel_min = hz_to_mel(fmin)
    mel_max = hz_to_mel(fmax)
    mel_points = linspace(mel_min, mel_max, n_mels + 2)
    hz_points = [mel_to_hz(m) for m in mel_points]

    # Step 2: Convert Hz to FFT bin numbers
    bin_points = [floor((n_fft + 1) * f / sr) for f in hz_points]

    # Step 3: Create filterbank
    filterbank = []
    for i in range(1, n_mels + 1):
        filter_row = [0.0] * (n_fft // 2 + 1)
        left = bin_points[i - 1]
        center = bin_points[i]
        right = bin_points[i + 1]

        # Rising slope
        for j in range(left, center):
            if center != left:
                filter_row[j] = (j - left) / float(center - left)

        # Falling slope
        for j in range(center, right):
            if right != center:
                filter_row[j] = (right - j) / float(right - center)

        filterbank.append(filter_row)

    # Step 4: Slaney-style normalization (optional)
    for i in range(len(filterbank)):
        hz_l = hz_points[i]
        hz_r = hz_points[i + 2]
        enorm = 2.0 / (hz_r - hz_l)
        filterbank[i] = [x * enorm for x in filterbank[i]]

    return filterbank