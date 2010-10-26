using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace NodeInspector
{
	class LogEntry
	{
		static int NextNumber = 0;

		public EventDescriptor EventDescriptor;
		public DateTime Time;
		public int Number;

		public LogEntry(EventDescriptor eventDescriptor, DateTime time)
		{
			EventDescriptor = eventDescriptor;
			Time = time;
			Number = Interlocked.Increment(ref NextNumber);
		}

		public ListViewItem ToListViewItem()
		{
			return new ListViewItem(new string[] { "", Number.ToString(), Time.TimeOfDay.ToString(), EventDescriptor.Subsystem, EventDescriptor.Module, EventDescriptor.Details }, EventDescriptor.Level);
		}
	}
}
