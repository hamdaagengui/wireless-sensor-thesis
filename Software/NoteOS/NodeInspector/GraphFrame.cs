using System;
using System.Text;

namespace NodeInspector
{
	class GraphFrame
	{
		public readonly DateTime TimeStamp;
		public readonly int Value;

		public GraphFrame(DateTime timeStamp, byte[] data)
		{
			TimeStamp = timeStamp;
			Value = (sbyte)data[0];
		}
	}
}
