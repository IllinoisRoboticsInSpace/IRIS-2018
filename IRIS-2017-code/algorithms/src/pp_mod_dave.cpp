#define DAVE    1
#define CASSIE  0
#define ACTTEST 0

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "checkboard_navigation_module.h"
#include "data_structure.hpp"
#include "MapTransformer.hpp"
#include "RRT.hpp"
#include "communication.hpp"

//#include <ros/ros.h>
//#include "std_msgs/String.h"
#include <sstream>

using namespace std;
string positionsString;
volatile fsm_repre_t cur_state = START;
volatile fsm_repre_t other_state = START;
volatile bool positionStringIsUsed = true;
extern volatile bool pathplan_map_used;

//#define pow2(x) (x)*(x)

const double LINEAR_CONST = 1000/2.;
const double ANGULAR_CONST = 1000/0.05;
const double CURVE_JSON_STEP = 1;
const double POINT1_CURVE_LENGTH = 4;
const double POINT2_CURVE_LENGTH = 8;
const double F_FORWARD_CONTROL = 500;
const double K_TURN_CONTROL = 250;

//Global variables
volatile double goal_x=0.;
volatile double goal_y=0.;
volatile double goal_t=M_PI;
enum{FORWARD=1, BACKWARDS =-1, ANY_DIR=3};
volatile int control_direction=ANY_DIR;
extern MATRIX pathplan_map;

enum{RETRACT = 0, STAY = 1, DEPLOY = 2};
locate_actuator actuators;

locate_actuator get_desired_actuator()
{
    return actuators;
}

//minimum path radious for RRT
const double min_radius = 10;


/**
* Checks whether or not if the robot will collide if it is at x,y, and angle theta.
* @return false if the robot will collide, true otherwise.
*/
bool collision_checker_f(double x, double y, double theta){
    int robotCellHalfWidth = 75/5./2.;
    int robotCellHalfHeight = 150/5./2.;
    int count =0;
    int threshold=robotCellHalfWidth*robotCellHalfHeight*.1;
    for (int robotX=-robotCellHalfWidth; robotX<=robotCellHalfWidth; ++robotX){
        for (int robotY=-robotCellHalfWidth; robotY<=robotCellHalfWidth; ++robotY){
            Vec2f point(robotX,robotY);
            point = MapTransformer::rotate_point(point,theta);
            point = MapTransformer::translate_point(point,x,y);
            if (pathplan_map.validIndex((int) point.x, (int) point.y) && pathplan_map((int) point.x, (int) point.y)==map_occupied){
                count++;
                if (count>threshold)
                    return false;
            }
        }
    }
    return true;
}


//absolute value templated
template<typename T> T absd(T d){return d<0?-d:d;}
//efficient square templated
//template<typename T> T pow2(T d){return d*d;}
//find distance in S(2pi) set (like a circle: through either side)
template<typename T> T diff2pi(T d)
{
    d=fmod2pi(d);
    return min(d,2*M_PI-d);
}

locate_motor desired_motor_action;

void* path_planning(void* unused)
{
    int count_loops=0;
    long int wtd_time_path_plan=millis();
    chesspos poss = {0,0,0,0};
    while(poss.millis==0) //wait for first location
        poss = get_chessboard_navigation_pos();

    std::cout<<"\033[0;32m"<< "PATHPLAN: got first position!"<<"\033[0m\n";

    path old_path;
    float old_direction=1;

    while(1)
    {
        float direction=1;
        double forward_cntl;
        double turning_cntl;
        //Get robot position
        chesspos pos = get_chessboard_navigation_pos();

        if(!pathplan_map_used) {

            path p_forward;
            if(control_direction!=BACKWARDS)
            {
                //Tells where the robot starts and ends at
                pose2d start(pos.x/5., pos.y/5., pos.t-M_PI/2.);
                pose2d end(goal_x/5., goal_y/5., goal_t-M_PI/2.); //change this later

                //creates a random path generator, runs 500 iterations per round, avoiding obstacles
                //check RRT.hpp to see how this function works
                p_forward = RRT(path_planner_functions<collision_checker_f_prototype>( min_radius, collision_checker_f), start, end, 500, false);
            }
            path p_backwards;
            if(control_direction!=FORWARD)
            {
                //Tells where the robot starts and ends at
                pose2d start(pos.x/5., pos.y/5., pos.t+M_PI/2.);
                pose2d end(goal_x/5., goal_y/5., goal_t+M_PI/2.); //change this later

                //creates a random path generator, runs 500 iterations per round, avoiding obstacles
                //check RRT.hpp to see how this function works
                p_backwards = RRT(path_planner_functions<collision_checker_f_prototype>( min_radius, collision_checker_f), start, end, 500, false);
            }

            path *p;

            switch(control_direction)
            {
                case(FORWARD):
                    p=&p_forward;
                    direction=1;
                    break;
                case(BACKWARDS):
                    p=&p_backwards;
                    direction=-1;
                    break;
                default:
                    if((p_forward.get_length()<p_backwards.get_length())&&p_forward.get_length()>0.)
                    {
                        p=&p_forward;
                        direction=1;
                    }
                    else
                    {
                        p=&p_backwards;
                        direction=-1;
                    }
            }
            if(p->get_length()==0.)
            {
                p=&old_path;
                direction=old_direction;
                std::cout<<"\033[0;42m"<< "PATHPLAN: **************** error in the path *******************"<<"\033[0m\n";
                std::cout<<"\033[0;32m"<< "PATHPLAN: Using old path"<<"\033[0m\n";
            }else
            {
                old_path=*p;
                old_direction=direction;
                std::cout<<"\033[0;32m"<< "PATHPLAN: path exists!"<<"\033[0m\n";
            }

            pathplan_map_used = true;





            pose2d point1,point2;
            turning_cntl=0;
            forward_cntl=0;
            switch(0)
            {
                default:
                if(millis()-pos.millis>4000)
                {
                    std::cout<<"\033[0;32m"<< "PATHPLAN: ********* position is too old ******** "<<": timestamp_location="<<pos.millis<<" timestamp_current="<<millis()<<"\033[0m\n";
                    break;
                }
                if( ! p->get_position(POINT1_CURVE_LENGTH, point1) )
                {
                    std::cout<<"\033[0;32m"<< "PATHPLAN: ********* path is too short to move!! ******** : point1"<<"\033[0m\n";
                    break;
                }
                if( ! p->get_position(POINT2_CURVE_LENGTH, point2) )
                {
                    std::cout<<"\033[0;32m"<< "PATHPLAN: ********* path is too short to move!! ******** : point2"<<"\033[0m\n";
                    break;
                }

                std::cout<<"\033[0;32m"<< "PATHPLAN: sending action to motors"<<"\033[0m\n";

                double theta=-fmod(point2.t-point1.t+M_PI,2*M_PI)-M_PI;

                turning_cntl=direction*K_TURN_CONTROL*theta;
                forward_cntl=direction*F_FORWARD_CONTROL;

                wtd_time_path_plan=millis();
            }


            //normalize and get right and left values
            double normalizer=(absd(turning_cntl)+absd(forward_cntl));
            //if(normalizer>1000.)
            {
                turning_cntl/=normalizer/1000.;
                forward_cntl/=normalizer/1000.;
            }
            if(control_direction!=0){
            desired_motor_action.motor_right=forward_cntl+turning_cntl;
            desired_motor_action.motor_left=forward_cntl-turning_cntl;
            }
            else{
                desired_motor_action.motor_right=0;
                desired_motor_action.motor_left=0;
            }

            std::cout<<"\033[0;32m"<< "PATHPLAN: current "<<pos.x<<" "<<pos.y<<" "<<pos.t<<" target "<< goal_x << " " << goal_y << " action l " << desired_motor_action.motor_left << " r " << desired_motor_action.motor_right <<"\033[0m\n";



            if (positionStringIsUsed) {
                //This string stores the Json data for the path
                positionsString = "{\"data\":[";

                //The length along the curve
                double curveLength = 0.0;

                //The distance in centimeters by which curveLength advances each time the following loop iterates
                double step = 1.0;

                //while next position is valid
                pose2d currentPoint;
                while (p->get_position(curveLength, currentPoint)) {
                    // start line with comma only if not first item
                    positionsString += (curveLength==0.0?"[":",[");
                    // x
                    positionsString += std::to_string(currentPoint.p.x);
                    positionsString += ",";
                    // y
                    positionsString += std::to_string(currentPoint.p.y);
                    positionsString += ",";
                    // theta
                    positionsString += std::to_string(currentPoint.t);
                    positionsString += "]";
                    // advance in curve length
                    curveLength+=CURVE_JSON_STEP;
                }

                //finish string
                positionsString += "],\"position\":["+std::to_string((int)(pos.x/5.))+".0,"+std::to_string((int)(pos.y/5.))+".0,"+std::to_string(pos.t)+
                        "],\"target\":["+std::to_string((int)(goal_x/5.))+".0,"+std::to_string((int)(goal_y/5.))+".0,"+std::to_string(goal_t)+"],\"motor\":[" +
                        std::to_string(desired_motor_action.motor_left) + "," + std::to_string(desired_motor_action.motor_right) + "]}";

                // this is the semaphore
                //when false debug_ip_server is controlling positionsString
                positionStringIsUsed = false;
            }

        }
        else
        {
            if(millis()-wtd_time_path_plan>2500)
            {
            desired_motor_action.motor_right=0;
            desired_motor_action.motor_left=0;
            }
        }

        count_loops++;
    }
}

locate_motor get_desired_motor()
{
	return desired_motor_action;
}

//Sets a goal to move to
void set_goal(double x, double y, int dir, const char * comment="")
{
    std::cout<<"\033[0;35m"<< "PATHPLAN: set_goal "<<x<<" "<<y<<" "<<dir<<" "<< comment <<"\033[0m\n";
    //Set the goal position to the x and y values
    goal_x = x;
    goal_y = y;
    control_direction = dir;
}

//Waits until robot reaches destination within specified tolerance
void wait_for_dist(double epsilon, const char * comment="", int maxtime=9999)
{
    std::cout<<"\033[0;35m"<< "PATHPLAN: wait_for_dist "<< epsilon << " " << comment <<"\033[0m\n";
    //Wait in here until the robot position is at or within the allowed tolerance
    chesspos pos = get_chessboard_navigation_pos();
    double dist = sqrt(pow2(goal_x - pos.x) + pow2(goal_y - pos.y));

    int count=maxtime*2;
    while(dist > epsilon)
    {
        //Recompute distance from goal
        pos = get_chessboard_navigation_pos();
        dist = sqrt(pow2(goal_x - pos.x) + pow2(goal_y - pos.y));

        //Wait for 1s
        sleep(500);
        count--;
        if(count<=0)
        {
            printf("\033[0;35mTIMEOUT IN FSM *******************************************\033[0m\n");
            break;
        }

    }

    control_direction = 0;
}

bool kinect_is_not_collecting=true;

void sleepf(float f)
{
    usleep(1000000*f);
}

void fsm_cassie_standby(void) {
    actuators.webcam = DEPLOY;
    actuators.height = DEPLOY;
    actuators.front = RETRACT;
    actuators.bin_or_still = RETRACT;
    actuators.collection = RETRACT;
    sleepf(8);
}

void fsm_dave_standby(void) {
    actuators.height = STAY;
    actuators.front = STAY;
    actuators.collection = STAY;
    actuators.webcam = DEPLOY;
    actuators.bin_or_still = RETRACT;
    sleepf(8);
}

/**
 * Generic wait for dumping function that (should be) called by Dave and Cassie
 * @brief
 */
void fsm_wait(void) {
    while (other_state != WAITING) {
        sleepf(1);
    }
}

/**
 * Dave performs these actions to get gravels
 * @brief
 */
void fsm_dave_ct(void) {
    actuators.bin_or_still = RETRACT;
    sleepf(8);
}

void fsm_dave_hold(void) {
    actuators.bin_or_still = RETRACT;
    sleepf(5);
}

void fsm_cassie_hold(void) {
    actuators.front = RETRACT;
    actuators.collection = RETRACT;
    actuators.height = RETRACT;
    sleepf(7);
}

/**
 * Dave performs these actions after Dave to transmit gravels
 * @brief
 */
void fsm_cassie_ct(void) {
    actuators.height = DEPLOY;
    actuators.bin_or_still = DEPLOY;
    sleepf(4);
    actuators.front = DEPLOY;
    sleepf(4);
    actuators.collection = DEPLOY;
    sleepf(4);
}

void fsm_dump(void) {
    actuators.bin_or_still = DEPLOY;
    sleepf(20);
    actuators.bin_or_still = RETRACT;
}

/**
 * Action - Mine \in FSM. Let Cassie mine stuff
 * @brief
 */
void fsm_mine(void) {
    // go forth and back before mining. Last year. But why?
    actuators.bin_or_still = DEPLOY;
    actuators.collection = DEPLOY;
    sleepf(11);
    actuators.height = RETRACT;
    actuators.front = DEPLOY;
    sleepf(8);
    actuators.collection = RETRACT;
    sleepf(5);
    actuators.height = DEPLOY;
    actuators.front = RETRACT;
    actuators.bin_or_still = RETRACT;
    sleepf(9);
}

//Main Finite State Machine
void *FSM(void * unused) {
    printf("STARTING FSM ***********************************\n");
    //Sequentially move through the different states: move_to_mine -> mine -> move_to_deposit -> deposit
    //Offset for varying the x-axis position of the goal states and iteration level
    int offset[3] = {0, 100, -100};
    int iter = 0;
    double x;
    double y;
    double epsilon = 250;
#ifdef ACTTEST
    while (1) {
        // tests to be added
    }
#endif
#ifdef CASSIE
    fsm_cassie_standby();

    while(1) {
        //Move to mine
        x = offset[iter];
        y = 500;
        set_goal(x, y, 1, "Move to mine");
        sleepf(10); // to move some dist
        cur_state = MOVING_TO_MINE;
        wait_for_dist(epsilon,  "Move to mine");

        sleepf(3);
        cur_state = MINING;
        fsm_mine();

        //Move to deposit
        //Align to center of arena
        x = 0; //center
        y = 297;
        set_goal(x, y, -1, "Move to meet");
        cur_state = MOVING_TO_MEET;
        wait_for_dist(epsilon,  "Move to meet");
        sleepf(4);
        cur_state = WAITING;
        fsm_wait();
        cur_state = TRANSMITTING;
        sleepf(20); // wait for DAVE to be ready

        //Deposit
        fsm_cassie_ct();
        sleepf(2);

        //Increment the iteration
        iter = (++iter) % 3;
        std::cout<<"\033[0;35m"<< "PATHPLAN: New iteration "<<iter  <<" of type "<< offset[iter] <<"\033[0m\n";
    }
#else
    fsm_dave_standby();
    sleepf(40); // for cassie to get in front of Dave

    while(1) {
        x = 0; // center of the map
        y = 297;
        set_goal(x, y, 1, "Move to meet");
        cur_state = MOVING_TO_MEET;
        wait_for_dist(epsilon, "Move to meet");
        // now we have got to the meeting place
        cur_state = WAITING;
        fsm_wait();
        cur_state = TRANSMITTING;
        fsm_dave_ct();
        while (other_state == TRANSMITTING) {
            sleepf(1);
        }
        // now we have the gravels
        fsm_dave_hold();
        x=0; // prep deposit
        y=0;
        set_goal(x, y, -1, "Moving to dump");
        cur_state = MOVING_TO_DUMP;
        wait_for_dist(epsilon, "Moving to dump");
        sleepf(2);
        //Move up to bin
        x = 0; //bin
        y = -50;
        set_goal(x, y, -1, "Moving to dump");
        wait_for_dist(epsilon, "Moving to dump");
        sleepf(2);
        fsm_dump();
        sleepf(10);
    }
#endif
}

bool kinect_is_ready()
{
    return true;
}
