import config
from random import randint

class Rover:
    def __init__(self, canvas=None, messages=None):
        self.canvas = canvas
        self.messages = messages
        self.rX = 20
        self.rY = 570
        self.points = self.calcPoints()
        self.drawShapes()

    def drawShapes(self):
        self.shape = self.canvas.create_polygon(
            self.points, outline='white', fill='#1f1', width=1, tags='rover')

    def calcPoints(self):
        return [
            self.rX-5, self.rY-5,
            self.rX-5, self.rY+5,
            self.rX+5, self.rY+5,
            self.rX+5, self.rY-5
        ]

    def movement(self):
        xm = randint(-10, 30)
        ym = randint(-30, 10)
        if(self.rX + xm < 590 and self.rX + xm > 10 and self.rY + ym < 590 and self.rY + ym > 10):
            self.rX += xm
            self.rY += ym
            self.canvas.move('rover', xm, ym)
            self.messages.addMessage("Rover has moved to position: \n(" + str(self.rX) + "," + str(self.rY) + ")\n")
            # messages.see(END)
            self.historyUpdate()
        if(len(config.history) < 50):
            self.canvas.after(500, self.movement)

    def historyUpdate(self):
        lastX = config.history[-1][0]
        lastY = config.history[-1][1]
        self.canvas.create_oval(lastX - 2, lastY - 2,
                                lastX + 2, lastY + 2, fill="#1f1")
        self.canvas.create_line(lastX, lastY, self.rX, self.rY, fill="#1f1")
        config.history.append([self.rX, self.rY])