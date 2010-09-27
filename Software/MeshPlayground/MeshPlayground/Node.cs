using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MeshPlayground
{
	class Node
	{
		public int Id { get; set; }
		public List<int> Neighbours { get; set; }

		public Node()
		{
			Neighbours = new List<int>();
		}
	}
}
