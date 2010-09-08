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

		void con_SweepCompleted(LinkQualityIndicator[] results)
		{
			if (InvokeRequired)
			{
				BeginInvoke(new SweepCompletedDelegate(con_SweepCompleted), new object[] { results });
				return;
			}

		//	Text = results[0].Quality.ToString() + " dBm";
		}
	}
}
