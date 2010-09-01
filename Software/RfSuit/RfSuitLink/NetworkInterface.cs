using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;

namespace RfSuit
{
	class NetworkInterface
	{
		SerialPort prot;

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

			// Send Nothing token and wait for the ring to die

			return count;
		}

		public void StartSweeping(int deviceCount)
		{

		}
	}
}
