using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RfSuit
{
	public delegate void SweepStartedDelegate(int channel);
	public delegate void SweepCompletedDelegate(SweepResults[] results);

	public class Connection
	{
		public event SweepStartedDelegate SweepStarted;
		public event SweepCompletedDelegate SweepCompleted;

		public void Start(string portname)
		{

		}
	}
}
