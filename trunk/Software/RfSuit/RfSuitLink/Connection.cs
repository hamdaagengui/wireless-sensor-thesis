using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;
using Coma.Net.Embedded.PhysicalLayer;
using Coma.Net.Embedded.DataLinkLayer;
using System.Collections.Concurrent;
using System.Threading.Tasks;

namespace RfSuit
{
	public class Connection : IConnection
	{
		public event SweepCompletedDelegate SweepCompleted;

		const byte LOCAL_ADDRESS = 0;

		static readonly byte[] nothingToken = NothingTokenMessage.Create(1, 0);

		IDataLinkLayer dll;
		MessageDispatcher md;

		bool[] devicePresence = new bool[16];
		int tokenRingLength = -1;

		SweepResults[] sweepResults;
		ConcurrentQueue<byte[]> txQueue = new ConcurrentQueue<byte[]>();

		DateTime lastTime = DateTime.Now;
		public bool Start(string portName)
		{
			md = new MessageDispatcher();
			md.AddHandler<PingReplyMessage>(m =>
			{
				devicePresence[m.Source - 1] = true;
			});
			md.AddHandler<NothingTokenMessage>(m =>
			{
				TokenReceived(m.Destination);
			});
			md.AddHandler<ReportTokenMessage>(m =>
			{
				TokenReceived(m.Destination);

				for (int i = 0; i < sweepResults.Length; i++)
				{
					sweepResults[m.Source - 1].Rssis[i] = ((int)m.Rssis[i]) - 256;
				}

				if (m.Source == (tokenRingLength - 1)) // last device in the ring means a full sweep has been made
				{
					Task.Factory.StartNew(() =>
					{
						var now = DateTime.Now;
						var time = now - lastTime;
						lastTime = now;
						Console.WriteLine("Rx Report @ " + string.Format("{0:0.0}", 1000.0 / time.TotalMilliseconds) + " reports per second");
						for (int i = 0; i < sweepResults.Length; i++)
						{
							for (int j = 0; j < sweepResults.Length; j++)
							{
								//if (i != j)
								//{
								Console.WriteLine("{0} <--> {1}: {2} dBm", i, j, sweepResults[i].Rssis[j]);
								//}
							}
						}
						if (SweepCompleted != null)
						{
							SweepCompleted(sweepResults);
						}
					});
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

			DetectDevices(); // detect which devices are online and detect the length of the ring

			sweepResults = new SweepResults[tokenRingLength - 1];
			for (int i = 0; i < sweepResults.Length; i++)
			{
				sweepResults[i] = new SweepResults();
				sweepResults[i].Rssis = new int[sweepResults.Length];
			}

			Console.WriteLine("Found " + (tokenRingLength - 1) + " devices");

			dll.Send(nothingToken); // start the token passing

			Console.WriteLine("Started");

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
				dll.Send(PingRequestMessage.Create((byte)(i + 1), 0));
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

		void Send(byte[] message)
		{
			txQueue.Enqueue(message);
		}

		void TokenReceived(byte destination)
		{
			if (destination != LOCAL_ADDRESS && destination != tokenRingLength)
			{
				return;
			}

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
