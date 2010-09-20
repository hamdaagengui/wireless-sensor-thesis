using System;
using System.Collections.Generic;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using dk.iha;
using System.Drawing;
using ZedGraph;
using RfSuitPlayer.Properties;
using RfSuitLoggerInterfaces;

namespace RfSuitPlayer
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();
    }

    private Entry[] _entries;
    private double[] _timeline;
    private GraphData _graphData;
    private bool _enableCurveUpdates = true;
    private CheckBox _allCheckBox;
    private int _position;
    private byte[] _activePicture = null;

    private void OpenToolStripMenuItemClick(object sender, EventArgs e)
    {
      if (openFileDialog.ShowDialog(this) != DialogResult.OK) return;
      var entries = EntrySerializer.Instance.OpenRead(openFileDialog.FileName);
      _entries = entries.ToArray();

      var tmp = new List<Picture>();
      foreach (var entry in _entries)
      {
        if(entry.pictures.Count > 0) {
          tmp = entry.pictures;
        } else {
          entry.pictures.AddRange(tmp);
        }
      }
      _position = 0;
      _graphData = new GraphData(_entries);
      
      flowLayoutPanel1.Controls.Clear();
      foreach (var connectionData in _graphData.ConnectionDatas)
      {
        var checkBox = new CheckBox {
          Tag = connectionData,
          Text = connectionData.ToString(),
          Checked = true,
          ForeColor = connectionData.Color
        };
        checkBox.CheckedChanged += CheckBoxClick;
        flowLayoutPanel1.Controls.Add(checkBox);
      }
      _allCheckBox = new CheckBox
      {
        Text = "All",
        Checked = true,
        ForeColor = Color.White,
      };
      _allCheckBox.CheckedChanged += CheckAllCheckedChanged;
      flowLayoutPanel1.Controls.Add(_allCheckBox);

      CreateChart();
    }

    void CheckAllCheckedChanged(object sender, EventArgs e)
    {
      if (_enableCurveUpdates == false) return;
      var checkAllCheckBox = sender as CheckBox;
      if (checkAllCheckBox == null) return;
      _enableCurveUpdates = false;
      foreach (var checkBox in flowLayoutPanel1.Controls.OfType<CheckBox>())
        checkBox.Checked = checkAllCheckBox.Checked;
      _enableCurveUpdates = true;
      UpdateCurves();
    }

    void CheckBoxClick(object sender, EventArgs e)
    {
      UpdateCurves();

      if(_enableCurveUpdates) {
        _enableCurveUpdates = false;
        var cbs = from cb in flowLayoutPanel1.Controls.OfType<CheckBox>()
                where cb.Tag is ConnectionData
                select cb;
        bool isAllSelected = true;
        bool isNoneSelected = true;
        foreach (var checkBox in cbs) {
          if (checkBox.Checked) {
            isNoneSelected = false;
          } else {
            isAllSelected = false;
          }
        }
        if (isAllSelected)
          _allCheckBox.Checked = true;
        else if (isNoneSelected)
          _allCheckBox.Checked = false;
        else
          _allCheckBox.CheckState = CheckState.Indeterminate;
        _enableCurveUpdates = true;
      }
    }

    public void UpdateCurves() {
      if (_enableCurveUpdates == false) return;
      var myPane = zedGraphControl1.GraphPane;
      myPane.CurveList.Clear();

      var filteredConnections = from cb in flowLayoutPanel1.Controls.OfType<CheckBox>()
                                where cb.Checked && cb.Tag is ConnectionData
                                select cb.Tag as ConnectionData;
      foreach (var connectionData in filteredConnections)
      {
        var curve = myPane.AddCurve(connectionData.ToString(), _timeline, connectionData.Quality, connectionData.Color);
        curve.Symbol.IsVisible = false;
      }

      // Calculate the Axis Scale Ranges
      zedGraphControl1.AxisChange();
      myPane.XAxis.Title.Text = "Timestamp [" + myPane.XAxis.Scale.Format + "]";
      zedGraphControl1.Invalidate();
    }

    public void CreateChart()
    {
      var myPane = zedGraphControl1.GraphPane;

      // Set the title and axis labels
      myPane.Title.Text = "RfData";
      myPane.Title.IsVisible = false;
      myPane.XAxis.Type = AxisType.Date;
      myPane.YAxis.Title.Text = "Link Quality [dBm]";
      myPane.Legend.IsVisible = false;
      myPane.YAxis.Title.IsVisible = showLegendToolStripMenuItem.Checked;
      myPane.XAxis.Title.IsVisible = showLegendToolStripMenuItem.Checked;


      myPane.Fill = new Fill(Color.Transparent);
      myPane.Border.IsVisible = false;

      myPane.Chart.Fill = new Fill(Color.Black);

      _timeline = _graphData.Timeline;

      myPane.XAxis.Scale.Min = _timeline.First();
      myPane.XAxis.Scale.Max = _timeline.Last();

      UpdateGraphLine();

      UpdateCurves();
    }

    private void UpdateGraphLine() {

      if (_activePicture != _entries[_position].pictures[0].data) {
        _activePicture = _entries[_position].pictures[0].data;
        pictureBox.Image = new Bitmap(new MemoryStream(_activePicture));
      }

      var zgc = zedGraphControl1;
      var myPane = zgc.GraphPane;
      var timestamp = _timeline[_position];
      myPane.GraphObjList.RemoveAll(c => c is LineObj);
      var line = new LineObj(Color.White, timestamp, 0, timestamp, 1) {
        Location = {CoordinateFrame = CoordType.XScaleYChartFraction},
        ZOrder = ZOrder.E_BehindCurves,
        IsClippedToChartRect = true,
        Line = {Style = DashStyle.Dash, Width = 2}
      };
      myPane.GraphObjList.Add(line);
      zgc.Invalidate();
    }

    private void ExitToolStripMenuItemClick(object sender, EventArgs e)
    {
      Close();
    }

/*    private void TrackBarValueChanged(object sender, EventArgs e)
    {
      if (_entries == null) return;
      var entry = _entries[trackBar.Value];
      pictureBox.Image = Image.FromStream(new MemoryStream(entry.pictures[0].data));
      UpdateGraphLine();
    }

    private void PreviousButtonClick(object sender, EventArgs e)
    {
      if (_player != null)
        _player.StopPlayer();
      if (trackBar.Value > trackBar.Minimum)
        --trackBar.Value;
    }

    private void NextButtonClick(object sender, EventArgs e)
    {
      if (_player != null)
        _player.StopPlayer();
      if (trackBar.Value < trackBar.Maximum)
        ++trackBar.Value;
    }

    private void PlayButtonClick(object sender, EventArgs e)
    {
      if (_player != null)
        _player.TogglePlayer();
    }

    private void TrackBarScroll(object sender, EventArgs e)
    {
      if (_player != null)
        _player.StopPlayer();
    }*/

    private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
    {
      Settings.Default.Save();
    }

    private void ShowLegendToolStripMenuItemClick(object sender, EventArgs e)
    {
      showLegendToolStripMenuItem.Checked = !showLegendToolStripMenuItem.Checked;
      CreateChart();
    }

    private bool ZedGraphControlMouseEvent(ZedGraphControl sender, MouseEventArgs e)
    {
      if((ModifierKeys & Keys.Alt) != 0 && e.Button == MouseButtons.Left) {
        var mousePt = new PointF(e.X, e.Y);
        double x;
        double y;
        sender.GraphPane.ReverseTransform(mousePt, out x, out y);
        _position = _graphData.GetNearestIndex(x);
        UpdateGraphLine();
      }
      return default(bool);
    }
  }
}
