using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RfSuit
{
	class NetworkInterface
	{
		public bool Open(string portName, int baudrate)
		{
			return false;
		}

		public void Close()
		{

		}

		public int DetectNumberOfDevices()
		{
			int count = 0;

			// Send tokens and wait for the ring to die

			return count;
		}

		public void StartSweeping(int deviceCount)
		{

		}
	}
}
