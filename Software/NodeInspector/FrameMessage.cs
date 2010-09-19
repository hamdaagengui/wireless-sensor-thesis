using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NodeInspector
{
	class FrameMessage
	{
		public int Number { get; set; }
		public DateTime Time { get; set; }
		public int Source { get; set; }
		public int Destination { get; set; }
		public byte[] Payload { get; set; }

		public FrameMessage()
		{
			Payload = new byte[0];
		}
	}
}
