using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace RouteSimulator
{
	public partial class MainView : Form
	{
		static List<LinkLossValues> linkLosses = new List<LinkLossValues>(15);
		BindingList<LinkLossValues> linkLossBindingList = new BindingList<LinkLossValues>(linkLosses);
		RouteBuildingInfo routeBuildingInfo = new RouteBuildingInfo();
		CostValues[] costs = new CostValues[15];
		List<RouteContainer> routes = new List<RouteContainer>();

		Random r = new Random(DateTime.Now.Millisecond);

		public MainView()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			for (int i = 0; i < 15; i++)
			{
				linkLosses.Add(new LinkLossValues { Node = i });
				for (int j = 0; j < 15; j++)
				{
					if (i != j)
					{
						linkLosses[i].Loss[j] = r.Next(40, 90);
					}
				}
			}
			dataGridViewLinkLosses.DataSource = linkLossBindingList;

			routeBuildingInfo.HopCost = 20;
			propertyGridRouteBuildingInfo.SelectedObject = routeBuildingInfo;

			for (int i = 0; i < 15; i++)
			{
				costs[i] = new CostValues { Node = i };
			}
			dataGridViewCosts.DataSource = costs;
			dataGridViewRoutes.DataSource = routes;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			CalculateCosts();
			DoDijkstra();
			BuildRoutingTable();

			dataGridViewCosts.Refresh();
			dataGridViewRoutes.Refresh();
		}


		int CalculateHopCost(int source, int destination)
		{
			int currentHopCost = linkLosses[source].Loss[destination];
			if (currentHopCost != 0) // source can see destination
			{
				return currentHopCost * currentHopCost + routeBuildingInfo.HopCost;
			}
			else
			{
				return 0;
			}
		}
		void CalculateCosts()
		{
			for (int i = 0; i < 15; i++) // loop all nodes
			{
				for (int j = 0; j < 15; j++) // loop all destination nodes
				{
					if (i != j) // skip self
					{
						costs[i].Costs[j] = CalculateHopCost(i, j);
					}
				}
			}
		}

		void BuildRoutingTable()
		{

		}

		int PickCheapestNode(Node[] pool)
		{
			int lowestCost = INFINITY;
			int index = -1;

			for (int i = 0; i < 15; i++)
			{
				if (pool[i].marked == false)
				{
					if (pool[i].cost < lowestCost)
					{
						index = i;
					}
				}
			}

			return index;
		}

		const int INFINITY = 0x7fffffff;
		void DoDijkstra()
		{
			Node[] nodes = new Node[15];
			for (int i = 0; i < 15; i++)
			{
				Node v = nodes[i] = new Node();
				for (int j = 0; j < 15; j++)
				{
					v.costToNeighbor[j] = costs[i].Costs[j];
				}
			}

			// Start Dijkstra
			int start = Convert.ToInt32(numericUpDownSender.Value);
			int target = Convert.ToInt32(numericUpDownReceiver.Value);

			for (int i = 0; i < 15; i++)
			{
				nodes[i].marked = false;
				nodes[i].cost = INFINITY;
				nodes[i].previous = -1;
			}
			nodes[start].cost = 0;

			for (int i = 0; i < 15; i++)
			{
				int vIndex = PickCheapestNode(nodes);
				if (vIndex == -1)
				{
					// no route found
					break;
				}
				if (vIndex == target)
				{
					// found shortest route
					List<int> route = new List<int>();
					Node n = nodes[target];
					int c = target;
					while (c != -1)
					{
						route.Insert(0, c);
						c = nodes[c].previous;
					}
					Console.WriteLine(string.Join(" --> ", route));
					break;
				}
				Node current = nodes[vIndex];
				for (int j = 0; j < 15; j++)
				{
					if (current.costToNeighbor[j] != 0)
					{
						Node neighbor = nodes[j];
						int newCost = current.cost + current.costToNeighbor[j];
						if (newCost < neighbor.cost)
						{
							neighbor.cost = newCost;
							neighbor.previous = vIndex;
						}
					}
				}

				current.marked = true; // done with node
			}
		}

		private void buttonReadData_Click(object sender, EventArgs e)
		{
			using (var d = new OpenFileDialog())
			{
				if (d.ShowDialog() == System.Windows.Forms.DialogResult.OK)
				{
					try
					{
						for (int i = 0; i < 15; i++)
						{
							linkLosses.Add(new LinkLossValues { Node = i });
						}

						var lines = File.ReadAllLines(d.FileName).Skip(1);
						foreach (var l in lines)
						{
							var p = l.Split(';');
							int a = int.Parse(p[0]);
							int b = int.Parse(p[1]);
							double v = double.Parse(p[2].Replace('.', ','));

							linkLosses[a].Loss[b] = Convert.ToInt32(v);
							linkLosses[b].Loss[a] = Convert.ToInt32(v);
						}
					}
					catch (Exception ex)
					{
						MessageBox.Show("Negativ");
					}
				}
			}
		}
	}

	class Node
	{
		public int[] costToNeighbor = new int[15];

		public bool marked;
		public int cost;
		public int previous;
	}
}
