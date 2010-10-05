using System;
using System.Collections.Generic;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using dk.iha;
using System.Drawing;
using WinFormPlusPlus;
using ZedGraph;
using RfSuitPlayer.Properties;
using RfSuitLoggerInterfaces;
using System.Threading.Tasks;

namespace RfSuitPlayer
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();
      smoothingToolStripComboBox.SelectedIndex = 0;
    }

    private Entry[] _entries;
    private double[] _timeline;
    private GraphData _graphData;
    private bool _enableCurveUpdates = true;
    private int _position;
    private byte[] _activePicture = null;

    private void OpenToolStripMenuItemClick(object sender, EventArgs e)
    {
      if (openFileDialog.ShowDialog(this) != DialogResult.OK) return;
      var entries = EntrySerializer.Instance.OpenRead(openFileDialog.FileName);
      Text = Path.GetFileName(openFileDialog.FileName);
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
      var parents = new Dictionary<int, CheckBoxParent>();
      var parentAll = new CheckBoxParent {Text = "All", ForeColor = Color.White};
      foreach (var connectionData in _graphData.ConnectionDatas)
      {
        var checkBox = new CheckBox {
          Tag = connectionData,
          Text = connectionData.ToString(),
          ForeColor = connectionData.Color
        };
        parentAll.AddChildCheckBox(checkBox);
        checkBox.CheckedChanged += CheckBoxClick;
        flowLayoutPanel1.Controls.Add(checkBox);
        foreach (var endPoint in connectionData.EndPoints)
        {
          CheckBoxParent parent;
          if (!parents.TryGetValue(endPoint, out parent))
          {
            parent = new CheckBoxParent {Text = "EP " + endPoint, ForeColor = Color.White };
            parents.Add(endPoint, parent);
          }
          parent.AddChildCheckBox(checkBox);
        }
      }

      foreach (var checkBoxParent in parents)
      {
        flowLayoutPanel1.Controls.Add(checkBoxParent.Value);
      }

      flowLayoutPanel1.Controls.Add(parentAll);

      parentAll.Checked = true;

      CreateChart();
    }

    private bool _updateAwaits = false;
    void CheckBoxClick(object sender, EventArgs e)
    {
      _updateAwaits = true;
    }

    private int updates = 0;

    public void UpdateCurves() {
      if (_enableCurveUpdates == false) return;
      Console.WriteLine("Update: " + updates++);
      var myPane = zedGraphControl1.GraphPane;
      myPane.CurveList.Clear();

      var filteredConnections = from cb in flowLayoutPanel1.Controls.OfType<CheckBox>()
                                where cb.Checked && cb.Tag is ConnectionData
                                select cb.Tag as ConnectionData;
      int smoothingFactor = GetSmoothingFactor();
      Parallel.ForEach(filteredConnections, connectionData => {
        var data = connectionData.GetQuality(smoothingFactor);
        var curve = myPane.AddCurve(connectionData.ToString(), _timeline, data, connectionData.Color);
        curve.Symbol.IsVisible = false;
      });

      // Calculate the Axis Scale Ranges
      zedGraphControl1.AxisChange();
      myPane.XAxis.Title.Text = "Timestamp [" + myPane.XAxis.Scale.Format + "]";
      zedGraphControl1.Invalidate();
    }

    private int GetSmoothingFactor() {
      return smoothingToolStripComboBox.SelectedIndex * 2 + 1;
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
      var mousePt = new PointF(e.X, e.Y);
      double x;
      double y;
      sender.GraphPane.ReverseTransform(mousePt, out x, out y);
      statusToolStripStatusLabel.Text = String.Format("Point: {0}, {1}.", x, y);
      if ((ModifierKeys & Keys.Alt) != 0)
      {
        if (e.Button == MouseButtons.Left)
        {
          // Vertical bar
          _position = _graphData.GetNearestIndex(x);
          UpdateGraphLine();
        }
        else
        {
          // Horizontal bar
          CurveItem curve;
          int nearest;
          if (sender.GraphPane.FindNearestPoint(mousePt, out curve, out nearest) == false) return default(bool);
          statusToolStripStatusLabel.Text += String.Format(" Nearest curve: {0}.", curve.Label.Text);
        }
      }
      return default(bool);
    }

    private void SmoothingToolStripComboBoxSelectedIndexChanged(object sender, EventArgs e)
    {
      if(smoothingToolStripComboBox.SelectedIndex >= 0) {
        UpdateCurves();
      }
    }

    private void TimerTick(object sender, EventArgs e)
    {
      if (!_updateAwaits) return;
      UpdateCurves();
      _updateAwaits = false;
    }

    private void toolStripStatusLabel1_Click(object sender, EventArgs e)
    {

    }
  }
}
