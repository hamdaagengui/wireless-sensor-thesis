using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using RfSuit;
using WebCamService;
using System.Threading.Tasks;

namespace RfSuitLogger
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();

      Task.Factory.StartNew(() => { capture = new Capture(0, 30, 640, 480); });
    }

    private Connection connection;
    private Capture capture;
    private IntPtr bitmap;

    private void startButton_Click(object sender, EventArgs e)
    {
      if (capture == null) {
        MessageBox.Show("Please wait for the webcam to start!");
        return;
      }

      capture.Start();

      // Setup RF
      connection = new Connection();
      connection.SweepStarted += connection_SweepStarted;
      connection.SweepCompleted += connection_SweepCompleted;
      connection.Start("COM30");
    }

    void connection_SweepCompleted(int channel, SweepResults[] results)
    {
 	    throw new NotImplementedException();
    }

    void connection_SweepStarted(int channel)
    {
      capture.GetBitMap(); // TODO: Fetch bitmap, and convert it... in a seperate thread!
 	    throw new NotImplementedException();
    }
  }
}
