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
			EventId = data[0];
		}
	}
}
