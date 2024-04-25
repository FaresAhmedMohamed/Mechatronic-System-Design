arduinolist=["cb","cl","cl","sl","sb","cb","sl","sb"]

currentposition=len(arduinolist)-1
final=[]
if arduinolist[currentposition] in ("sl","cl"):
    k = 0
    while k<len(arduinolist):
        if arduinolist[(currentposition+k+1)%len(arduinolist)] in ("sb","cb"):
            final.append((currentposition+k+1)%len(arduinolist))
            final.append("ccw")
            currentposition = (currentposition+k+1)%len(arduinolist)
            break
        elif arduinolist[abs((currentposition-(k+1))%len(arduinolist))] in ("sb","cb"):
            final.append(abs((currentposition-(k+1))%len(arduinolist)))
            final.append("cw")
            currentposition = abs((currentposition-(k+1))%len(arduinolist))
            break
        k+=1
j=0
passed=[]
while j<len(arduinolist)-1:
    i = currentposition
    objtype=arduinolist[currentposition][0]
    passed.append(i)
    turns = 1
    direction = "ccw"
    while i<currentposition+len(arduinolist):
        if (i+1)%len(arduinolist) in passed:
            i += 1
            turns += 1
            continue
        nextobject = arduinolist[((i+1) % len(arduinolist))]
        if ((nextobject == "sl" and objtype == "s") or (nextobject == "cl" and objtype == "c")) and j%2==0:
            if turns>(len(arduinolist)/2):
                direction="cw"
            currentposition = (i+1) % len(arduinolist)
            final.append((i+1) % len(arduinolist))
            final.append(direction)
            break
        elif (nextobject in ("sb","cb")) and j%2==1:
            if turns>(len(arduinolist)/2):
                direction="cw"
            currentposition = (i+1) % len(arduinolist)
            final.append((i+1) % len(arduinolist))
            final.append(direction)
            break
        i += 1
        turns += 1
    j+=1
i = 0
print(final)