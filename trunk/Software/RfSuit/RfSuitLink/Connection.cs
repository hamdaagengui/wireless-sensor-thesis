using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;
using Coma.Net.Embedded.PhysicalLayer;
using Coma.Net.Embedded.DataLinkLayer;
using System.Collections.Concurrent;

namespace RfSuit
{
	public delegate void SweepCompletedDelegate(SweepResults[] results);

	public class Connection
	{
		public event SweepCompletedDelegate SweepCompleted;

		const byte LOCAL_ADDRESS = 0;

		IDataLinkLayer dll;
		MessageDispatcher md;

		bool[] devicePresence = new bool[16];
		int tokenRingLength = -1;

		SweepResults[] sweepResults;
		ConcurrentQueue<byte[]> txQueue = new ConcurrentQueue<byte[]>();

		public bool Start(string portName)
		{
			md = new MessageDispatcher();
			md.AddHandler<PingReplyMessage>(m =>
			{
				devicePresence[m.Source - 1] = true;
			});
			md.AddHandler<NothingTokenMessage>(m =>
			{
				if (m.Destination == LOCAL_ADDRESS || m.Destination == tokenRingLength)
				{
					PassToken();
				}
			});
			md.AddHandler<ReportTokenMessage>(m =>
			{
				for (int i = 0; i < m.Rssis.Length; i++)
				{
					sweepResults[m.Source - 1].Rssis[i] = m.Rssis[i];
				}

				if (m.Destination == LOCAL_ADDRESS || m.Destination == tokenRingLength)
				{
					PassToken();
				}
			});

			try
			{
				var sp = new SerialPort(portName, 500000);
				var pl = new SerialPortWrapper(sp);
				dll = new FrameTransceiver(pl);
				dll.FrameReceived += md.HandleFrame;
				dll.Start();
			}
			catch
			{
				return false;
			}

			DetectDevices(); // detect which devices are online and detect the length of the ring



			dll.Send(nothingToken); // start the token passing

			return true;
		}

		public void Stop()
		{
			dll.Stop();
		}

		public bool[] DetectDevices()
		{
			for (byte i = 0; i < devicePresence.Length; i++)
			{
				devicePresence[i] = false;
				dll.Send(PingRequestMessage.Create(i, 0));
				Thread.Sleep(5);
			}

			tokenRingLength = 1;
			for (byte i = 0; i < devicePresence.Length; i++)
			{
				if (devicePresence[i])
				{
					tokenRingLength++;
				}
				else
				{
					break;
				}
			}

			return devicePresence;
		}

		public void Send(byte[] message)
		{
			txQueue.Enqueue(message);
		}

		static readonly byte[] nothingToken = NothingTokenMessage.Create(1, 0);
		void PassToken()
		{
			byte[] msg;
			if (txQueue.TryDequeue(out msg))
			{
				dll.Send(msg);
			}
			else
			{
				dll.Send(nothingToken);
			}
		}
	}
}
