#ifndef JACO2_RS485
#define JACO2_RS485

#include <ctime>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include "Kinova.API.CommLayerUbuntu.h"
#include "Kinova.API.UsbCommandLayerUbuntu.h"
#include "KinovaTypes.h"

using namespace std;

#define SOURCE_ADDRESS 0x00
#define SEND_ACTUAL_POSITION 0x002
#define POSITION_COMMAND 0x0010
#define POSITION_AND_CURRENT 0x0011
#define RS485_MSG_SEND_POSITION_AND_TORQUE_COMMAND 0x0014
#define REPORT_ERROR 0x0030
#define CLEAR_ERROR_FLAG 0x0033
#define GET_TORQUE_CONFIG_SAFETY 0x003F
#define ACK_MESSAGE 0x003F
#define GET_TORQUE_VALIDATION_REQUEST 0x0201
#define SEND_TORQUE_VALIDATION 0x0202
#define SWITCH_CONTROL_MODE_REQUEST 0x0203
#define SWITCH_CONTROL_MODE_REPLY 0x0204
#define SEND_TORQUE_CONFIG_SAFETY 0x0208
#define SEND_TORQUE_CONFIG_FILTERS 0x209
#define SEND_TORQUE_CONFIG_FEEDFORWARD_ADVANCED 0x213
#define SEND_TORQUE_CONFIG_CONTROL_PARAM_1 0x214
#define SEND_TORQUE_CONFIG_CONTROL_PARAM_2 0x215
#define POSITION_LIMIT 0x0021

typedef int (*fptr)(void);

class Jaco2 {

    public:
        static const unsigned char CONTROL_MODE;
        static const unsigned char HAND_ADDRESS[3];
        static const float MAX_TORQUE[6];
        static const unsigned char JOINT_ADDRESS[6];
        static const unsigned char TORQUE_DAMPING[6];
        static const short TORQUE_KP[6];

        CartesianPosition currentCommand;
        // misc variables
        int current_motor;
        int delay;
        vector<string> error_message;
        int updated[6]; // for switching to torque mode
        int updated2[6]; // for switching to position mode
        int updated_hand[3];
        string datetime;
        int display_error_level;
        vector<string> types;
        int file_limit;
        string log_save_location;

        // read variables
        float pos[6]; // from Halls sensor
        float pos_finger[3];
        float pos_rad[6];
        float torque_load[6];
        float vel[6];

        // torque variables

        // variables used during the communication process.
        int packets_read;
        int packets_sent;
        int read_count;
        int write_count;
        float target_angle[6];
        int ctr;  // counter for printing occasionally

        // function pointers to Kinova API
        int (*MyInitAPI)();
        int (*MyCloseAPI)();
        int (*MyMoveHome)();
        int (*MyInitFingers)();
        int (*MyGetDevices)(KinovaDevice[], int&);
        int (*MySetActiveDevice)(KinovaDevice);
        int (*MySendBasicTrajectory)(TrajectoryPoint);
        int (*MyGetQuickStatus)(QuickStatus&);
        int (*MyGetCartesianCommand)(CartesianPosition&);


        // main functions
        void Connect();
        void Move();
        CartesianInfo GetPosition();
        void MoveToPos(CartesianInfo position);
        void Disconnect();
        void Home();
        void InitPositionMode();
        int log_msg(int type, string msg);

        Jaco2(int a_display_error_level); //constructor
        ~Jaco2(); // deconstructor

        // A handle needed to open the API(library).
        void *commandLayer_handle;
};

#endif
