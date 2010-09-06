using System;

namespace RfSuit
{
	interface IConnection
	{
		bool[] DetectDevices();
		bool Start(string portName);
		void Stop();
		event SweepCompletedDelegate SweepCompleted;
	}
}
