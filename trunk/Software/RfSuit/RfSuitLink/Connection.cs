using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;
using Coma.Net.Embedded.DataLinkLayer;
using Coma.Net.Embedded.PhysicalLayer;
using dk.iha;

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

		ConcurrentQueue<byte[]> txQueue;

		LinkQualityIndicator[] lqis;
		double[,] qualityMatrix;

		DateTime lastTime = DateTime.Now;
		public bool Start(string portName)
		{
			txQueue = new ConcurrentQueue<byte[]>();

			md = new MessageDispatcher();
			md.AddHandler<PingReplyMessage>(m =>
			{
				devicePresence[m.Source - 1] = true;
				Console.WriteLine("Device {0} is present [{1:0.0} ms]", m.Source, (DateTime.Now - pingTimer).TotalMilliseconds);
			});
			md.AddHandler<NothingTokenMessage>(m =>
			{
				TokenReceived(m.Destination);
			});
			md.AddHandler<ReportTokenMessage>(m =>
			{
				TokenReceived(m.Destination);

				for (int i = 0; i < tokenRingLength - 1; i++)
				{
					qualityMatrix[m.Source - 1, i] = ((sbyte)m.Rssis[i]);
				}

				if (m.Destination == tokenRingLength) // last device in the ring means a full sweep has been made
				{
					Task.Factory.StartNew(() =>
					{
						var now = DateTime.Now;
						var time = now - lastTime;
						lastTime = now;

						foreach (var lqi in lqis) // calculate lqi as the average of sensed RSSI by either end point of a link
						{
							int a = lqi.EndPointA;
							int b = lqi.EndPointB;
							lqi.Quality = (qualityMatrix[a, b] + qualityMatrix[b, a]) / 2.0;
						}

						if (SweepCompleted != null)
						{
							SweepCompleted(lqis);
						}
					});
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

			Thread.Sleep(200);

			DetectDevices(); // detect which devices are online and detect the length of the ring

			List<LinkQualityIndicator> l = new List<LinkQualityIndicator>();
			qualityMatrix = new double[tokenRingLength - 1, tokenRingLength - 1];

			for (int i = 0; i < tokenRingLength - 1; i++)
			{
				for (int j = 0; j < tokenRingLength - 1; j++)
				{
					qualityMatrix[i, j] = 0.0;

					if (i < j)
					{
						l.Add(new LinkQualityIndicator { EndPointA = i, EndPointB = j, Quality = 0.0 });
					}
				}
			}

			lqis = l.ToArray();

			Console.WriteLine("Found " + (tokenRingLength - 1) + " devices");

			dll.Send(nothingToken); // start the token passing

			Console.WriteLine("Started");

			return true;
		}

		public void Stop()
		{
			md = null;
			dll.Stop();
		}

		DateTime pingTimer;
		public bool[] DetectDevices()
		{
			for (int i = 0; i < devicePresence.Length; i++)
			{
				devicePresence[i] = false;
			}

			for (byte i = 1; i <= devicePresence.Length; i++)
			{
				if (i > 1 && devicePresence[i - 2] == false)
				{
					break;
				}

				pingTimer = DateTime.Now;
				dll.Send(PingRequestMessage.Create(i, 0));
				Thread.Sleep(200);
			}

			tokenRingLength = 1;
			for (int i = 0; i < devicePresence.Length; i++)
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

		public void SetTxPower(byte power)
		{
			Send(SetTxPowerTokenMessage.Create(1, 0, power));
		}

		public void SetChannel(byte channel)
		{
			Send(SetChannelTokenMessage.Create(1, 0, channel));
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

			//	Thread.Sleep(100);

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
