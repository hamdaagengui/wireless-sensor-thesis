using System;
using dk.iha;

namespace RfSuit
{
	public delegate void SweepCompletedDelegate(LinkQualityIndicator[] linkQualityIndicators);

	public interface IConnection
	{
		event SweepCompletedDelegate SweepCompleted;

		bool Start(string portName);
		void Stop();

		bool[] DetectDevices();

		void SetTxPower(byte power);
		void SetChannel(byte channel);
	}
}
