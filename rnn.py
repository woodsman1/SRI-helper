import librosa
import numpy as np

def extract_mfcc(audio_chunk, sr=16000, n_mfcc=13):
    mfcc = librosa.feature.mfcc(
        y=audio_chunk, sr=sr, n_mfcc=n_mfcc, 
        n_fft=512, hop_length=512, center=False
    )
    return mfcc.T[0]  # Shape: (13,) — one vector per 512-sample chunk
    
    
    
    
from collections import deque

sequence_length = 20  # Tunable
mfcc_buffer = deque(maxlen=sequence_length)

def update_buffer(mfcc_vector):
    mfcc_buffer.append(mfcc_vector)
    if len(mfcc_buffer) == sequence_length:
        return np.array(mfcc_buffer)  # Shape: [20, 13]
    return None
    
    
    
import tensorflow as tf

class CausalSnoreRNN(tf.keras.Model):
    def __init__(self, input_size=13, hidden_size=32):
        super(CausalSnoreRNN, self).__init__()
        self.rnn = tf.keras.layers.GRU(hidden_size, return_sequences=False)
        self.fc = tf.keras.layers.Dense(1, activation='sigmoid')

    def call(self, x):
        return self.fc(self.rnn(x))  # Input: [batch, time, features]
        
        
        
        
model = CausalSnoreRNN()

# Run once to initialize weights
dummy_input = tf.random.normal((1, 20, 13))
model(dummy_input)

# Simulate processing of a new audio chunk
def process_new_audio_chunk(audio_chunk):
    mfcc_vec = extract_mfcc(audio_chunk)
    sequence = update_buffer(mfcc_vec)
    if sequence is not None:
        input_tensor = tf.expand_dims(sequence, axis=0)  # Shape: [1, 20, 13]
        prediction = model(input_tensor).numpy()[0, 0]
        print("Snoring Probability:", prediction)
