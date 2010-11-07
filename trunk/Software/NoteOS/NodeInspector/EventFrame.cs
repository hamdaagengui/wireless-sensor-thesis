using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NodeInspector
{
	class EventFrame
	{
		public DateTime TimeStamp;
		public int EventId;

		public EventFrame(DateTime timeStamp, byte[] data)
		{
			TimeStamp = timeStamp;
			EventId = data[0];
		}
	}
}
