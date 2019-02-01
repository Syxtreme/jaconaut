#include "kinova_api.h"
#include "Kinova.API.CommLayerUbuntu.h"
#include "Kinova.API.UsbCommandLayerUbuntu.h"

const unsigned char Jaco2::CONTROL_MODE = 0x01;
const unsigned char Jaco2::HAND_ADDRESS[3] = {0x16, 0x17, 0x18};
const float Jaco2::MAX_TORQUE[6] = {40.0, 80.0, 40.0, 20.0, 20.0, 20.0};  // in Nm
const unsigned char Jaco2::JOINT_ADDRESS[6] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
const unsigned char Jaco2::TORQUE_DAMPING[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const short Jaco2::TORQUE_KP[6] = {1000, 1500, 1000, 1750, 1750, 1750};

Jaco2::Jaco2(int a_display_error_level) {

    CartesianPosition currentCommand;
    // get current date and time for error logging
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %I:%M:%S",timeinfo);
    string currentdatetime(buffer);
    datetime = currentdatetime;

    // check the current log file size, if greater than 5Mb, delete it
    file_limit = 5e6;
    log_save_location = ".jaco2_log.txt";

    ifstream file(log_save_location.c_str(), ios::binary | ios::ate);
    if (file.tellg() > file_limit)
    {
       remove(log_save_location.c_str());
       log_msg(1, "File size exceeded, log file deleted");
    }

    ctr = 0;
    //set common variables
    delay = 1250;
    packets_sent = 6;
    packets_read = 18; //3 responses (14, 15, 16) expected per motor
    current_motor = 6; // only 6 joints so if source address is not < 6 after
                      // reading should receive error due do array size
    display_error_level = a_display_error_level;
    types.push_back("DEBUG");
    types.push_back("INFO");
    types.push_back("WARNING");
    types.push_back("ERROR");

    // get the current date and time
    // TODO: get date and time with ctime for first line of log and name of
    // file

    memset(updated, 0, (size_t)sizeof(int)*6);
    memset(updated_hand, 0, (size_t)sizeof(int)*3);
    memset(pos_finger, 0.0, (size_t)sizeof(float)*3);

    write_count = 0;
    read_count = 0;
    unsigned short d = 0x00;

}

Jaco2::~Jaco2() { }

void Jaco2::Connect() {
	commandLayer_handle = dlopen("Kinova.API.USBCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL);

	//We load the functions from the library (Under Windows, use GetProcAddress)
	MyInitAPI = (int (*)()) dlsym(commandLayer_handle,"InitAPI");
	MyCloseAPI = (int (*)()) dlsym(commandLayer_handle,"CloseAPI");
	MyMoveHome = (int (*)()) dlsym(commandLayer_handle,"MoveHome");
	MyInitFingers = (int (*)()) dlsym(commandLayer_handle,"InitFingers");
	MyGetDevices = (int (*)(KinovaDevice devices[MAX_KINOVA_DEVICE], int &result)) dlsym(commandLayer_handle,"GetDevices");
	MySetActiveDevice = (int (*)(KinovaDevice devices)) dlsym(commandLayer_handle,"SetActiveDevice");
	MySendBasicTrajectory = (int (*)(TrajectoryPoint)) dlsym(commandLayer_handle,"SendBasicTrajectory");
	MyGetQuickStatus = (int (*)(QuickStatus &)) dlsym(commandLayer_handle,"GetQuickStatus");
	MyGetCartesianCommand = (int (*)(CartesianPosition &)) dlsym(commandLayer_handle,"GetCartesianCommand");

	if((MyInitAPI == NULL) || (MyCloseAPI == NULL) ||
	   (MyGetQuickStatus == NULL) || (MySendBasicTrajectory == NULL) ||
	   (MySendBasicTrajectory == NULL) || (MyMoveHome == NULL) || (MyInitFingers == NULL) ||
	   (MyGetCartesianCommand == NULL))
	{
		cout << "* * *  E R R O R   D U R I N G   I N I T I A L I Z A T I O N  * * *" << endl;
	}
	else
	{
        log_msg(1, "Initializing Communication...");
        fprintf(stderr, "Error string before initializing connection: %s\n", dlerror());

        //Flag used during initialization.
        int result;
        result = (*MyInitAPI)();
        cout << "Initialization's result :" << result << endl;

        KinovaDevice list[MAX_KINOVA_DEVICE];

        int devicesCount = MyGetDevices(list, result);

        for(int i = 0; i < devicesCount; i++) {
            cout << "Found a robot on the USB bus (" << list[i].SerialNumber << ")" << endl;

            //Setting the current device as the active device.
            MySetActiveDevice(list[i]);

            cout << "Send the robot to HOME position" << endl;
            MyMoveHome();

            cout << "Initializing the fingers" << endl;
            MyInitFingers();
        }
    }
}

CartesianInfo Jaco2::GetPosition() {
    CartesianInfo cartesianPosition;
    MyGetCartesianCommand(currentCommand);
    cartesianPosition = currentCommand.Coordinates;
    // printf("X: %f; Y: %f; Z: %f\nThetaX: %f; ThetaY: %f; ThetaZ: %f\n", cartesianPosition.X, cartesianPosition.Y, cartesianPosition.Z, cartesianPosition.ThetaX, cartesianPosition.ThetaY, cartesianPosition.ThetaZ);
    return cartesianPosition;
}

void Jaco2::Home() {
    MyMoveHome();
}

void Jaco2::Move() {
    TrajectoryPoint pointToSend;
    pointToSend.InitStruct();
    //We specify that this point will be an angular(joint by joint) position.
    pointToSend.Position.Type = CARTESIAN_POSITION;

    //We get the actual angular command of the robot.
    MyGetCartesianCommand(currentCommand);

    pointToSend.Position.CartesianPosition.X = currentCommand.Coordinates.X;
    pointToSend.Position.CartesianPosition.Y = currentCommand.Coordinates.Y - 0.1f;
    pointToSend.Position.CartesianPosition.Z = currentCommand.Coordinates.Z;
    pointToSend.Position.CartesianPosition.ThetaX = currentCommand.Coordinates.ThetaX;
    pointToSend.Position.CartesianPosition.ThetaY = currentCommand.Coordinates.ThetaY;
    pointToSend.Position.CartesianPosition.ThetaZ = currentCommand.Coordinates.ThetaZ;

    cout << "*********************************" << endl;
    cout << "Sending the first point to the robot." << endl;
    MySendBasicTrajectory(pointToSend);

    pointToSend.Position.CartesianPosition.Z = currentCommand.Coordinates.Z + 0.1f;
    cout << "Sending the second point to the robot." << endl;
    MySendBasicTrajectory(pointToSend);

    cout << "*********************************" << endl << endl << endl;
}

void Jaco2::MoveToPos(CartesianInfo position) {
    TrajectoryPoint pointToSend;
    pointToSend.InitStruct();
    //We specify that this point will be an angular(joint by joint) position.
    pointToSend.Position.Type = CARTESIAN_POSITION;

    pointToSend.Position.CartesianPosition.X = position.X;
    pointToSend.Position.CartesianPosition.Y = position.Y;
    pointToSend.Position.CartesianPosition.Z = position.Z;
    pointToSend.Position.CartesianPosition.ThetaX = position.ThetaX;
    pointToSend.Position.CartesianPosition.ThetaY = position.ThetaY;
    pointToSend.Position.CartesianPosition.ThetaZ = position.ThetaZ;
    MySendBasicTrajectory(pointToSend);
    cout << "*********************************" << endl << endl << endl;
}

void Jaco2::Disconnect() {
    int result;
    cout << endl << "C L O S I N G   A P I" << endl;
    result = (*MyCloseAPI)();
	dlclose(commandLayer_handle);
    log_msg(2, "Connection closed");

    ofstream myfile;
    myfile.open (log_save_location.c_str(), fstream::app);
    myfile << "------------------------------------------------------\n";
    myfile.close();
}

void Jaco2::InitPositionMode() {
    log_msg(1, "Initializing position control mode...");
    // TODO
    log_msg(2, "Position control mode activated");
}

int Jaco2::log_msg(int type, string msg)
{
    // check if message level based off enum, is higher than warning level, if
    // so print out message
    if (type >= display_error_level){
        cout << types[type-1].c_str() << ": " << msg << endl;
    }
    // append message log to save later
    ofstream myfile;
    myfile.open (log_save_location.c_str(), fstream::app);
    myfile << datetime << " " << types[type-1].c_str() << ": " << msg << "\n";
    myfile.close();
    return 0;
}

// int main() {
//     Jaco2 *jaco;
//     jaco = new Jaco2(0);
//     jaco->Connect();

//     return 0;
// }