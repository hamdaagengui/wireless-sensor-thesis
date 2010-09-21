using System;

namespace NodeInspector
{
	class FrameEntry
	{
		public UInt32 Number;
		public DateTime Time;
		public int Source;
		public int Destination;
		public byte[] Payload = new byte[0];
	}
}
