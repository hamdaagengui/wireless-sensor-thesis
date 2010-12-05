using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RouteSimulator
{
	class RouteContainer
	{
		public List<int> nodes = new List<int>();

		public string Route
		{
			get
			{
				return string.Join(" --> ", nodes);
			}
		}
	}
}
