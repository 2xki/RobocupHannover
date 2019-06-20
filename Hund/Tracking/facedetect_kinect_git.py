import numpy as np
import cv2
import freenect
import serial
from pygame import mixer

mixer.init()
mixer.music.load('Bellen.mp3')

# multiple cascades: https://github.com/Itseez/opencv/tree/master/data/haarcascades

#https://github.com/Itseez/opencv/blob/master/data/haarcascades/haarcascade_frontalface_default.xml
face_cascade = cv2.CascadeClassifier('data/haarcascade_frontalface_default.xml')
#https://github.com/Itseez/opencv/blob/master/data/haarcascades/haarcascade_eye.xml
eye_cascade = cv2.CascadeClassifier('data/haarcascade_eye.xml')

hund = serial.Serial('/dev/ttyUSB0', 9600)
print(hund.name)
#print(hund.readline())

lastOut = b'100100\n'
x = 0
y = 0
w = 0
l = 100
r = 100
while 1:
    img = freenect.sync_get_video()[0]
    depth = freenect.sync_get_depth()[0].astype(np.uint8)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    for (x,y,w,h) in faces:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = img[y:y+h, x:x+w]
        
        eyes = eye_cascade.detectMultiScale(roi_gray)
        for (ex,ey,ew,eh) in eyes:
            cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)


    #Showing images
    d3 = np.dstack((depth,depth,depth)).astype(np.uint8)
    da = np.hstack((d3,img))
    cv2.imshow('both',np.array(da[::2,::2,::-1]))
    #cv2.imshow('img',img)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break
    
    #extracting cords
    if(type(faces).__module__ == np.__name__):
        w = faces[0,2]          #150 - 50
        x = faces[0,0] + w / 2  #100 - 300 - 600
        y = faces[0,1] + faces[0,3] / 2
        
        if(x < 250):
            l = 150
            r = 200
            out = b'100150\n'
        elif(x > 350):
            l = 200
            r = 150
            out = b'150100\n'
        else:
            l = 150
            r = 150
            out = b'150150\n'
        if(w > 50 and w <= 60):
            l = 100
            r = 100
            out = b'100100\n'
            hund.write(b'2\n')   # Schwanzwedeln
        elif(w > 60):
            out = b'050050\n'
        
        print('x: ' + str(x) + '\tw: ' + str(w) + '\tl: ' + str(l) + '\tr: ' + str(r) + '\tout: ' + out)
        #out = str(l) + str(r)
        
        #print(faces[0])
        #print("Depth: " + str(depth))
    else:
        out = b'100100\n'
        hund.write(b'3\n')   # Schwanzwedeln aus
    
    
    
    if(out != lastOut):
        hund.write(out)
        print('sended: ' + out)
        '''print(hund.readline())
        print(hund.readline())
        print(hund.readline()) '''      
        lastOut = out
        mixer.music.play()

cv2.destroyAllWindows()
