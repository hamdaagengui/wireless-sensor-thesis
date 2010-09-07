using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using AForge.Video;
using AForge.Video.DirectShow;
using System.IO;
using RfSuit;
using RfSuitLoggerInterfaces;

namespace RfSuitLogger
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();

      RefreshButtonClick(this, EventArgs.Empty);
      _visualSource = new VisualSource(1);
      _logger = new Logger(_visualSource, new ConnectionSimulator(10));
    }

    //    private Connection connection;
    private FilterInfoCollection _videoDevices;
    private VideoCaptureDevice _videoSource;
    private readonly VisualSource _visualSource;
    private readonly Logger _logger;

    private void VideoNewFrame(object sender,
        NewFrameEventArgs eventArgs)
    {
      // get new frame
      var bitmap = (Bitmap)eventArgs.Frame.Clone();
      previewPictureBox.BeginInvokeIfRequired(p =>
      {
        try
        {
          if (p.Created) {
            _visualSource.SetVisual(0, (Bitmap)bitmap.Clone());
            p.Image = bitmap;
          }
        }
        catch (Exception ex)
        {
          DisplayError(ex);
        }
      });
    }

    private void RefreshButtonClick(object sender, EventArgs e)
    {
      _videoDevices = new FilterInfoCollection(FilterCategory.VideoInputDevice);
      captureDevicesComboBox.Items.Clear();
      videoCapabilitiesComboBox.Items.Clear();
      foreach (var fi in _videoDevices.OfType<FilterInfo>()) {
        captureDevicesComboBox.Items.Add(new DisplayableFilterInfo { FilterInfo = fi });
      }
    }

    static void DisplayError(Exception ex, string msg = null)
    {
      MessageBox.Show(msg ?? ex.Message, ex.GetType().Name, MessageBoxButtons.OK, MessageBoxIcon.Error);
    }

    private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
    {
      _logger.Stop();
      StopVideoSource();
    }

    private void CaptureDevicesComboBoxSelectedIndexChanged(object sender, EventArgs e)
    {
      var dfi = captureDevicesComboBox.SelectedItem as DisplayableFilterInfo;
      if (dfi == null)
        return;
      var videoSource = new VideoCaptureDevice(dfi.FilterInfo.MonikerString);
      if (videoSource.VideoCapabilities == null) {
        videoCapabilitiesComboBox.Items.Clear();
        videoCapabilitiesComboBox.Items.Add("N/A");
        videoCapabilitiesComboBox.SelectedIndex = 0;
        _videoSource = null;
        return;
      }
      StopVideoSource();
      _videoSource = videoSource;
      videoCapabilitiesComboBox.Items.Clear();
      videoCapabilitiesComboBox.Items.AddRange(
        Array.ConvertAll(_videoSource.VideoCapabilities, vc => new DisplayableVideoCapabilities { VideoCapabilities = vc }));
      videoCapabilitiesComboBox.SelectedIndex = 0;
    }

    private void StopVideoSource()
    {
      if (_videoSource == null) return;
      previewPictureBox.Visible = false;
      Application.DoEvents();
      _videoSource.Stop();
      _videoSource.NewFrame -= VideoNewFrame;
    }

    private void StartVideoSource(VideoCapabilities vc) {
      if (_videoSource == null) return;
      _videoSource.DesiredFrameRate = vc.MaxFrameRate;
      _videoSource.DesiredFrameSize = vc.FrameSize;
      _videoSource.NewFrame += VideoNewFrame;
      _videoSource.Start();
      previewPictureBox.Visible = true;
    }

    private void VideoCapabilitiesComboBoxSelectedIndexChanged(object sender, EventArgs e)
    {
      var dvc = videoCapabilitiesComboBox.SelectedItem as DisplayableVideoCapabilities;
      if (dvc == null) return;
      StopVideoSource();
      StartVideoSource(dvc.VideoCapabilities);
    }

    private void PropertiesButtonClick(object sender, EventArgs e)
    {
      if(_videoSource != null)
      _videoSource.DisplayPropertyPage(Handle);
    }

    private void StartLogButtonClick(object sender, EventArgs e)
    {
      var serialPortInfo = serialPortsComboBox.SelectedItem as SerialPortInfo;
      if (serialPortInfo == null) {
        DisplayError("Please select a serial port!", "Missing Serial Port");
        return;
      }
      _logger.Start(new FileStream(Directory.GetParent(Application.ExecutablePath) + @"\log" + (long)Utils.MillisecondsSinceEpoch() + ".rflog", FileMode.CreateNew), serialPortInfo.Name);
    }

    private static void DisplayError(string title, string msg)
    {
      MessageBox.Show(msg, title, MessageBoxButtons.OK, MessageBoxIcon.Error);
    }

    private void StopLogButtonClick(object sender, EventArgs e)
    {
      _logger.Stop();
    }

    private void SerialPortsComboBoxDropDown(object sender, EventArgs e)
    {
      serialPortsComboBox.Items.Clear();
      serialPortsComboBox.Items.AddRange(SerialPortInfo.GetSerialPorts());
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
