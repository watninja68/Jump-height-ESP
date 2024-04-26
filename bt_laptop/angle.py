import math

def angles(horizontal_distance, vertical_distance):
    angle = math.degrees(math.atan(vertical_distance / horizontal_distance))
    return angle

def types(x,y):
    angle = angles(x,y)
    x = ""
    if angle >75 and angle<90:
        if angle !=90:
            x = "Jump straighter " + str(abs(angle-90)) + "degree to the your center"
        return "Vertical" + x , angle
    else:
        if angle !=45:
            if 45 - angle >0:
                x = ", Jump at and angle of 45 " + str(45 - angle) + " degrees higgher"
            else:
                x = ", Jump at and angle of 45 " + str(angle-45) + " degrees lower"

        return "Horizontal" +x , angle

x = 10  
y = 5     

jump,angle = types(x,y)

print(f"Jump angle is {angle} degrees,  {jump}.")
