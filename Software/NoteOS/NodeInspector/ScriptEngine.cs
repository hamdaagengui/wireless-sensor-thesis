using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NodeInspector
{
	class ScriptEngine
	{
		public Dictionary<string, DynamicVariable> Variables;

		public string GetVariableName(string script)
		{
			var p = script.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

			if (p.Length > 1)
			{
				return p[0];
			}
			else
			{
				return "";
			}
		}

		public void RunScript(string script)
		{
			var p = script.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

			if (p.Length < 2)
			{
				return;
			}
			try
			{
				switch (p[1])
				{
					case "++":
						Variables[p[0]].Value++;
						break;

					case "--":
						Variables[p[0]].Value--;
						break;

					case "=":
						Variables[p[0]].Value = int.Parse(p[2]);
						break;

					default:
						break;
				}
			}
			catch { }
		}
	}
}
