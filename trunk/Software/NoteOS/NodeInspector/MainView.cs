using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NodeInspector.Properties;
using System.IO;
using System.Threading.Tasks;
using System.IO.Ports;
using Coma.Ports;
using System.Collections.Concurrent;

namespace NodeInspector
{
	public partial class MainView : Form
	{
		SerialPort port;
		ConcurrentQueue<PortQueueElement> portQueue = new ConcurrentQueue<PortQueueElement>();
		Dictionary<int, EventDescriptor> eventDescriptors = new Dictionary<int, EventDescriptor>();
		List<LogEntry> logs = new List<LogEntry>();

		public MainView()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
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

			LoadEventDescriptors();

			UpdatePortList();

			var ports = SerialPortInfo.GetSerialPorts();
			foreach (var p in ports)
			{
				if (p.ToString() == Settings.Default.Port)
				{
					comboBoxPort.SelectedItem = p;
				}
			}
		}

		private void MainView_FormClosing(object sender, FormClosingEventArgs e)
		{
			Settings.Default.Save();
		}

		void LoadEventDescriptors()
		{
			foreach (var line in File.ReadAllLines("EventDescriptors.txt"))
			{
				var ed = EventDescriptor.Parse(line);
				if (ed == null)
				{
					// error in descriptor
				}
				else
				{
					eventDescriptors[ed.Id] = ed;
				}
			}
		}

		void UpdateListView()
		{
			bool[] levels = new bool[3];
			levels[0] = checkBoxShowMessages.Checked;
			levels[1] = checkBoxShowWarnings.Checked;
			levels[2] = checkBoxShowErrors.Checked;

			var lvis = new List<ListViewItem>();

			foreach (var log in logs)
			{
				if (levels[log.EventDescriptor.Level])
				{
					lvis.Add(log.ToListViewItem());
				}
			}

			listViewLogs.SuspendLayout();
			listViewLogs.Items.Clear();
			listViewLogs.Items.AddRange(lvis.ToArray());
			listViewLogs.ResumeLayout();
		}

		private void LevelCheckboxesChanged(object sender, EventArgs e)
		{
			UpdateListView();
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			bool[] levels = new bool[3];
			levels[0] = checkBoxShowMessages.Checked;
			levels[1] = checkBoxShowWarnings.Checked;
			levels[2] = checkBoxShowErrors.Checked;

			var lvis = new List<ListViewItem>();

			PortQueueElement pqe;
			while (portQueue.TryDequeue(out pqe))
			{
				LogEntry log = null;
				if (eventDescriptors.ContainsKey(pqe.Id))
				{
					log = new LogEntry(eventDescriptors[pqe.Id], pqe.Time);
				}
				else
				{
					if (eventDescriptors.ContainsKey(255))
					{
						log = new LogEntry(eventDescriptors[255], pqe.Time);
					}
				}

				if (log != null)
				{
					logs.Add(log);

					if (levels[log.EventDescriptor.Level])
					{
						lvis.Add(log.ToListViewItem());
					}
				}
			}

			listViewLogs.Items.AddRange(lvis.ToArray());
		}

		void UpdatePortList()
		{
			comboBoxPort.Items.Clear();
			comboBoxPort.Items.AddRange(SerialPortInfo.GetSerialPorts());
		}

		private void checkBoxRun_CheckedChanged(object sender, EventArgs e)
		{
			if (checkBoxRun.Checked)
			{
				try
				{
					var spi = (SerialPortInfo)comboBoxPort.SelectedItem;
					port = new SerialPort(spi.Name, int.Parse(comboBoxBaudrate.Text));
					port.Open();
					port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
				}
				catch
				{
					checkBoxRun.Checked = false;
				}
			}
			else
			{
				if (port != null)
				{
					if (port.IsOpen)
					{
						port.Close();
					}
					port = null;
				}
			}
		}

		void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			portQueue.Enqueue(new PortQueueElement { Id = (byte)port.ReadByte(), Time = DateTime.Now });
		}

		private void listViewLogs_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			logs.Clear();
			UpdateListView();
		}
	}
}
