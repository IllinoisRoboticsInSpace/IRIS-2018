import numpy as np
import random
import math
from copy import deepcopy

_black_pixel = 1
_possible_radius = 20
_max_possible_height = 250

_distance_between_two_points = lambda x1, y1, x2, y2: math.sqrt((float(x1) - x2) ** 2 + (float(y1) - y2) ** 2)

class coord_system:

    def __init__(self, width, height):
        a = [0 for i in range(width)]
        self.coord = [deepcopy(a) for i in range(height)]
        self.width = width
        self.height = height

    def display(self):
        return self.coord

    def get_np_array(self):
        return np.array(self.coord, np.uint8)

    def _horizontal_line(self, width = 1):
        line = random.randrange(0, self.height)
        for i in range(self.width):
            self.coord[line][i] = int(i * 2 / self.width * _black_pixel)

    def scramble(self, times = 1):
        if times == 0: return None
        self._horizontal_line()
        #self._horizontal_line()
        self._circular_hill()
        self._circular_hill()
        self.scramble(times = times - 1)

    def _circular_hill(self, max_height = _max_possible_height):
        self._temp_circle_processed_point = []
        center = (random.choice(range(self.height)), random.choice(range(self.width)))
        radius = random.randrange(1, _possible_radius)
        self._circle_render(center, radius, max_height)

    def _circle_render(self, center, radius, max_height):
        for y in range(self.height):
            for x in range(self.width):
                distance = _distance_between_two_points(center[0], center[1], x, y)
                if distance > radius: continue
                #self.coord[y][x] = max_height * (radius - distance) / radius
                self.coord[x][y]=1

'''
    def _circle_render(self, center, radius, max_height, x1, y1):
        try:
            if (x1, y1) in self._temp_circle_processed_point: return None
            distance = _distance_between_two_points(center[0], center[1], x1, y1)
            if distance > radius: return None
            self.coord[x1][y1] = max_height * (radius - distance) / radius
            self._temp_circle_processed_point.append((x1, y1))
            self._circle_render(center, radius, max_height, x1 + 1, y1)
            self._circle_render(center, radius, max_height, x1 + 1, y1 + 1)
            self._circle_render(center, radius, max_height, x1, y1 + 1)
            self._circle_render(center, radius, max_height, x1 - 1, y1 + 1)
            self._circle_render(center, radius, max_height, x1 - 1, y1)
            self._circle_render(center, radius, max_height, x1 - 1, y1 - 1)
            self._circle_render(center, radius, max_height, x1, y1 - 1)
            self._circle_render(center, radius, max_height, x1 + 1, y1 - 1)
        except IndexError:
            return None
'''
