using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Globalization;

namespace NodeInspector
{
	class LogEntry
	{
		static int NextNumber = 1;

		static public void ResetNumbering()
		{
			NextNumber = 1;
		}

		public EventDescriptor EventDescriptor;
		public DateTime Time;
		public int Number;

		public LogEntry(EventDescriptor eventDescriptor, DateTime time)
		{
			EventDescriptor = eventDescriptor;
			Time = time;
			Number = NextNumber++;
		}

		public ListViewItem ToListViewItem()
		{
			return new ListViewItem(new string[] { "", Number.ToString(), Time.ToString("HH:mm:ss.ffffff"), EventDescriptor.Subsystem, EventDescriptor.Module, EventDescriptor.Details }, EventDescriptor.Level);
		}
	}
}
