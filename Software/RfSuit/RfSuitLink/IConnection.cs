using System;

namespace RfSuit
{
	public interface IConnection
	{
		bool[] DetectDevices();
		bool Start(string portName);
		void Stop();
		event SweepCompletedDelegate SweepCompleted;
	}
}
