from config import messages
from tkinter import END
from datetime import datetime

class Messages:
    def __init__(self, listbox = None):
        self.listbox = listbox
        self.listbox.insert(END, self.timeit() + '\t Console initialized')
    def addMessage(self, message = None, type = None):
        self.listbox.insert(END, self.timeit() + '\t' + message)
    def timeit(self):
        return datetime.now().strftime("%H:%M:%S ")
            