using System;
using dk.iha;

namespace RfSuit
{
	public delegate void SweepCompletedDelegate(LinkQualityIndicator[] linkQualityIndicators);

	public interface IConnection
	{
		bool[] DetectDevices();
		bool Start(string portName);
		void Stop();
		event SweepCompletedDelegate SweepCompleted;
	}
}
