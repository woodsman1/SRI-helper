Perfect — let’s build your causal RNN step by step. It will:
	•	Take chunks of 512 audio samples
	•	Convert each to MFCCs
	•	Maintain a buffer of past chunks
	•	Feed that sequence to a causal RNN (GRU)
	•	Predict snoring in real time

⸻

🧱 Overall Architecture

Microphone Audio (16 kHz)
        ↓
512-sample chunk (~32ms)
        ↓
MFCC features (e.g. 13 coefficients)
        ↓
Sliding window buffer (e.g. last 20 chunks)
        ↓
[20, 13] MFCC sequence → Causal RNN (GRU)
        ↓
Snoring prediction (0 or 1)


⸻

✅ Step-by-Step Code (in TensorFlow)

⸻

📌 1. Extract MFCC from 512-sample chunk

import librosa
import numpy as np

def extract_mfcc(audio_chunk, sr=16000, n_mfcc=13):
    mfcc = librosa.feature.mfcc(
        y=audio_chunk, sr=sr, n_mfcc=n_mfcc, 
        n_fft=512, hop_length=512, center=False
    )
    return mfcc.T[0]  # Shape: (13,) — one vector for 512 chunk


⸻

📌 2. Create Sliding Window Buffer

from collections import deque

sequence_length = 20  # You can tune this
mfcc_buffer = deque(maxlen=sequence_length)

def update_buffer(mfcc_vector):
    mfcc_buffer.append(mfcc_vector)
    if len(mfcc_buffer) == sequence_length:
        return np.array(mfcc_buffer)  # Shape: [20, 13]
    return None


⸻

📌 3. Define a Causal RNN (GRU) Model

import tensorflow as tf

class CausalSnoreRNN(tf.keras.Model):
    def __init__(self, input_size=13, hidden_size=32):
        super(CausalSnoreRNN, self).__init__()
        self.rnn = tf.keras.layers.GRU(hidden_size, return_sequences=False)
        self.fc = tf.keras.layers.Dense(1, activation='sigmoid')

    def call(self, x):
        return self.fc(self.rnn(x))  # Input shape: [batch, time, features]

✅ The model is causal because:
	•	GRU is unidirectional
	•	It only processes inputs from past → present
	•	It does not use bidirectional layers

⸻

📌 4. Simulate Real-Time Prediction Loop

model = CausalSnoreRNN()
# For now, run this once to build the model:
dummy_input = tf.random.normal((1, 20, 13))
model(dummy_input)

# Simulate mic input
def process_new_audio_chunk(audio_chunk):
    mfcc_vec = extract_mfcc(audio_chunk)
    sequence = update_buffer(mfcc_vec)
    if sequence is not None:
        input_tensor = tf.expand_dims(sequence, axis=0)  # [1, 20, 13]
        prediction = model(input_tensor).numpy()[0, 0]
        print("Snoring Probability:", prediction)


⸻

🛠️ 5. Optional: Training Later

If you later want to train this model:
	•	Prepare sequences of shape [num_samples, 20, 13]
	•	Labels: 1 = snoring, 0 = not
	•	Loss: binary_crossentropy
	•	Optimizer: Adam

⸻

Would you like help building a live audio stream reader from the mic or simulate fake chunks?
Also, do you want to train this model from scratch or load a pre-trained one if you get labeled data?