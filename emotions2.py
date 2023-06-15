import cv2
import numpy as np
from keras.models import load_model
from utils.datasets import get_labels
from utils.inference import apply_offsets
from utils.preprocessor import preprocess_input

emotion_model_path = './models/emotion_model.hdf5'
emotion_labels = get_labels('fer2013')

face_cascade = cv2.CascadeClassifier('./models/haarcascade_frontalface_default.xml')
emotion_classifier = load_model(emotion_model_path)
emotion_target_size = emotion_classifier.input_shape[1:3]

cap = cv2.VideoCapture(0)
cap.set(3, 320)
cap.set(4, 240)

while True:  # True:
    ret, bgr_image = cap.read()
    gray_image = cv2.cvtColor(bgr_image, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray_image, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30),
                                          flags=cv2.CASCADE_SCALE_IMAGE)

    for face_coordinates in faces:
        x1, x2, y1, y2 = apply_offsets(face_coordinates, (20, 40))
        gray_face = gray_image[y1:y2, x1:x2]
        try:
            gray_face = cv2.resize(gray_face, (emotion_target_size))
            gray_face = preprocess_input(gray_face, True)
            gray_face = np.expand_dims(gray_face, 0)
            gray_face = np.expand_dims(gray_face, -1)

            face_list = emotion_classifier.predict(gray_face)[0]
            face_index = 0;
            face_text = emotion_labels[0]

            for i, v in enumerate(face_list):
                if face_list[i] > face_list[face_index]:
                    face_index = i
                    face_text = emotion_labels[i]

            cv2.rectangle(gray_image, (x1, y1), (x2, y2), (0, 0, 255), 3)
            cv2.putText(gray_image, face_text, (x1, y1), 0, 1, (0, 0, 255), 2)
        except:
            continue

    cv2.imshow('video', gray_image)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
