/*****************************************************************************************************/
/*                                                                                                   */
/*     */
/*                                                                                                   */
/*****************************************************************************************************/

#include <iostream>
#include <dlfcn.h>
#include <vector>
#include "Kinova.API.EthCommLayerUbuntu.h"
#include "Kinova.API.EthCommandLayerUbuntu.h"
#include "KinovaTypes.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fstream>

using namespace std;

int main()
{
	int result;

	//Handle for the library's command layer.
	void * commandLayer_handle;

	//Function pointers to the functions we need
	int (*MyInitAPI)();
	int (*MyCloseAPI)();
	int (*MyStartControlAPI)();
	int (*MyStopControlAPI)();
	int (*MyRefresDevicesList)();
	int (*MyJoystickCommand)(JoystickCommand joystickCommand);
	int (*MyJoystickValueSpec)(Controller controller, JoystickCommand &joystickCommand);
	int (*MySetControllerMuting)(Controller controller, bool state);
	int	(*MyGetControllerMuting)(Controller controller, bool &state);

	//We load the library
	commandLayer_handle = dlopen("Kinova.API.EthCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL);

	//We load the functions from the library
	MyInitAPI = (int (*)()) dlsym(commandLayer_handle,"Ethernet_InitAPI");
	MyCloseAPI = (int (*)()) dlsym(commandLayer_handle,"Ethernet_CloseAPI");
	MyStartControlAPI = (int(*)()) dlsym(commandLayer_handle, "Ethernet_StartControlAPI");
	MyStopControlAPI = (int(*)()) dlsym(commandLayer_handle, "Ethernet_StopControlAPI");
	MyRefresDevicesList = (int (*)()) dlsym(commandLayer_handle,"Ethernet_RefresDevicesList");
	MyJoystickCommand = (int(*)(JoystickCommand joystickCommand)) dlsym(commandLayer_handle, "Ethernet_SendJoystickCommand");
	MyJoystickValueSpec = (int(*)(Controller controller, JoystickCommand &joystickCommand)) dlsym(commandLayer_handle, "Ethernet_GetJoystickValueSpecific");
	MySetControllerMuting = (int(*)(Controller controller, bool state)) dlsym(commandLayer_handle, "Ethernet_SetControllerMuting");
	MyGetControllerMuting = (int(*)(Controller controller, bool &state)) dlsym(commandLayer_handle, "Ethernet_GetControllerMuting");

	//If the was loaded correctly
	if((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyStartControlAPI == NULL) || (MyStopControlAPI == NULL) ||
	(MyJoystickCommand == NULL) || (MyJoystickValueSpec == NULL) || (MySetControllerMuting == NULL) || (MyGetControllerMuting == NULL))
	{
		printf("Error during initialization.\n");
	}
	else
	{
		printf("Initialization completed.\n");

		(*MyInitAPI)();
		(*MyRefresDevicesList)();
		JoystickCommand joystickCommand3Axis;
		JoystickCommand joystickCommandOut;
		PeripheralInfo peripheralInfo[MAX_INVENTORY];

		printf("Starting Control API\n");
		(MyStartControlAPI)();

		printf("Muting 3-axis joystick\n");
		(*MySetControllerMuting)(THREE_AXIS_JOYSTICK, true);

		bool drink = false;
		bool autoMove = false;
		int loop = 0;
		float rotate = -0.5;
		float fb = 0.3;
		float lr = 0.7;

		while (1) {
			memset(&joystickCommandOut, 0, sizeof(JoystickCommand));

			if (loop % 20 == 0) {
				printf("Toggling drinking mode. Loop: %d\n", loop);
				joystickCommandOut.ButtonValue[3] = 1;
			}

			if(autoMove)
			{
				if (loop % 20 == 0) {
					rotate *= -1;
					fb *= -1;
					lr *= -1;
					printf("Inverting joystick motion. FB: %f\tLR: %f\tRot: %f\n", fb, lr, rotate);
				}

				joystickCommandOut.Rotate = rotate;
				joystickCommandOut.InclineForwardBackward = fb;
				joystickCommandOut.InclineLeftRight = lr;
			}
			else
			{
				// Read from muted 3-axis and send reverse command to robot
				(*MyJoystickValueSpec)(THREE_AXIS_JOYSTICK, joystickCommand3Axis);
				joystickCommandOut.Rotate = joystickCommand3Axis.Rotate * -1;
				joystickCommandOut.InclineForwardBackward = joystickCommand3Axis.InclineForwardBackward * -1;
				joystickCommandOut.InclineLeftRight = joystickCommand3Axis.InclineLeftRight * -1;
			}
			(*MyJoystickCommand)(joystickCommandOut);

			if (loop % 20 == 0)
			{
				printf("[%05d] lr: %f\tfb: %f\trot: %f\n", loop,
					joystickCommandOut.InclineLeftRight,
					joystickCommandOut.InclineForwardBackward,
					joystickCommandOut.Rotate);
			}

			loop++;
			usleep(50000);
		}

		printf("Stopping Control API\n");
		(MyStopControlAPI)();


		(*MyCloseAPI)();
	}

	dlclose(commandLayer_handle);

	return 0;
}
