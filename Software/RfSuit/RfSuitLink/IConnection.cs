using System;

namespace RfSuit
{
	public delegate void SweepCompletedDelegate(SweepResults[] results);

	public interface IConnection
	{
		bool[] DetectDevices();
		bool Start(string portName);
		void Stop();
		event SweepCompletedDelegate SweepCompleted;
	}
}
