using System;
using System.Drawing;
using System.Windows.Forms;
using AForge.Video;
using AForge.Video.DirectShow;
using System.IO;
using System.Drawing.Imaging;

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
    private VideoCaptureDevice videoSource;

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
      if (captureDevicesComboBox.SelectedItem is DisplayableFilterInfo == false)
      {
        MessageBox.Show("Please select a webcam to start!");
        return;
      }
      var fi = (DisplayableFilterInfo)captureDevicesComboBox.SelectedItem;
      videoSource = new VideoCaptureDevice(fi.FilterInfo.MonikerString);
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
      var bitmap = (Bitmap)eventArgs.Frame.Clone();
      previewPictureBox.BeginInvokeIfRequired(p =>
                                           {
                                             try
                                             {
                                               if (p.Created) {
                                                 bitmap.Save(Stream.Null, ImageFormat.Jpeg);
                                                 p.Image = bitmap;
                                               }
                                             }
                                             catch (Exception ex)
                                             {
                                               DisplayError(ex);
                                             }
                                           });
    }

    private void refreshButton_Click(object sender, EventArgs e)
    {
      videoDevices = new FilterInfoCollection(FilterCategory.VideoInputDevice);
      captureDevicesComboBox.Items.Clear();
      foreach (var videoDevice in videoDevices)
      {
        var fi = videoDevice as FilterInfo;
        if (fi != null)
          captureDevicesComboBox.Items.Add(new DisplayableFilterInfo { FilterInfo = fi });
      }
    }

//        void connection_SweepCompleted(int channel, SweepResults[] results)
//        {
//          throw new NotImplementedException();
//        }
//
//        void connection_SweepStarted(int channel)
//        {
          //capture.GetBitMap(); // TODO: Fetch bitmap, and convert it... in a seperate thread!
//          throw new NotImplementedException();
//        }
    static void DisplayError(Exception ex, string msg = null)
    {
      MessageBox.Show(msg ?? ex.Message, ex.GetType().Name, MessageBoxButtons.OK, MessageBoxIcon.Error);
    }

    private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
    {
      stopVideoSource();
    }

    private void captureDevicesComboBox_SelectedIndexChanged(object sender, EventArgs e)
    {
      var dfi = captureDevicesComboBox.SelectedItem as DisplayableFilterInfo;
      if (dfi == null)
        return;
      stopVideoSource();
      videoSource = new VideoCaptureDevice(dfi.FilterInfo.MonikerString);
      videoCapabilitiesComboBox.Items.Clear();
      videoCapabilitiesComboBox.Items.AddRange(
        Array.ConvertAll(videoSource.VideoCapabilities, vc => new DisplayableVideoCapabilities { VideoCapabilities = vc }));
    }

    private void stopVideoSource() {
      if (videoSource == null) return;
      previewPictureBox.Visible = false;
      Application.DoEvents();
      videoSource.Stop();
      videoSource.NewFrame -= video_NewFrame;
    }

    private void startVideoSource(VideoCapabilities vc) {
      videoSource.DesiredFrameRate = vc.MaxFrameRate;
      videoSource.DesiredFrameSize = vc.FrameSize;
      videoSource.NewFrame += video_NewFrame;
      videoSource.Start();
      previewPictureBox.Visible = true;
    }

    private void videoCapabilitiesComboBox_SelectedIndexChanged(object sender, EventArgs e)
    {
      var dvc = videoCapabilitiesComboBox.SelectedItem as DisplayableVideoCapabilities;
      if (dvc == null) return;
      stopVideoSource();
      startVideoSource(dvc.VideoCapabilities);
    }

    private void propertiesButton_Click(object sender, EventArgs e)
    {
      videoSource.DisplayPropertyPage(Handle);
    }
  }

  class DisplayableFilterInfo
  {
    public FilterInfo FilterInfo { get; set; }
    public override string ToString()
    {
      return (FilterInfo != null ? FilterInfo.Name : "NULL");
    }
  }
  class DisplayableVideoCapabilities {
    public VideoCapabilities VideoCapabilities { get; set; }
    public override string ToString()
    {
      return String.Format("{0}x{1} @ {2}", VideoCapabilities.FrameSize.Width, VideoCapabilities.FrameSize.Height, VideoCapabilities.MaxFrameRate);
    }
  }

}
