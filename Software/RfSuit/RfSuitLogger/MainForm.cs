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

      refreshButton_Click(this, EventArgs.Empty);
      //Task.Factory.StartNew(() => { capture = new Capture(0, 30, 640, 480); });
    }

//    private Connection connection;
    //private Capture capture;
    private IntPtr bitmap;
    private FilterInfoCollection videoDevices;

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
      
      // create video source
      if(captureDevicesComboBox.SelectedItem is DisplayableFilterInfo == false)
      {
        MessageBox.Show("Please select a webcam to start!");
        return;
      }
      DisplayableFilterInfo fi = (DisplayableFilterInfo) captureDevicesComboBox.SelectedItem;
      VideoCaptureDevice videoSource = new VideoCaptureDevice(fi.FilterInfo.MonikerString);
      // set NewFrame event handler
      videoSource.NewFrame += video_NewFrame;
      videoSource.DesiredFrameRate = 15;
      videoSource.DesiredFrameSize = previewPictureBox.Size;
      // start the video source
      videoSource.Start();
    }

    private void video_NewFrame(object sender,
        NewFrameEventArgs eventArgs)
    {
      // get new frame
      Bitmap bitmap = eventArgs.Frame;
      previewPictureBox.InvokeIfRequired(p =>
                                           {
                                             p.Image = bitmap;
                                           });
    }

    private void refreshButton_Click(object sender, EventArgs e)
    {
      videoDevices = new FilterInfoCollection(FilterCategory.VideoInputDevice);
      captureDevicesComboBox.Items.Clear();
      foreach (var videoDevice in videoDevices)
      {
        FilterInfo fi = videoDevice as FilterInfo;
        if(fi != null)
          captureDevicesComboBox.Items.Add(new DisplayableFilterInfo() { FilterInfo = fi });
      }
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

  class DisplayableFilterInfo
  {
    public FilterInfo FilterInfo { get; set; }
    public override string ToString()
    {
      return (FilterInfo != null ? FilterInfo.Name : "NULL");
    }
  }

}
