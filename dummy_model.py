import tensorflow as tf

# Input shape: (64, 188, 1)
inputs = tf.keras.Input(shape=(64, 188, 1))

# Convolution
x = tf.keras.layers.Conv2D(8, 3, activation='relu', padding='same')(inputs)

# Built-in global pooling layers
gmp = tf.keras.layers.GlobalMaxPooling2D()(x)
gap = tf.keras.layers.GlobalAveragePooling2D()(x)

# Manual reduce max and mean
max_manual = tf.reduce_max(x, axis=[1, 2])
mean_manual = tf.reduce_mean(x, axis=[1, 2])

# Matrix multiplication (dot product)
dot = tf.matmul(tf.expand_dims(max_manual, 1), tf.expand_dims(mean_manual, 2))
dot = tf.squeeze(dot, axis=[1, 2])  # shape: (batch,)

# Combine everything
combined = gmp + gap + max_manual + mean_manual

# Final dense layer to output 2 values
outputs = tf.keras.layers.Dense(2)(combined)

# Build model
model = tf.keras.Model(inputs=inputs, outputs=outputs)

# Summary
model.summary()

# Example run
dummy_input = tf.random.normal([4, 64, 188, 1])
output = model(dummy_input)
print(output.shape)  # Should be (4, 2)