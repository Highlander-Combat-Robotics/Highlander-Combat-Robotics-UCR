import time
import random
import board
import terminalio
from adafruit_matrixportal.matrixportal import MatrixPortal

# --- Display setup ---
matrixportal = MatrixPortal(status_neopixel=board.NEOPIXEL, debug=True)

#figure out how the button works 

# Static Countdown Text
matrixportal.add_text(
    text_font=terminalio.FONT,
    text_scale = 2,
    text_position=(-3, (matrixportal.graphics.display.height // 2) - 1),
)

#TIMES UP!
matrixportal.add_text(
    text_font=terminalio.FONT,
    text_position=(6, (matrixportal.graphics.display.height // 2) - 1),
)

#3, 2, 1 COUNTDOWN 
matrixportal.add_text(
    text_font=terminalio.FONT,
    text_scale = 2,
    text_position=(27, (matrixportal.graphics.display.height // 2) - 1),
)

#GO!!!
matrixportal.add_text(
    text_font=terminalio.FONT,
    text_scale = 2,
    text_position=(17, (matrixportal.graphics.display.height // 2) - 1),
)

# #some useless crap 
# SCROLL_DELAY = 0.02
# UPDATE_DELAY = 600
# # create an array called colors with the colors of the rainbow, hex strings
# colors = ["#FF0000", "#FF7F00", "#FFFF00", "#00FF00", "#0000FF", "#4B0082", "#8F00FF"]

def setTime(mins, secs):
    minCount = mins
    secCount = secs
    outputDisplay = ""

    if secCount < 10:
        outputDisplay = ' ' + str(minCount) + ':0' + str(secCount)
    else:
        outputDisplay = ' ' + str(minCount) + ':' + str(secCount)
    
    if secCount == 0:
        minCount = minCount -1
        secCount = 60
    secCount = secCount - 1
    return outputDisplay

#countdown 
for k in range(3, -1, -1):
    matrixportal.set_text(k, 2)
    if(k == 0):
        matrixportal.set_text(" ", 2)
        matrixportal.set_text("GO!", 3)
    time.sleep(1)

matrixportal.set_text(" ", 3)

# timer decrement function
for j in range(3, -1, -1):
    if(j == 3):
        matrixportal.set_text(setTime(3, 0), 0)
        time.sleep(1)
    else:
        for i in range(59, -1, -1):
            if(j ==  0 and i < 10):
                matrixportal.set_text_color("#FF0000", 1) #FF0000
            matrixportal.set_text(setTime(j, i), 0) 
            time.sleep(1)

matrixportal.set_text(" ", 0)

while True:
    color_index = random.randrange(0, len(colors))
    matrixportal.set_text_color(colors[color_index], 1)
    matrixportal.set_text("TIMES UP!", 1)
    time.sleep(1)
