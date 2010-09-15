using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing.Drawing2D;
using System.IO;
using System.Windows.Forms;
using dk.iha;
using ProtoBuf;
using System.Drawing;
using System.Threading;
using ZedGraph;

namespace RfSuitPlayer
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();
    }

    private Entry[] _entries;
    private Player _player;
    private double[] _timeline;

    // ZedGraph stuff


    private void OpenToolStripMenuItemClick(object sender, EventArgs e)
    {
      if (openFileDialog.ShowDialog(this) != DialogResult.OK) return;
      var fileStream = File.OpenRead(openFileDialog.FileName);

      var entries = new List<Entry>(1000);
      while (fileStream.Position < fileStream.Length)
      {
        var entry = Serializer.DeserializeWithLengthPrefix<Entry>(fileStream, PrefixStyle.Base128);
        entries.Add(entry);
      }
      _entries = entries.ToArray();
      trackBar.Minimum = 0;
      trackBar.Maximum = _entries.Length - 1;
      
      CreateChart(zedGraphControl1, new GraphData(_entries));

      trackBar.Value = 0;
      if (_player != null)
        _player.StopPlayer();
      _player = new Player(_entries, trackBar);
    }

    public void CreateChart(ZedGraphControl zgc, GraphData graphData)
    {
      var myPane = zgc.GraphPane;

      // Set the title and axis labels
      myPane.Title.Text = "RfData";
      myPane.XAxis.Type = AxisType.Date;
      myPane.YAxis.Title.Text = "Link Quality [dBm]";

      _timeline = graphData.Timeline;

      var rotator = ColorSymbolRotator.StaticInstance;
      foreach (var connectionData in graphData.ConnectionDatas)
      {
        var curve = myPane.AddCurve(connectionData.EndPointA + " <> " + connectionData.EndPointB, _timeline, connectionData.Quality, rotator.NextColor);
        curve.Symbol.IsVisible = false;
        //curve.Symbol.Fill = new Fill(Color.White);
      }
      
      // Fill the axis background with a color gradient
      myPane.Chart.Fill = new Fill(Color.FromArgb(255, 255, 245), Color.FromArgb(255, 255, 190), 90F);

      // Display the Y axis grid lines
//      myPane.YAxis.MajorGrid.IsVisible = true;
//      myPane.YAxis.MinorGrid.IsVisible = true;
      #region bah
/*
      // Draw a box item to highlight a value range
      BoxObj box = new BoxObj(0, 100, 1, 30, Color.Empty,
              Color.FromArgb(150, Color.LightGreen));
      box.Fill = new Fill(Color.White, Color.FromArgb(200, Color.LightGreen), 45.0F);
      // Use the BehindAxis zorder to draw the highlight beneath the grid lines
      box.ZOrder = ZOrder.E_BehindCurves;
      // Make sure that the boxObj does not extend outside the chart rect if the chart is zoomed
      box.IsClippedToChartRect = true;
      // Use a hybrid coordinate system so the X axis always covers the full x range
      // from chart fraction 0.0 to 1.0
      box.Location.CoordinateFrame = CoordType.XChartFractionYScale;
      myPane.GraphObjList.Add(box);

      // Add a text item to label the highlighted range
      TextObj text = new TextObj("Optimal\nRange", 0.95f, 85, CoordType.AxisXYScale,
                              AlignH.Right, AlignV.Center);
      text.FontSpec.Fill.IsVisible = false;
      text.FontSpec.Border.IsVisible = false;
      text.FontSpec.IsBold = true;
      text.FontSpec.IsItalic = true;
      text.Location.CoordinateFrame = CoordType.XChartFractionYScale;
      text.IsClippedToChartRect = true;
      myPane.GraphObjList.Add(text);
*/
      #endregion

      zgc.IsEnableZoom = false;

      // Fill the pane background with a gradient
      myPane.Fill = new Fill(Color.White, Color.WhiteSmoke, 0F);

      UpdateGraphLine();

      // Calculate the Axis Scale Ranges
      zgc.AxisChange();

      myPane.XAxis.Title.Text = "Timestamp [" + myPane.XAxis.Scale.Format + "]";
      
      zgc.Invalidate();
    }

    private void UpdateGraphLine() {
      var zgc = zedGraphControl1;
      var myPane = zgc.GraphPane;
      var timestamp = _timeline[trackBar.Value];
      myPane.GraphObjList.RemoveAll(c => c is LineObj);
      var line = new LineObj(Color.DimGray, timestamp, 0, timestamp, 1)
      {
        Location = { CoordinateFrame = CoordType.XScaleYChartFraction },
        ZOrder = ZOrder.E_BehindCurves,
        IsClippedToChartRect = true,
        Line = { Style = DashStyle.Dash }
      };
      myPane.GraphObjList.Add(line);      
      zgc.Invalidate();
    }

    private void ExitToolStripMenuItemClick(object sender, EventArgs e)
    {
      Close();
    }

    private void TrackBarValueChanged(object sender, EventArgs e)
    {
      if (_entries == null) return;
      var entry = _entries[trackBar.Value];
      pictureBox.Image = Image.FromStream(new MemoryStream(entry.pictures[0].data));
      propertyGrid.SelectedObjects = entry.results.ToArray();
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
    }
  }
}
