using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NodeInspector
{
	class EventDescriptor
	{
		public int Id;
		public int Level;
		public string Subsystem;
		public string Module;
		public string Details;
		public string Script;

		static public EventDescriptor Parse(string description)
		{
			// int;int;string;string;string;string
			// id;level;subsystem;module;details;script

			EventDescriptor ed = new EventDescriptor();

			var parts = description.Split(new char[] { ';' });
			if (parts.Length != 5 && parts.Length!=6)
			{
				return null;
			}

			if (int.TryParse(parts[0], out ed.Id) == false)
			{
				return null;
			}

			if (int.TryParse(parts[1], out ed.Level) == false)
			{
				return null;
			}

			ed.Subsystem = parts[2];

			ed.Module = parts[3];

			ed.Details = parts[4];

			if (parts.Length == 6)
			{
				ed.Script = parts[5];
			}
			else
			{
				ed.Script = "";
			}

			return ed;
		}
	}
}
