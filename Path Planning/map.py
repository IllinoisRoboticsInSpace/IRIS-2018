import numpy as np
import freenect
import math

dimX = 640
dimY = 480
half_dimX = dimX / 2
half_dimY = dimY / 2
degree_2_rad = math.pi / 180.0
fovX = 57 * degree_2_rad
fovY = 43 * degree_2_rad
half_fovX = fovX / 2.0
half_fovY = fovY / 2.0

def _raw_2_millimeter(raw_depth):
    mystery1 = -0.0030711016 #copied from IRIS-2017; it said these magic num were obtained from the internet
    mystery2 = 3.3309495161
    return (1.0 / (raw_depth * mystery1 + mystery2)) * 1000

def get_angle(x, y):
    x = ((half_dimX - x) / half_dimX ) * half_fovX
    y = (math.pi / 2.0) - (((half_dimY - y)/ half_dimY) * half_fovY)
    return (x, y)

class map:

    def __init__(self):
        self.map = np.zeros((100, 100)) #10cms
        #number of time each point is detected. Used for calculate mean of point to hopefully make it more accurate
        self.num_times_detected = np.zeros((100, 100))
        self.my_pos = self.update_my_location()

    def __getitem__(self, y, x):
        return self.map[y, x]

    def update_my_location(self):
        return (0, 0)

    def update_current_depth_frame(self):
        pass #it should update self._depth_frame with freenect

    def _depth_2_cartesian(self, x, y):
        distance = _raw_2_millimeter(_depth_frame[y, x])
        angle_x, angle_y = get_angle(x, y)
        cart_x = distance #approximation
        cart_y = distance * math.tan(angle_x)
        cart_z = distance * math.tan(-angle_y + math.pi / 2.0)
        return (cart_x, cart_y, cart_z)
