import tensorflow as tf
from tensorflow.keras.models import load_model

# Load the HDF5 model
hdf5_model_path = './models/_mini_XCEPTION.102-0.66.hdf5'
loaded_model = load_model(hdf5_model_path)

# Convert the loaded model to TFLite format
converter = tf.lite.TFLiteConverter.from_keras_model(loaded_model)
tflite_model = converter.convert()

# Save the TFLite model to a file
tflite_model_path = './models/emotion_model.tflite'
with open(tflite_model_path, 'wb') as f:
    f.write(tflite_model)

print("TFLite model saved successfully!")
