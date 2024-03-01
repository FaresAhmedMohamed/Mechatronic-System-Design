from cvzone.SerialModule import SerialObject
import cv2
import numpy

arduino = SerialObject()
#cap = cv2.VideoCapture(0)
img = cv2.imread("../TestImg/cubel.png") #remove when using cam
def empty(a):
    pass

def getContours(img,imgContour):
    squares=0
    circles=0
    prevarea=0
    contours,hierarchy = cv2.findContours(img,cv2.RETR_LIST,cv2.CHAIN_APPROX_NONE)
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area==0:
            continue
        differ=abs(area-prevarea)/area
        if differ<0.5:
            continue
        prevarea = area
        #print(area)
        if area>1000:
            cv2.drawContours(imgContour, cnt, -1, (255, 0, 0), 3)
            peri = cv2.arcLength(cnt,True)
            #print(peri)
            approx = cv2.approxPolyDP(cnt,0.02*peri,True)
            #print(len(approx))
            objCor = len(approx)
            x, y, w, h = cv2.boundingRect(approx)

            if objCor == 4:
                aspRatio = w/float(h)
                if aspRatio >0.93 and aspRatio <1.07:
                    objectType= "Square"
                else:objectType="Rectangle"
                squares = squares + 1
            elif objCor>4:
                objectType= "Circle"
                circles=circles+1
            else:objectType="None"
            cv2.putText(imgContour, objectType,
                        (x + (w // 2) - 10, y + (h // 2) - 10), cv2.FONT_HERSHEY_COMPLEX, 0.7,
                        (0, 0, 0), 2)
    idobj = [squares, circles]
    return idobj

#cv2.namedWindow("Parameters")
#cv2.resizeWindow("Parameters",640,240)
#cv2.createTrackbar("Threshold1","Parameters",130,255,empty)
#cv2.createTrackbar("Threshold2","Parameters",20,255,empty)

while True:
    go = arduino.getData()
    if go[0]=='9':
        framesread=0
        while framesread<100:
            # success, img = cap.read() #add when using cam
            imgContour = img.copy()
            imgGray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            imgBlur = cv2.GaussianBlur(imgGray, (11, 11), 0)
            #threshold1 = cv2.getTrackbarPos("Threshold1", "Parameters")
            #threshold2 = cv2.getTrackbarPos("Threshold2", "Parameters")
            threshold1 = 130
            threshold2 = 20
            imgCanny = cv2.Canny(imgBlur, threshold1, threshold2)
            kernel = numpy.ones((5, 5), )
            imgDialation = cv2.dilate(imgCanny, kernel, iterations=1)
            idobj = getContours(imgCanny, imgContour)
            cv2.imshow("Image", imgContour)
            cv2.imshow("ImageC", imgCanny)
            cv2.resizeWindow("Image", 1000, 1000)
            cv2.waitKey(1)
            framesread = framesread+1
        cv2.destroyAllWindows()
        if idobj[0]==2 and idobj[1]==0:
            arduino.sendData([1])
            img = cv2.imread("../TestImg/circleb.png")
        elif idobj[0]==1 and idobj[1]==0:
            arduino.sendData([2])
            img = cv2.imread("../TestImg/circlel.png")
        elif idobj[0]==0 and idobj[1]==2:
            arduino.sendData([3])
        elif idobj[0]==0 and idobj[1]==1:
            arduino.sendData([4])
            img = cv2.imread("../TestImg/cubeb.png")

