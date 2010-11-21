using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Windows.Forms;
using Coma.Ports;
using NodeInspector.Properties;
using System.ComponentModel;
using System.Threading;
using System.Diagnostics;

namespace NodeInspector
{
	public partial class MainView : Form
	{
		SerialPort port;

		ConcurrentQueue<EventFrame> eventFrameQueue = new ConcurrentQueue<EventFrame>();

		long bytesReceived = 0;
		long framesReceived = 0;

		Dictionary<int, EventDescriptor> eventDescriptors = new Dictionary<int, EventDescriptor>();
		List<LogEntry> logs = new List<LogEntry>();
		Dictionary<string, DynamicVariable> variables = new Dictionary<string, DynamicVariable>();
		BindingList<DynamicVariable> variableList = new BindingList<DynamicVariable>();
		ScriptEngine se = new ScriptEngine();

		public MainView()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			imageList1.Images.Add(SystemIcons.Information);
			imageList1.Images.Add(SystemIcons.Warning);
			imageList1.Images.Add(SystemIcons.Error);

			checkBoxShowMessages.ImageIndex = 0;
			checkBoxShowWarnings.ImageIndex = 1;
			checkBoxShowErrors.ImageIndex = 2;

			//dataGridView1.AutoGenerateColumns = false;
			//dataGridView1.Columns.Add(new DataGridViewTextBoxColumn() { DataPropertyName = "Name", HeaderText = "Variable" });
			//dataGridView1.Columns.Add(new DataGridViewTextBoxColumn() { DataPropertyName = "Value", HeaderText = "Value" });
			dataGridView1.DataSource = variableList;

			LoadEventDescriptors();

			se.Variables = variables;

			UpdatePortList();

			var ports = SerialPortInfo.GetSerialPorts();
			foreach (var p in ports)
			{
				if (p.ToString() == Settings.Default.Port)
				{
					comboBoxPort.Text = p.ToString();
				}
			}

			checkBoxRun_CheckedChanged(null, null);
		}

		private void MainView_FormClosing(object sender, FormClosingEventArgs e)
		{
			Settings.Default.Save();
		}

		void LoadEventDescriptors()
		{
			string path;
			if (File.Exists("EventDescriptors.txt"))
			{
				path = "EventDescriptors.txt";
			}
			else if (File.Exists("../../EventDescriptors.txt"))
			{
				path = "../../EventDescriptors.txt";
			}
			else
			{
				MessageBox.Show("No event descriptor file (\"EventDescriptors.txt\") found.", "Node Inspector", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return;
			}

			List<string> badDescriptors = new List<string>();

			foreach (var line in File.ReadAllLines(path))
			{
				string l = line;
				if (l.Contains("//"))
				{
					l = line.Remove(l.IndexOf("//"));
				}
				l = l.Trim();
				if (l.Length > 0)
				{
					var ed = EventDescriptor.Parse(line);
					if (ed == null)
					{
						// error in descriptor
						badDescriptors.Add(line);
					}
					else
					{
						eventDescriptors[ed.Id] = ed;
						if (ed.Script != "")
						{
							var name = se.GetVariableName(ed.Script);
							if (variables.ContainsKey(name) == false)
							{
								var dv = new DynamicVariable() { Name = name, Value = 0 };
								variables.Add(name, dv);
								variableList.Add(dv);
							}
						}
					}
				}
			}

			if (badDescriptors.Count > 10)
			{
				MessageBox.Show("Many of the event descriptors are bad. Please check EventDescriptors.txt.", "Node Inspector", MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}
			else if (badDescriptors.Count > 0)
			{
				var s = "The following bad descriptors were incountered:\n" + string.Join("\n", badDescriptors.ToArray());
				MessageBox.Show(s, "Node Inspector", MessageBoxButtons.OK, MessageBoxIcon.Warning);
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
			bool changes = false;

			bool[] levels = new bool[3];
			levels[0] = checkBoxShowMessages.Checked;
			levels[1] = checkBoxShowWarnings.Checked;
			levels[2] = checkBoxShowErrors.Checked;

			var lvis = new List<ListViewItem>();

			int frameCounter = 100;
			EventFrame ef;
			while (eventFrameQueue.TryDequeue(out ef) && (frameCounter-- > 0))
			{
				changes = true;
				LogEntry log = null;
				if (eventDescriptors.ContainsKey(ef.EventId))
				{
					log = new LogEntry(eventDescriptors[ef.EventId], ef.TimeStamp);
				}
				else
				{
					if (eventDescriptors.ContainsKey(255))
					{
						log = new LogEntry(eventDescriptors[255], ef.TimeStamp);
					}
				}

				if (log != null)
				{
					logs.Add(log);

					se.RunScript(log.EventDescriptor.Script);

					if (levels[log.EventDescriptor.Level])
					{
						lvis.Add(log.ToListViewItem());
					}
				}
			}

			if (changes)
			{
				bool doScroll = false;
				//if (listViewLogs.Items.Count > 0)
				//{
				//  int y = listViewLogs.Items[listViewLogs.Items.Count - 1].Position.Y;
				//  if (Math.Abs(listViewLogs.Height - y) < 19)
				//  {
				//    doScroll = true;
				//  }
				//}

				listViewLogs.Items.AddRange(lvis.ToArray());
				//			if (doScroll)
				if (checkBoxTrackNewest.Checked && listViewLogs.Items.Count > 0)
				{
					listViewLogs.EnsureVisible(listViewLogs.Items.Count - 1);
				}

				Text = "Node Inspector " + bytesReceived + "/" + framesReceived;
			}
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
					port.ReceivedBytesThreshold = 5;
					port.Open();
					port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
					Reset();
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

		#region Serial port stuff

		const byte ESC = 251;
		const byte EOF = 252;
		const byte SOF_STATE = 253;
		const byte SOF_MESSAGE = 254;
		const byte SOF_EVENT = 255;

		DateTime startTime = DateTime.Now;
		bool escReceived = false;
		List<byte> frameData = new List<byte>(200);
		DateTime frameTimeStamp;
		enum FrameType
		{
			Event,
			Message,
			State
		}
		FrameType frameType;
		bool portActive = false;
		bool timeSet = false;

		void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			if (timeSet == false)
			{
				startTime = DateTime.Now;
				timeSet = true;
			}

			if (portActive == false)
			{
				if (port != null)
				{
					port.DiscardInBuffer();
				}
				return;
			}

			while (port.BytesToRead > 0)
			{
				Interlocked.Increment(ref bytesReceived);
				byte v = (byte)port.ReadByte();

				switch (v)
				{
					case SOF_EVENT:
						frameTimeStamp = new DateTime(DateTime.Now.Ticks - startTime.Ticks);
						frameType = FrameType.Event;
						break;

					case SOF_MESSAGE:
						frameTimeStamp = DateTime.Now;
						frameType = FrameType.Message;
						break;

					case SOF_STATE:
						frameTimeStamp = DateTime.Now;
						frameType = FrameType.State;
						break;

					case ESC:
						escReceived = true;
						break;

					case EOF:
						try
						{
							switch (frameType)
							{
								case FrameType.Event:
									eventFrameQueue.Enqueue(new EventFrame(frameTimeStamp, frameData.ToArray()));
									Interlocked.Increment(ref framesReceived);
									break;
								case FrameType.Message:
									break;
								case FrameType.State:
									break;
							}
						}
						catch
						{
							Console.WriteLine("!");
						}
						frameData.Clear();
						break;

					default:
						if (escReceived)
						{
							escReceived = false;
							v |= 0x80;
						}
						frameData.Add(v);
						break;
				}
			}
		}

		#endregion

		private void buttonReset_Click(object sender, EventArgs e)
		{
			Reset();
		}

		void Reset()
		{
			portActive = false;

			port.DtrEnable = true;

			Thread.Sleep(500);

			port.DiscardInBuffer();
			EventFrame ef;
			while (eventFrameQueue.TryDequeue(out ef)) ;
			logs.Clear();
			LogEntry.ResetNumbering();
			foreach (var v in variables.Values)
			{
				v.Value = 0;
			}
			UpdateListView();

			port.DtrEnable = false;

			timeSet = false;

			portActive = true;

			Thread.Sleep(200);

			numericUpDownActiveNode_ValueChanged(null, null);
		}

		private void numericUpDownActiveNode_ValueChanged(object sender, EventArgs e)
		{
			//if (port != null && port.IsOpen)
			//{
			//  byte[] b = new byte[1];
			//  b[0] = Convert.ToByte(numericUpDownActiveNode.Value);
			//  port.Write(b, 0, 1);
			//}
		}
	}
}
