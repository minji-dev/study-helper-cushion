import numpy as np
import cv2
import dlib

faceCascade = cv2.CascadeClassifier('haarcascade_frontalcatface.xml')
predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")

JAWLINE = list(range(0,17))
RIGHT_EYEBROW = list(range(17,22))
LEFT_EYEBROW = list(range(22,27))
NOSE = list(range(27,36))
RIGHT_EYE = list(range(36,42))
LEFT_EYE = list(range(42,48))
MOUTH_OUTLINE = list(range(48,61))
MOUTH_INNER = list(range(61,68))

def detect(gray, frame):
    faces = faceCascade.detectMultiScale(gray, scaleFactor=1.05, minNeighbors=5, minSize=(100,100), flags=cv2.CASCADE_SCALE_IMAGE)

    for (x,y,w,h) in faces:
        dlib_rect = dlib.rectangle(int(x), int(y), int(x+w), int(y+h))
        landmarks = np.matrix([[p.x, p.y] for p in predictor(frame, dlib_rect).parts()])
        landmarks_display = landmarks[0:68]
        
        for idx, point in enumerate(landmarks_display):
            pos = (point[0, 0], point[0, 1])
            cv2.circle(frame, pos, 2, color=(0,255,255), thickness=-1)
            
    return frame

video_capture = cv2.VideoCapture(0)

while True:
    _, frame = video_capture.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    canvas = detect(gray, frame)
    cv2.imshow("window", canvas)
    cv2.waitKey(0)
                
video_capture.release()
cv2.destroyAllWindows()
