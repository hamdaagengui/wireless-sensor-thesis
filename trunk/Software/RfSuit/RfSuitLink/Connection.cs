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
		class SweepCompletedMessage
		{
			public int channel;
			public SweepResults[] results;
		}

		public event SweepStartedDelegate SweepStarted;
		public event SweepCompletedDelegate SweepCompleted;

		Thread workerThread;
		SerialPort port;

		public bool Start(string portName)
		{
			try
			{
				port = new SerialPort(portName, 115200);
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
				SweepResults[] sr = new SweepResults[16];
				
				ThreadPool.QueueUserWorkItem(new WaitCallback(ReportSweepCompleted), new SweepCompletedMessage{ channel = 1, results = sr });
			}

			port.Close();
		}
		
		void ReportSweepStarted(object o)
		{
			if (SweepStarted != null)
			{
				var channel= (int)o;
				SweepStarted(channel);
			}
		}

		void ReportSweepCompleted(object o)
		{
			if (SweepCompleted != null)
			{
				var scm = (SweepCompletedMessage)o;
				SweepCompleted(scm.channel, scm.results);
			}
		}
	}
}
