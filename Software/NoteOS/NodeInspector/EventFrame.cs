using System;

namespace NodeInspector
{
	class EventFrame
	{
		public readonly DateTime TimeStamp;
		public readonly int EventId;

		public EventFrame(DateTime timeStamp, byte[] data)
		{
			TimeStamp = timeStamp;
			//long t = data[3] * 24 + data[2] * 16 + data[1] * 8 + data[0];
			//TimeStamp = DateTime.FromFileTime(t);
			EventId = data[0];
		}
	}
}
