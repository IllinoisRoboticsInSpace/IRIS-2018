import dummy_height_data_generator as dummy_data
import numpy as np
import cv2
import time

_car_mark = 1
_map_size = 100

class animation:
    def __init__(self):
        self.pre_defined_map = dummy_data.coord_system(_map_size, _map_size)
        self.pre_defined_map.scramble(3)
        self.pre_defined_map = self.pre_defined_map.get_np_array()
        self._car_width = self.pre_defined_map.shape[0] / 30
        #cv2.imshow('go', pre_defined_map)
        #cv2.waitKey(0)
        #print self.pre_defined_map.shape

    def get_value(self, x, y):
        return self.pre_defined_map[y][x]

    def get_size(self):
        return self.pre_defined_map.shape

    def animate(self, car_x, car_y):
        _cur_map = np.copy(self.pre_defined_map)
        lower = max(0, car_y - self._car_width)
        upper = min(self.pre_defined_map.shape[0], car_y + self._car_width)
        left = max(0, car_x - self._car_width)
        right = min(self.pre_defined_map.shape[1], car_x + self._car_width)
        _cur_map[lower:upper, left:right] = _car_mark
        cv2.imwrite("debug.jpg", _cur_map)
        return _cur_map * 255

def main():
    a = animation()
    for i in range(_map_size):
        cv2.imshow("test", a.animate(i, i))
        #vout.write(img)
        #vout.write(img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        time.sleep(0.1)

if __name__ == '__main__':
    main()
