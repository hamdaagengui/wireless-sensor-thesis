using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using RfSuit;
using dk.iha;

namespace RfSuitConnectionTester
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			Connection con = new Connection();
			con.SweepCompleted += new SweepCompletedDelegate(con_SweepCompleted);
			con.Start("COM15");
		}

		Int64 counter = 0;
		DateTime time = DateTime.Now;
		void con_SweepCompleted(LinkQualityIndicator[] results)
		{
			if (InvokeRequired)
			{
				BeginInvoke(new SweepCompletedDelegate(con_SweepCompleted), new object[] { results });
				return;
			}

			counter++;
			DateTime now = DateTime.Now;
			Text = string.Format("{0:0.0} sweeps/second {1} sweeps", 1.0 / (now - time).TotalSeconds, counter.ToString());
			time = now;

			if (results.Length >= 1)
			{
				progressBar1.Value = (int)(results[0].Quality + 93);
				textBox1.Text = string.Format("{0:0.0} dBm", results[0].Quality);
			}
			if (results.Length >= 3)
			{
				progressBar2.Value = (int)(results[1].Quality + 93);
				textBox2.Text = string.Format("{0:0.0} dBm", results[1].Quality);
				progressBar3.Value = (int)(results[2].Quality + 93);
				textBox3.Text = string.Format("{0:0.0} dBm", results[2].Quality);
			}
			if (results.Length >= 6)
			{
				progressBar4.Value = (int)(results[3].Quality + 93);
				textBox4.Text = string.Format("{0:0.0} dBm", results[3].Quality);
				progressBar5.Value = (int)(results[4].Quality + 93);
				textBox5.Text = string.Format("{0:0.0} dBm", results[4].Quality);
				progressBar6.Value = (int)(results[5].Quality + 93);
				textBox6.Text = string.Format("{0:0.0} dBm", results[5].Quality);
			}
		}
	}
}
