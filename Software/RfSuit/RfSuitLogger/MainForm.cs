using System;
using System.Drawing;
using System.Windows.Forms;
using AForge.Video;
using AForge.Video.DirectShow;

namespace RfSuitLogger
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();

      //Task.Factory.StartNew(() => { capture = new Capture(0, 30, 640, 480); });
    }

//    private Connection connection;
    //private Capture capture;
    private IntPtr bitmap;

    private void startButton_Click(object sender, EventArgs e)
    {
      /*if (capture == null) {
        MessageBox.Show("Please wait for the webcam to start!");
        return;
      }*/

      //capture.Start();

      // Setup RF
      /*connection = new Connection();
      connection.SweepStarted += connection_SweepStarted;
      connection.SweepCompleted += connection_SweepCompleted;
      connection.Start("COM30"); */

      // enumerate video devices
      FilterInfoCollection videoDevices = new FilterInfoCollection(FilterCategory.VideoInputDevice);
      // create video source
      VideoCaptureDevice videoSource = new VideoCaptureDevice(videoDevices[0].MonikerString);
      // set NewFrame event handler
      videoSource.NewFrame += video_NewFrame;
      // start the video source
      videoSource.Start();
    }

    private void video_NewFrame(object sender,
        NewFrameEventArgs eventArgs)
    {
      // get new frame
      Bitmap bitmap = eventArgs.Frame;
      // process the frame
      previewPictureBox.Image = bitmap;
    }

/*
    void connection_SweepCompleted(int channel, SweepResults[] results)
    {
      throw new NotImplementedException();
    }

    void connection_SweepStarted(int channel)
    {
      //capture.GetBitMap(); // TODO: Fetch bitmap, and convert it... in a seperate thread!
      throw new NotImplementedException();
    }
*/
  }
}
