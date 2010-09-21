using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Coma.Ports;

namespace NodeInspector
{
	public partial class MainView : Form
	{
		Communicator com;
		DateTime startTime = DateTime.Now;
		List<FrameEntry> receivedFrames = new List<FrameEntry>();
		List<LogEntry> receivedLogs = new List<LogEntry>();
		UInt32 lostCounter;

		public MainView()
		{
			InitializeComponent();
		}

		private void MainView_Load(object sender, EventArgs e)
		{
			imageList1.Images.Add(SystemIcons.Information);
			imageList1.Images.Add(SystemIcons.Warning);
			imageList1.Images.Add(SystemIcons.Error);
			imageList1.Images.Add(SystemIcons.Application);
			imageList1.Images.Add(SystemIcons.Shield);

			checkBoxShowMessages.ImageIndex = 0;
			checkBoxShowWarnings.ImageIndex = 1;
			checkBoxShowErrors.ImageIndex = 2;
			checkBoxShowApplication.ImageIndex = 3;
			checkBoxShowFramework.ImageIndex = 4;
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
			if (comboBoxPorts.SelectedItem == null)
			{
				MessageBox.Show("Invalid port!");
				return;
			}

			string portName = ((SerialPortInfo)comboBoxPorts.SelectedItem).Name;
			int baudrate;
			if (int.TryParse(textBoxBaudrate.Text, out baudrate) == false)
			{
				MessageBox.Show("Invalid baudrate!");
				return;
			}

			com = new Communicator();
			if (com.Start(portName, baudrate))
			{
				connectToolStripMenuItem.Enabled = false;
				disconnectToolStripMenuItem.Enabled = true;
				startTime = DateTime.Now;
				timerRefreshLists.Enabled = true;
				receivedFrames.Clear();
				receivedLogs.Clear();
				lostCounter = 0;
				UpdateCounters();
			}
			else
			{
				MessageBox.Show("Unable to connect!");
			}
		}

		private void disconnectToolStripMenuItem_Click(object sender, EventArgs e)
		{
			timerRefreshLists.Enabled = false;

			com.Stop();
			connectToolStripMenuItem.Enabled = true;
			disconnectToolStripMenuItem.Enabled = false;
		}

		#region Presentation
		void timerRefreshLists_Tick(object sender, EventArgs e)
		{
			#region Update frames list

			var frames = com.GetReceivedFrames();
			if (frames.Length > 0)
			{
				int oldFrameCount = receivedFrames.Count;

				receivedFrames.AddRange(frames);

				var lvis = new List<ListViewItem>(frames.Length);
				for (int i = 0; i < frames.Length; i++)
				{
					var lvi = CreateFrameListViewItem(oldFrameCount + i);
					if (lvi != null)
					{
						lvis.Add(lvi);
					}
				}

				listViewFrames.Items.AddRange(lvis.ToArray());
			}

			#endregion

			#region Update log list

			var logs = com.GetReceivedLogs();
			if (logs.Length > 0)
			{
				int oldLogCount = receivedLogs.Count;

				receivedLogs.AddRange(logs);

				var lvis = new List<ListViewItem>(logs.Length);
				for (int i = 0; i < logs.Length; i++)
				{
					if (receivedLogs[oldLogCount + i].Type == 0 && checkBoxShowMessages.Checked == false ||
							receivedLogs[oldLogCount + i].Type == 1 && checkBoxShowWarnings.Checked == false ||
							receivedLogs[oldLogCount + i].Type == 2 && checkBoxShowErrors.Checked == false ||
							receivedLogs[oldLogCount + i].Level == 0 && checkBoxShowApplication.Checked == false ||
							receivedLogs[oldLogCount + i].Level == 1 && checkBoxShowFramework.Checked == false)
					{
						continue;
					}

					var lvi = CreateLogListViewItem(oldLogCount + i);
					if (lvi != null)
					{
						lvis.Add(lvi);
					}
				}

				listViewLog.Items.AddRange(lvis.ToArray());

				UpdateCounters();
			}

			#endregion
		}

		ListViewItem CreateFrameListViewItem(int frameId)
		{
			bool delta = toolStripMenuItemShowDeltaTimes.Checked;

			var frame = receivedFrames[frameId];

			var subItems = new string[5];

			subItems[0] = frame.Number.ToString();

			TimeSpan ts;
			if (delta && frameId > 0)
			{
				ts = frame.Time - receivedLogs[frameId - 1].Time;
			}
			else
			{
				ts = frame.Time - startTime;
			}
			subItems[1] = ts.ToString("mm\\:ss\\.FFFFFFF");

			subItems[2] = frame.Source.ToString();

			subItems[3] = frame.Destination.ToString();

			subItems[4] = string.Join<byte>(" ", frame.Payload);

			var lvi = new ListViewItem(subItems);

			lvi.Tag = frame;

			return lvi;
		}
		ListViewItem CreateLogListViewItem(int messageId)
		{
			bool delta = toolStripMenuItemShowDeltaTimes.Checked;

			var msg = receivedLogs[messageId];

			if (msg == null)
			{
				return null;
			}

			var subItems = new string[6];

			subItems[1] = msg.Number.ToString();

			TimeSpan ts;
			if (delta && messageId > 0)
			{
				ts = msg.Time - receivedLogs[messageId - 1].Time;
			}
			else
			{
				ts = msg.Time - startTime;
			}
			subItems[2] = ts.ToString("mm\\:ss\\.FFFFFFF");

			subItems[3] = msg.Level == 0 ? "App" : "Frmwk";

			subItems[4] = msg.Source.ToString();

			subItems[5] = msg.Text;

			var lvi = new ListViewItem(subItems, msg.Type);

			lvi.Tag = msg;

			return lvi;
		}

		void UpdateCounters()
		{
			toolStripStatusLabelFrames.Text = string.Format("Frames: {0} ({1})", listViewFrames.Items.Count, receivedFrames.Count);
			toolStripStatusLabelMessages.Text = string.Format("Logs: {0} ({1})", listViewLog.Items.Count, receivedLogs.Count);
			toolStripStatusLabelLost.Text = string.Format("Lost: {0}", lostCounter);
		}
		#endregion

		private void listViewFrames_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			if (listViewFrames.SelectedItems.Count == 1)
			{
				var msg = (FrameEntry)listViewFrames.SelectedItems[0].Tag;
				richTextBoxFrameInfo.Text = string.Join<byte>(" ", msg.Payload);
			}
		}


		private void testToolStripMenuItem_Click(object sender, EventArgs e)
		{
			timerRefreshLists_Tick(null, null);
			return;
			listViewLog.Items.Add(new ListViewItem("KO!", 0));
			listViewLog.Items.Add(new ListViewItem("HeST!", 1));
			listViewLog.Items.Add(new ListViewItem("PINDSVIN!", 2));

			checkBoxShowMessages.ImageIndex++;
		}
	}
}
