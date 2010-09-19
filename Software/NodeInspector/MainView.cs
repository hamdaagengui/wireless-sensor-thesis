using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NodeInspector
{
	public partial class MainView : Form
	{
		public MainView()
		{
			InitializeComponent();
		}

		private void optionsToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
		{
			comboBoxPorts.Items.Clear();
			foreach (var spi in Coma.Ports.SerialPortInfo.GetSerialPorts())
			{
				comboBoxPorts.Items.Add(spi);
			}
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}
