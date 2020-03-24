import sys
import config
import messages
from rover import Rover

try:
    import Tkinter as tk
except ImportError:
    import tkinter as tk

try:
    import ttk
    py3 = False
except ImportError:
    import tkinter.ttk as ttk
    py3 = True

def init(top, gui, *args, **kwargs):
    global w, top_level, root
    w = gui
    top_level = top
    root = top

def destroy_window():
    # Function which closes the window.
    global top_level
    top_level.destroy()
    top_level = None

if __name__ == '__main__':
    import main
    main.vp_start_gui()
    # r = Rover(top_level.Canvas1)
    # messages.insert(END, timeit() + "Waiting on start-up.\n")
    # s = input('Start?')
    # messages.insert(END, timeit() + "Rover has been started.\n")
    # r.movement()




