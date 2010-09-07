﻿using System;
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
					qualityMatrix[m.Source - 1, i] = ((int)m.Rssis[i]) - 256;
				}

				if (m.Destination == tokenRingLength) // last device in the ring means a full sweep has been made
				{
					Task.Factory.StartNew(() =>
					{
						var now = DateTime.Now;
						var time = now - lastTime;
						lastTime = now;
						Console.WriteLine("Rx Report @ " + string.Format("{0:0.0}", 1000.0 / time.TotalMilliseconds) + " reports per second");

						foreach (var lqi in lqis) // calculate lqi as the average of sensed RSSI by either end point of a link
						{
							int a = lqi.EndPointA;
							int b = lqi.EndPointB;
							lqi.Quality = (qualityMatrix[a, b] + qualityMatrix[b, a]) / 2.0;

							Console.WriteLine("{0} <--> {1}: {2} dBm", a, b, lqi.Quality);
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
