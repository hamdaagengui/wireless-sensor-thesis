using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Coma.Ports;
using System.Threading.Tasks;

namespace NodeInspector
{
	public partial class MainView : Form
	{
		Communicator com;
		DateTime startTime = DateTime.Now;

		public MainView()
		{
			InitializeComponent();
		}

		private void optionsToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
		{
			comboBoxPorts.Items.Clear();
			comboBoxPorts.Items.AddRange(SerialPortInfo.GetSerialPorts());
			if (comboBoxPorts.SelectedIndex < 0 || comboBoxPorts.Text == "")
			{
				comboBoxPorts.SelectedIndex = 0;
			}
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void connectToolStripMenuItem_Click(object sender, EventArgs e)
		{
			com = new Communicator();
			string portName = ((SerialPortInfo)comboBoxPorts.SelectedItem).Name;
			int baudrate;
			if (int.TryParse(textBoxBaudrate.Text, out baudrate) == false)
			{
				MessageBox.Show("Invalid baudrate!");
				return;
			}
			if (com.Start(portName, baudrate))
			{
				connectToolStripMenuItem.Enabled = false;
				disconnectToolStripMenuItem.Enabled = true;
				startTime = DateTime.Now;
			}
			else
			{
				MessageBox.Show("Unable to connect!");
			}
		}

		private void disconnectToolStripMenuItem_Click(object sender, EventArgs e)
		{
			com.Stop();
			connectToolStripMenuItem.Enabled = true;
			disconnectToolStripMenuItem.Enabled = false;
		}

		private void timerRefreshLists_Tick(object sender, EventArgs e)
		{
			bool delta = toolStripMenuItemShowDeltaTimes.Checked;

			#region Update frames list
			var frames = com.GetReceivedFrames();

			ListViewItem[] lvis = new ListViewItem[frames.Length];
			Parallel.For(0, frames.Length, i =>
			{
				var f = frames[i];
				var lvi = new ListViewItem(f.Number.ToString());
				if (delta)
				{
					if (i > 0)
					{
						lvi.SubItems.Add((f.Time - frames[i - 1].Time).ToString());
					}
					else
					{
						lvi.SubItems.Add(new TimeSpan(0, 0, 0, 0, 0).ToString());
					}
				}
				else
				{
					lvi.SubItems.Add((f.Time - startTime).ToString());
				}
				lvi.SubItems.Add(f.Source.ToString());
				lvi.SubItems.Add(f.Destination.ToString());
				lvi.SubItems.Add(string.Join<byte>(" ", f.Payload));
				lvi.Tag = f;
				lvis[i] = lvi;
			});

			listViewFrames.Items.AddRange(lvis);
			#endregion

			#region Update messages list
			var msgs = com.GetReceivedMessages();

			lvis = new ListViewItem[msgs.Length];
			Parallel.For(0, msgs.Length, i =>
			{
				var m = msgs[i];
				var lvi = new ListViewItem(m.Number.ToString());
				if (delta)
				{
					if (i > 0)
					{
						lvi.SubItems.Add((m.Time - msgs[i - 1].Time).ToString());
					}
					else
					{
						lvi.SubItems.Add(new TimeSpan(0, 0, 0, 0, 0).ToString());
					}
				}
				else
				{
					lvi.SubItems.Add((m.Time - startTime).ToString());
				}
				lvi.SubItems.Add(m.Level.ToString());
				lvi.SubItems.Add(m.Message);
				lvi.Tag = m;
				lvis[i] = lvi;
			});

			listViewFrames.Items.AddRange(lvis);
			#endregion

			#region Update errors list
			var errors = com.GetReceivedErrors();

			lvis = new ListViewItem[errors.Length];
			Parallel.For(0, errors.Length, i =>
			{
				var er = errors[i];
				var lvi = new ListViewItem(er.Number.ToString());
				if (delta)
				{
					if (i > 0)
					{
						lvi.SubItems.Add((er.Time - errors[i - 1].Time).ToString());
					}
					else
					{
						lvi.SubItems.Add(new TimeSpan(0, 0, 0, 0, 0).ToString());
					}
				}
				else
				{
					lvi.SubItems.Add((er.Time - startTime).ToString());
				}
				lvi.SubItems.Add(er.Level.ToString());
				lvi.SubItems.Add(er.Message);
				lvi.Tag = er;
				lvis[i] = lvi;
			});

			listViewFrames.Items.AddRange(lvis);
			#endregion
		}

		private void listViewFrames_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			if (listViewFrames.SelectedItems.Count == 1)
			{
				var msg = (FrameMessage)listViewFrames.SelectedItems[0].Tag;
				richTextBoxFrameInfo.Text = string.Join<byte>(" ", msg.Payload);
			}
		}














		private void testToolStripMenuItem_Click(object sender, EventArgs e)
		{

		}
	}
}
