import numpy as np

def power_to_db(S, ref=1.0, amin=1e-10, top_db=80.0):
    """
    Convert a power spectrogram (amplitude**2) to decibel (dB) units.

    Parameters:
    - S: np.ndarray [shape=(d, t)] — Power spectrogram
    - ref: float or callable — Reference value
    - amin: float — Minimum threshold for numerical stability
    - top_db: float — Threshold the output at `top_db` below the peak

    Returns:
    - S_db: np.ndarray — Power spectrogram in dB scale
    """
    S = np.asarray(S)
    if callable(ref):
        ref_value = ref(S)
    else:
        ref_value = ref

    log_spec = 10.0 * np.log10(np.maximum(amin, S))
    log_spec -= 10.0 * np.log10(np.maximum(amin, ref_value))

    if top_db is not None:
        log_spec = np.maximum(log_spec, log_spec.max() - top_db)

    return log_spec