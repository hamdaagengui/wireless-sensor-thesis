using System;

namespace RfSuit
{
	public delegate void SweepCompletedDelegate(dk.iha.LinkQualityIndicator[] linkQualityIndicators);

	public interface IConnection
	{
		bool[] DetectDevices();
		bool Start(string portName);
		void Stop();
		event SweepCompletedDelegate SweepCompleted;
	}
}
