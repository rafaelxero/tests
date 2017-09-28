#!/usr/bin/env python

from IPython.display import Image
from tvtk.tools import ivtk
#from graph import Axis

viewer = ivtk.viewer()
viewer.size = (640, 480)

viewer.scene.camera.focal_point = (0.175, 0.321, 0.0)
viewer.scene.camera.position = (0.175, 0.321, 3.3)

display_number = 1

def display():
    global display_number
    path = 'img/%s.png' % display_number
    display_number += 1
    viewer.scene.save(path)
    return Image(path)

import numpy as np
import eigen3 as e
import spacevecalg as sva
