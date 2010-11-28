///*
// * AutomaticMemoryManager.c
// *
// *  Created on: 25/11/2010
// *      Author: coma
// */
//
//#include "AutomaticMemoryManager.h"
//
//#define WaitFor(cond) if(!(cond)) return; else __state++;
//#define Exit() __state = 0; return;
//
//void Send()
//{
//}
//
//void Bla()
//{
//	static uint8_t counter;
//
//	static uint8_t __state = 0;
//
//	switch (__state)
//	{
//		case 0:
//			Send();
//			__state++;
//			break;
//
//		case 1:
//			WaitFor(counter == 0)
//			break;
//
//		case 2:
//			Exit()
//			;
//	}
//}
//
//// shared
//typedef const struct
//{
//	void (*Clear)();
//	void (*OpenWindow)(void* window);
//} window_manager_interface;
//
//// WM internal
//static const uint8_t version = 3;
//
//void Clear()
//{
//}
//
//void OpenWindow(void* window)
//{
//}
//
//static window_manager_interface WindowManager = { .Clear=Clear, .OpenWindow=OpenWindow };
//
//window_manager_interface* GetWindowManagerInterface(uint8_t minimumVersion)
//{
//	if (version < minimumVersion)
//	{
//		return NULL;
//	}
//	else
//	{
//		return &WindowManager;
//	}
//}
//
//// WM header
////extern void window_manager_interface* GetWindowManagerInterface();
//
//// user app
//void App()
//{
//	window_manager_interface* wmi = GetWindowManagerInterface(2);
//
//	wmi->Clear();
//}





#define foreach(v, f) while()


void B()
{


}
