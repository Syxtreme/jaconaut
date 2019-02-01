/*****************************************************************************************************/
/*                                                                                                   */
/* Reads joystick position / buttons from multiple specific devices using the                        */
/* GetJoystickValueSpecific() API																     */
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
	MyRefresDevicesList = (int (*)()) dlsym(commandLayer_handle,"Ethernet_RefresDevicesList");
	MyJoystickCommand = (int(*)(JoystickCommand joystickCommand)) dlsym(commandLayer_handle, "Ethernet_SendJoystickCommand");
	MyJoystickValueSpec = (int(*)(Controller controller, JoystickCommand &joystickCommand)) dlsym(commandLayer_handle, "Ethernet_GetJoystickValueSpecific");
	MySetControllerMuting = (int(*)(Controller controller, bool state)) dlsym(commandLayer_handle, "Ethernet_SetControllerMuting");
	MyGetControllerMuting = (int(*)(Controller controller, bool &state)) dlsym(commandLayer_handle, "Ethernet_GetControllerMuting");

	//If the was loaded correctly
	if((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyJoystickCommand == NULL) || (MyJoystickValueSpec == NULL) || (MySetControllerMuting == NULL) || (MyGetControllerMuting == NULL))
	{
		printf("Error during initialization.\n");
	}
	else
	{
		printf("Initialization completed.\n");

		(*MyInitAPI)();
		(*MyRefresDevicesList)();
		printf("API init completed with result %d\n", result);fflush(stdout);

		JoystickCommand joystickCommandUI;
		JoystickCommand joystickCommand3Axis;
		int muteloop = 0;
		bool muted = false;

		while (1) {
			(*MyJoystickValueSpec)(THREE_AXIS_JOYSTICK, joystickCommand3Axis);
			(*MyJoystickValueSpec)(UNIVERSAL_INTERFACE, joystickCommandUI);
			printf("\n---- Joystick position UI / 3Axis----\n");
			printf("Incline L/R: %f\t%f\n", joystickCommandUI.InclineLeftRight, joystickCommand3Axis.InclineLeftRight);
			printf("Incline F/B: %f\t%f\n", joystickCommandUI.InclineForwardBackward, joystickCommand3Axis.InclineForwardBackward);
			printf("   Move L/R: %f\t%f\n", joystickCommandUI.MoveLeftRight, joystickCommand3Axis.MoveLeftRight);
			printf("   Move F/B: %f\t%f\n", joystickCommandUI.MoveForwardBackward, joystickCommand3Axis.MoveForwardBackward);
			printf("     Rotate: %f\t%f\n", joystickCommandUI.Rotate, joystickCommand3Axis.Rotate);
			printf("  Push-Pull: %f\t%f\n", joystickCommandUI.PushPull, joystickCommand3Axis.PushPull);
			printf("    Buttons UI: ");
			for (int i = 0; i < JOYSTICK_BUTTON_COUNT; i++) {
				printf(joystickCommandUI.ButtonValue[i] ? "1 " : "0 ");
			}
			printf("\n");
			printf("Buttons 3-Axis: ");
			for (int i = 0; i < JOYSTICK_BUTTON_COUNT; i++) {
				printf(joystickCommand3Axis.ButtonValue[i] ? "1 " : "0 ");
			}
			printf("\n");

			if (muteloop++ % 20 == 0)
			{
				muted = !muted;
				printf("Setting 3axis muting to %s\n", muted ? "MUTED" : "UNMUTED");
				(*MySetControllerMuting)(THREE_AXIS_JOYSTICK, muted);

				printf("Setting tower box muting to %s\n", !muted ? "MUTED" : "UNMUTED");
				(*MySetControllerMuting)(UNIVERSAL_INTERFACE, !muted);
			}

			bool joyMute = false;
			bool uiMute = false;
			(*MyGetControllerMuting)(THREE_AXIS_JOYSTICK, joyMute);
			(*MyGetControllerMuting)(UNIVERSAL_INTERFACE, uiMute);

			printf("Muting status from unit: joy: %s\t ui: %s\n", joyMute ? "Muted" : "Unmuted", uiMute ? "Muted" : "Unmuted");

			usleep(500000);
		}

		result = (*MyCloseAPI)();
	}

	dlclose(commandLayer_handle);

	return 0;
}
