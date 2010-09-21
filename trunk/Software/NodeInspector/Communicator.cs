using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Coma.Net.Embedded.DataLinkLayer;
using Coma.Net.Embedded.PhysicalLayer;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Concurrent;

namespace NodeInspector
{
	public delegate void DeviceFoundDelegate(int deviceId);
	public delegate void DeviceLostDelegate(int deviceId);

	class Communicator
	{
		public event DeviceFoundDelegate DeviceFound;
		public event DeviceLostDelegate DeviceLost;

		public List<FrameMessage> receivedFrames;
		public List<MessageMessage> receivedMessages;
		public List<ErrorMessage> receivedErrors;

		FrameTransceiver dll;
		MessageDispatcher md;

		AutoResetEvent pingingDevice = new AutoResetEvent(false);
		int deviceBeingPinged;
		DateTime pingStartTime;

		public Communicator()
		{

		}

		public bool Start(string portName, int baudrate)
		{
			try
			{
				receivedFrames = new List<FrameMessage>();
				receivedMessages = new List<MessageMessage>();
				receivedErrors = new List<ErrorMessage>();

				md = new MessageDispatcher();
				md.MessagePredicate += (d, s) => { return d == 0 || d == 255; };
				md.AddHandler<PingReplyMessage>(m => { });
				md.AddHandler<ReportFrameMessage>(m =>
				{
					lock (receivedFrames)
					{
						receivedFrames.Add(new FrameMessage { Destination = m.To, Source = m.From, Number = m.Number, Time = DateTime.Now, Payload = m.Payload });
					}
				});

				var sp = new SerialPort(portName, baudrate);
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
			if (dll != null)
			{
				dll.Stop();
			}
		}

		public FrameMessage[] GetReceivedFrames()
		{
			FrameMessage[] msgs;

			lock (receivedFrames)
			{
				msgs = receivedFrames.ToArray();
				receivedFrames.Clear();
			}

			return msgs;
		}

		public MessageMessage[] GetReceivedMessages()
		{
			MessageMessage[] msgs;

			lock (receivedMessages)
			{
				msgs = receivedMessages.ToArray();
				receivedMessages.Clear();
			}

			return msgs;
		}

		public ErrorMessage[] GetReceivedErrors()
		{
			ErrorMessage[] msgs;

			lock (receivedErrors)
			{
				msgs = receivedErrors.ToArray();
				receivedErrors.Clear();
			}

			return msgs;
		}



		public TimeSpan Ping(int deviceId)
		{

			return new TimeSpan();
		}

		public void EnableFrameReplaying(int deviceId)
		{

		}

		public void DisableFrameReplaying(int deviceId)
		{

		}

		public void EnableLogReports(int deviceId)
		{

		}

		public void DisableLogReports(int deviceId)
		{

		}

		public void EnableErrorReports(int deviceId)
		{

		}

		public void DisableErrorReports(int deviceId)
		{

		}
	}
}
