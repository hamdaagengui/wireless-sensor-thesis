using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;

namespace RfSuit
{
	public delegate void SweepStartedDelegate(int channel);
	public delegate void SweepCompletedDelegate(int channel, SweepResults[] results);

	public class Connection
	{
		public event SweepStartedDelegate SweepStarted;
		public event SweepCompletedDelegate SweepCompleted;

		Thread workerThread;
		SerialPort port;

		public bool Start(string portname)
		{
			try
			{
				port = new SerialPort(portname, 115200);
			}
			catch
			{
				return false;
			}

			port.Open();
			if (port.IsOpen == false)
			{
				return false;
			}

			workerThread = new Thread(new ThreadStart(WorkerMethod));
			workerThread.IsBackground = true;
			workerThread.Start();

			return true;
		}

		void WorkerMethod()
		{
			bool run = true;

			while (run)
			{

			}

			port.Close();
		}
	}
}
