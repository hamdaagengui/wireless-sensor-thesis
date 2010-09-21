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

		public List<FrameEntry> receivedFrames;
		public List<LogEntry> receivedLogs;

		const byte BROADCAST_ID = 255;
		const byte LOCAL_ID = 250;

		FrameTransceiver dll;
		MessageDispatcher md;

		AutoResetEvent pingingDevice = new AutoResetEvent(false);
		int deviceBeingPinged;
		DateTime pingStartTime;

		UInt32 nextFrameEntryNumber;
		UInt32 nextLogEntryNumber;

		public Communicator()
		{

		}

		public bool Start(string portName, int baudrate)
		{
			nextFrameEntryNumber = 1;
			nextLogEntryNumber = 1;

			try
			{
				receivedFrames = new List<FrameEntry>();
				receivedLogs = new List<LogEntry>();

				md = new MessageDispatcher();
				md.MessagePredicate += (d, s) => { return d == LOCAL_ID || d == BROADCAST_ID; };
				md.AddHandler<PingReplyMessage>(m => { });
				md.AddHandler<ReportFrameMessage>(m =>
				{
					lock (receivedFrames)
					{
						receivedFrames.Add(new FrameEntry { Number = nextFrameEntryNumber++, Destination = m.To, Source = m.From, Time = DateTime.Now, Payload = m.Payload });
					}
				});
				md.AddHandler<ReportApplicationMessageMessage>(m =>
				{
					lock (receivedLogs)
					{
						receivedLogs.Add(new LogEntry { Type = LogEntry.TYPE_MESSAGE, Level = LogEntry.LEVEL_APPLICATION, Number = nextLogEntryNumber++, Source = m.Source, Time = DateTime.Now, Text = ASCIIEncoding.ASCII.GetString(m.Message) });
					}
				});
				md.AddHandler<ReportApplicationWarningMessage>(m =>
				{
					lock (receivedLogs)
					{
						receivedLogs.Add(new LogEntry { Type = LogEntry.TYPE_WARNING, Level = LogEntry.LEVEL_APPLICATION, Number = nextLogEntryNumber++, Source = m.Source, Time = DateTime.Now, Text = ASCIIEncoding.ASCII.GetString(m.Message) });
					}
				});
				md.AddHandler<ReportApplicationErrorMessage>(m =>
				{
					lock (receivedLogs)
					{
						receivedLogs.Add(new LogEntry { Type = LogEntry.TYPE_ERROR, Level = LogEntry.LEVEL_APPLICATION, Number = nextLogEntryNumber++, Source = m.Source, Time = DateTime.Now, Text = ASCIIEncoding.ASCII.GetString(m.Message) });
					}
				});
				md.AddHandler<ReportFrameworkMessageMessage>(m =>
				{
					lock (receivedLogs)
					{
						receivedLogs.Add(new LogEntry { Type = LogEntry.TYPE_MESSAGE, Level = LogEntry.LEVEL_FRAMEWORK, Number = nextLogEntryNumber++, Source = m.Source, Time = DateTime.Now, Text = ASCIIEncoding.ASCII.GetString(m.Message) });
					}
				});
				md.AddHandler<ReportFrameworkWarningMessage>(m =>
				{
					lock (receivedLogs)
					{
						receivedLogs.Add(new LogEntry { Type = LogEntry.TYPE_WARNING, Level = LogEntry.LEVEL_FRAMEWORK, Number = nextLogEntryNumber++, Source = m.Source, Time = DateTime.Now, Text = ASCIIEncoding.ASCII.GetString(m.Message) });
					}
				});
				md.AddHandler<ReportFrameworkErrorMessage>(m =>
				{
					lock (receivedLogs)
					{
						receivedLogs.Add(new LogEntry { Type = LogEntry.TYPE_ERROR, Level = LogEntry.LEVEL_FRAMEWORK, Number = nextLogEntryNumber++, Source = m.Source, Time = DateTime.Now, Text = ASCIIEncoding.ASCII.GetString(m.Message) });
					}
				});
				md.AddHandler<ReportMessagesDroppedMessage>(m =>
				{
					nextFrameEntryNumber += m.FramesDropped;
					nextLogEntryNumber += m.LogsDropped;
				});

				Task.Factory.StartNew(() =>
				{
					while (true)
					{
						md.HandleFrame(ReportApplicationMessageMessage.Create(LOCAL_ID, 123, new byte[30] { 65, 66, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));

						md.HandleFrame(ReportFrameMessage.Create(LOCAL_ID, 123, 1, 2, new byte[32] { 65, 0, 66, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));

						Thread.Sleep(200);
					}
				});

				return true;

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

		public FrameEntry[] GetReceivedFrames()
		{
			FrameEntry[] frames;

			lock (receivedFrames)
			{
				frames = receivedFrames.ToArray();
				receivedFrames.Clear();
			}

			return frames;
		}

		public LogEntry[] GetReceivedLogs()
		{
			LogEntry[] logs;

			lock (receivedLogs)
			{
				logs = receivedLogs.ToArray();
				receivedLogs.Clear();
			}

			return logs;
		}

		public TimeSpan Ping(int deviceId)
		{

			return new TimeSpan();
		}

		public void ConfigureReporting(int deviceId, bool enableFrames, bool enableMessages, bool enableWarnings, bool enableErrors, bool enableApplicationLevel, bool enableFrameworkLevel)
		{
			dll.Send(ConfigureReportingMessage.Create((byte)deviceId, LOCAL_ID, ToByte(enableFrames), ToByte(enableMessages), ToByte(enableWarnings), ToByte(enableErrors), ToByte(enableApplicationLevel), ToByte(enableFrameworkLevel)));
		}

		// Helpers

		byte ToByte(bool value)
		{
			if (value)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}
