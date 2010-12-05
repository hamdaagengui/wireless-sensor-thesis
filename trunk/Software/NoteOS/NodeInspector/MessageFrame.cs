using System;
using System.Text;

namespace NodeInspector
{
	class MessageFrame
	{
		public readonly DateTime TimeStamp;
		public readonly string Text;

		public MessageFrame(DateTime timeStamp, byte[] data)
		{
			TimeStamp = timeStamp;
			Text = Encoding.ASCII.GetString(data);
		}
	}
}
