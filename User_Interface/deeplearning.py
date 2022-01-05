import time
import numpy as np
import cv2
import tensorflow as tf
from scipy.special import softmax


#face detection model
#I made the model with my code but I cannot share with you because of file size. 
face_detection_model = cv2.dnn.readNetFromCaffe('./models/deploy.prototxt.txt',
                                                './models/res10_300x300_ssd_iter_140000_fp16.caffemodel')
# face mask recognition model
model = tf.keras.models.load_model('face_cnn_model/')

labels = ['Mask', 'No Mask', 'Covered Mouth Chin', 'Covered Nose Mouth']

def getColor(label):
    # I create if stataments for text color which in real-time video and recognize the status of people who looks camera
    if label == 'Mask':
        file = open("result.txt","w+")
        file.write('1')
        file.close()
        color = (0,255,0)
    elif label == 'No Mask':
        file = open("result.txt","w+")
        file.write('2')
        file.close()
        color = (0,0,255)
    elif label == 'Covered Mouth Chin':
        file = open("result.txt","w+")
        file.write('3')
        file.close()
        color = (0,255,255)
    else:
        file = open("result.txt","w+")
        file.write('4')
        file.close()
        color = (255,255,0)
    return color

def face_mask_prediction(img):
    #face detection
    image = img.copy()
    h, w = image.shape[:2]
    # I use 'blob' name for variable because, A blob is a group of pixels that the computer identifies as objects.
    blob = cv2.dnn.blobFromImage(image,1,(300,300),(104,117,123),swapRB=True)
    # I make the program use instant image to process and create a result 
    
    face_detection_model.setInput(blob)
    # program detect people in the instant image which in real-time video
    detection_img = face_detection_model.forward()
    for i in range(0,detection_img.shape[2]):
        confidence = detection_img[0,0,i,2]
        #I use the statement of confidence is bigger than 0.5 because I want to make true prediction
        if confidence > 0.5:
            box = detection_img[0,0,i,3:7]*np.array([w,h,w,h])
            box = box.astype(int)
            side1 = (box[0], box[1])
            side2 = (box[2], box[3])
            #cv2.rectangle(image,side1,side2,(0,255,0),1)

            #data processing
            face = image[box[1]:box[3],box[0]:box[2]]
            face_blob = cv2.dnn.blobFromImage(face,1,(100,100),(104,117,123),swapRB=True)
            face_blob_squeeze = np.squeeze(face_blob).T
            #The processed image was sideways, I froze it 90 degrees
            face_blob_rotate = cv2.rotate(face_blob_squeeze,cv2.ROTATE_90_CLOCKWISE)
            #Since the picture is like in the mirror, so, I flipped it
            face_blob_flip = cv2.flip(face_blob_rotate,1)
            
            #clear negative numbers
            img_normal = np.maximum(face_blob_flip,0)/face_blob_flip.max()

            #deep learning cnn
            image_input = img_normal.reshape(1,100,100,3)
            result = model.predict(image_input)
            result = softmax(result)[0]
            # Softmax is a mathematical function that converts a vector of numbers into a vector of probabilities, 
            # where the probabilities of each value are proportional to the relative scale of each value in the vector.
            confidence_index = result.argmax()
            confidence_score = result[confidence_index]
            label = labels[confidence_index]
            # I set my text which will be written to reel time video
            label_text = '{} : {:,.0f} %'.format(label,confidence_score*100)

            #I get text color from getColor fucntion which defined on the top 
            color = getColor(label)
            cv2.rectangle(image,side1,side2,color,1)
            #I wrote my prediction to video
            cv2.putText(image,label_text,side1,cv2.FONT_HERSHEY_PLAIN,2,color,2)
    time.sleep(0.1)
    #Since we have few processed images, I used the sleep command to instantly reduce the display of incorrect results.
    #With more powerful computers, we can make models with reduced error coefficients, or I have to learn and use darknet
    return image

# #recognition code
# img = cv2.imread("./download.jpg")
# image = face_mask_prediction(img)
# cv2.imshow('predictions',image)
# cv2.waitKey()
# cv2.destroyAllWindows()
# #real time
# import time
# cap = cv2.VideoCapture(0)
# while True:
#     ret, frames = cap.read()
#     if ret == False:
#         break
#     image = face_mask_prediction(frames)
#     cv2.imshow('Face Mask Prediction',image)
#     if cv2.waitKey(1) == 27:
#         break
#     time.sleep(0.1)    
# cap.release()
# cv2.destroyAllWindows()