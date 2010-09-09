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
		void con_SweepCompleted(LinkQualityIndicator[] results)
		{
			if (InvokeRequired)
			{
				BeginInvoke(new SweepCompletedDelegate(con_SweepCompleted), new object[] { results });
				return;
			}

			if (results.Length == 1)
			{
				progressBar1.Value = (int)(results[0].Quality + 90);
				textBox1.Text = results[0].Quality + " dBm";
			}
			if (results.Length == 3)
			{
				progressBar1.Value = (int)(results[0].Quality + 90);
				textBox1.Text = results[0].Quality + " dBm";
				progressBar2.Value = (int)(results[1].Quality + 90);
				textBox2.Text = results[1].Quality + " dBm";
				progressBar3.Value = (int)(results[2].Quality + 90);
				textBox3.Text = results[2].Quality + " dBm";
			}

			counter++;
			Text = counter.ToString();
		}
	}
}
