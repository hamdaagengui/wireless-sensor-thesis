using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MeshPlayground
{
	public partial class Form1 : Form
	{
		List<Node> nodes = new List<Node>();

		public Form1()
		{
			InitializeComponent();
		}

		private void buttonAddNode_Click(object sender, EventArgs e)
		{
			if (nodes.Count < 16)
			{
				nodes.Add(new Node { Id = nodes.Count });
				UpdateNodeList();
			}
		}
		private void buttonRemoveNode_Click(object sender, EventArgs e)
		{
			if (nodes.Count > 0)
			{
				nodes.Remove((from n in nodes where n.Id == nodes.Count - 1 select n).First());
				UpdateNodeList();
			}
		}

		void UpdateNodeList()
		{
			listViewNodes.Items.Clear();

			foreach (var n in nodes)
			{
				var lvi = new ListViewItem(n.Id.ToString());
				lvi.SubItems.Add(string.Join(" ", n.Neighbours.ToArray()));
				lvi.Tag = n;
				listViewNodes.Items.Add(lvi);
			}
		}

		private void listViewNodes_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (listViewNodes.SelectedItems.Count == 1)
			{
				Console.WriteLine(((Node)listViewNodes.SelectedItems[0].Tag).Id);
				propertyGridNodeStats.SelectedObject = (Node)listViewNodes.SelectedItems[0].Tag;
			}
			else
			{
				propertyGridNodeStats.SelectedObject = null;
			}
		}

		private void propertyGridNodeStats_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
		{
		}

		private void propertyGridNodeStats_Validated(object sender, EventArgs e)
		{

			UpdateNodeList();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			var lvi = new ListViewItem("0");
			var si = new ListViewItem.ListViewSubItem(lvi, "-");
			si. Checked = true;

			listView1.Items.Add(lvi);
		}
	}
}
