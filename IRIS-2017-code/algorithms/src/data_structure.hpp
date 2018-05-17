#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_
// Main data structure
#include "Map.hpp"//Map<T>

enum {
    START          = 0,
    MOVING_TO_MINE = 1,
    MOVING_TO_MEET = 2,
    WAITING        = 3,
    TRANSMITTING   = 4,
    MOVING_TO_DUMP = 5,
    DUMPING        = 6,
    MINING         = 7
} fsm_repre_t;

struct navigation_and_mapping_data {
    volatile MATRIX* map;
    volatile MATRIX* local_map;
    volatile double true_pos_x;
    volatile double true_pos_y;
    volatile double true_theta;
    volatile double imu_theta;
    volatile double track_right;
    volatile double track_left;
};

struct chesspos{
    float x,y;
    float t;
    long int millis;
};

chesspos get_chessboard_navigation_pos();

extern navigation_and_mapping_data D;

long int millis();

#endif
