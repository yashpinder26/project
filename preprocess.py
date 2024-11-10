import tensorflow as tf
from tensorflow.keras.preprocessing.image import ImageDataGenerator

# Set up paths
base_dir = '/home/yashpinder/project/dataset'  # Your dataset directory

# Create ImageDataGenerator for data augmentation and loading
datagen = ImageDataGenerator(rescale=1./255, validation_split=0.2)  # Normalize pixel values

# Load the training data
train_data = datagen.flow_from_directory(
    base_dir,
    target_size=(3280, 2464),  # Resize images to the input shape
    batch_size=32,
    class_mode='binary',  # Change to 'categorical' if you have more than two classes
    subset='training',
    shuffle=True
)

# Load the validation data
validation_data = datagen.flow_from_directory(
    base_dir,
    target_size=(3280, 2464),
    batch_size=32,
    class_mode='binary',
    subset='validation'
)
