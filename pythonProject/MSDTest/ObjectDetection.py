from cvzone.SerialModule import SerialObject
import cv2
import numpy

arduino = SerialObject()
#cap = cv2.VideoCapture(0)
img = cv2.imread("../TestImg/4objects.png") #remove when using cam


def getContours(img,imgContour):
    squares=0
    circles=0
    prevarea=0
    px=0
    py=0
    contours,hierarchy = cv2.findContours(img,cv2.RETR_LIST,cv2.CHAIN_APPROX_NONE)
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area==0 or area<1000:
            continue
        peri = cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, 0.02 * peri, True)
        objCor = len(approx)
        x, y, w, h = cv2.boundingRect(approx)
        differa=abs(area-prevarea)/area
        differx=abs(x-px)
        differy=abs(y-py)
        if differx<50 and differy<50 and differa<0.5:
            continue
        prevarea = area
        px=x
        py=y
        if x < 600 and y < 600:
            cv2.drawContours(imgContour, cnt, -1, (255, 0, 0), 3)
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

def most_frequent(List):
    unique, counts = numpy.unique(List, return_counts=True)
    index = numpy.argmax(counts)
    return unique[index]

#cv2.namedWindow("Parameters")
#cv2.resizeWindow("Parameters",640,240)
#cv2.createTrackbar("Threshold1","Parameters",130,255,empty)
#cv2.createTrackbar("Threshold2","Parameters",20,255,empty)

while True:
    go = arduino.getData()
    if go[0]=='9':
        counter=0
        arduinolist=[]
        while counter<4:
            squarevals=[]
            circlevals=[]
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
                kernel = numpy.ones((3, 3), )
                #imgDilation = cv2.dilate(imgCanny, kernel, iterations=1)
                #imgErosion = cv2.erode(imgCanny, kernel, iterations=1)
                idobj = getContours(imgCanny, imgContour)
                squarevals.append(idobj[0])
                circlevals.append(idobj[1])
                cv2.imshow("Image", imgContour)
                #cv2.imshow("ImageC", imgCanny)
                cv2.resizeWindow("Image", 1200, 1200)
                cv2.waitKey(1)
                framesread = framesread+1
            cv2.destroyAllWindows()
            sqval = most_frequent(squarevals)
            circval = most_frequent(circlevals)
            if sqval==2 and circval==0:
                arduinolist.append("sb")
            elif sqval==1 and circval==0:
                arduinolist.append("sl")
            elif sqval==0 and circval==2:
                arduinolist.append("cb")
            elif sqval==0 and circval==1:
                arduinolist.append("cl")
            img = cv2.rotate(img,cv2.ROTATE_90_CLOCKWISE)

            counter+=1
            arduino.sendData([counter])
            while True:
                go = arduino.getData()
                if go[0] == '8':
                    go[0]='0'
                    break
    print(arduinolist)
    currentposition = len(arduinolist) - 1
    final = []
    if arduinolist[currentposition] in ("sl", "cl"):
        k = 1
        while k <= len(arduinolist):
            if arduinolist[(currentposition + k) % len(arduinolist)] in ("sb", "cb"):
                final.append(k*45)
                final.append("ccw")
                currentposition = (currentposition + k) % len(arduinolist)
                break
            elif arduinolist[(currentposition - (k) + len(arduinolist)) % len(arduinolist)] in ("sb", "cb"):
                final.append(k*45)
                final.append("cw")
                currentposition = (currentposition - (k) + len(arduinolist)) % len(arduinolist)
                break
            k += 1
    j = 0
    passed = []
    while j < len(arduinolist) - 1:
        k = 1
        passed.append(currentposition)
        objtype = arduinolist[currentposition][0]
        if arduinolist[currentposition] in ("sb", "cb"):
            while k <= len(arduinolist):
                nextobject = arduinolist[(currentposition + k) % len(arduinolist)]
                if (currentposition + k) % len(arduinolist) in passed:
                    pass
                elif (nextobject == "sl" and objtype == "s") or (nextobject == "cl" and objtype == "c"):
                    final.append(k*45)
                    final.append("ccw")
                    final.append(objtype)
                    currentposition = (currentposition + k) % len(arduinolist)
                    break
                nextobject = arduinolist[(currentposition - (k) + len(arduinolist)) % len(arduinolist)]
                if (currentposition - (k) + len(arduinolist)) % len(arduinolist) in passed:
                    pass
                elif (nextobject == "sl" and objtype == "s") or (nextobject == "cl" and objtype == "c"):
                    final.append(k*45)
                    final.append("cw")
                    final.append(objtype)
                    currentposition = (currentposition - (k) + len(arduinolist)) % len(arduinolist)
                    break
                k += 1
        elif arduinolist[currentposition] in ("sl", "cl"):
            while k <= len(arduinolist):
                nextobject = arduinolist[(currentposition + k) % len(arduinolist)]
                if (currentposition + k) % len(arduinolist) in passed:
                    pass
                elif nextobject in ("sb", "cb"):
                    final.append(k*45)
                    final.append("ccw")
                    currentposition = (currentposition + k) % len(arduinolist)
                    break
                nextobject = arduinolist[(currentposition - (k) + len(arduinolist)) % len(arduinolist)]
                if (currentposition - (k) + len(arduinolist)) % len(arduinolist) in passed:
                    pass
                elif nextobject in ("sb", "cb"):
                    final.append(k*45)
                    final.append("cw")
                    currentposition = (currentposition - (k) + len(arduinolist) % len(arduinolist))
                    break
                k += 1
        j += 1
    i = 0
    print(final)