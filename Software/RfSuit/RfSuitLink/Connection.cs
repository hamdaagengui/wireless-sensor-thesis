using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;
using Coma.Net.Embedded.PhysicalLayer;
using Coma.Net.Embedded.DataLinkLayer;

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

		IDataLinkLayer dll;
		MessageDispatcher md;

		public bool Start(string portName)
		{
			md = new MessageDispatcher();
			md.AddHandler<NothingMessage>(m =>
			{
				if (m.Destination == 0)
				{
					PassToken();
				}
			});
			md.AddHandler<ReportMessage>(m =>
			{
				if (m.Destination == 0)
				{
					PassToken();
				}
			});

			try
			{
				var sp = new SerialPort(portName, 115200);
				var pl = new SerialPortWrapper(sp);
				dll = new FrameTransceiver(pl);
				dll.FrameReceived += md.HandleFrame;
				dll.Start();
			}
			catch
			{
				return false;
			}

			return true;
		}

		public void Stop()
		{
			dll.Stop();
		}

		void PassToken()
		{

		}

		void ReportSweepStarted(object o)
		{
			if (SweepStarted != null)
			{
				var channel = (int)o;
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
