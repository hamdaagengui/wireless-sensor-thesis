using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using dk.iha;
using RfSuit;
using RfSuitLoggerInterfaces;
using ZedGraph;

namespace RfSuitLive
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();
    }

    private Connection _connection;
    private int _tickStart;

    private void MainForm_Load(object sender, EventArgs e)
    {
      comPortsComboBox.Items.AddRange(SerialPortInfo.GetSerialPorts());
      try
      {
        comPortsComboBox.SelectedIndex = 0;
      } catch(ArgumentOutOfRangeException) { }
      powerComboBox.SelectedIndex = 0;
      channelComboBox.SelectedIndex = 0;
      var pane = zedGraphControl.GraphPane;
      pane.Title.IsVisible = false;
      pane.XAxis.Title.IsVisible = false;
      pane.YAxis.Title.IsVisible = false;
      pane.Fill = new Fill(Color.Transparent);
      pane.Border.IsVisible = false;
      pane.Chart.Fill = new Fill(Color.Black);
    }

    private void ComPortsComboBoxDropDown(object sender, EventArgs e)
    {
      comPortsComboBox.Items.Clear();
      comPortsComboBox.Items.AddRange(SerialPortInfo.GetSerialPorts());
    }

    private void ConnectButtonClick(object sender, EventArgs e)
    {
      var info = comPortsComboBox.SelectedItem as SerialPortInfo;
      if(info == null)
        return;
      if(_connection != null)
        _connection.Stop();

      var channel = (byte)(channelComboBox.SelectedIndex + 11);
      var power = (byte)powerComboBox.SelectedIndex;
      _connection = new Connection();
      _connection.Start(info.Name);
      _connection.SetChannel(channel);
      _connection.SetTxPower(power);
      _connection.SweepCompleted += ConnectionSweepCompleted;
    }

    private RollingPointPairList[] _smoothing;

    void CreateSmoothing(LinkQualityIndicator[] lqis)
    {
      _smoothing = new RollingPointPairList[lqis.Length];
      for (int i = 0; i < lqis.Length; i++)
      {
        _smoothing[i] = new RollingPointPairList(SmoothingFactor);
        for (int j = 0; j < SmoothingFactor; j++)
          _smoothing[i].Add(0, lqis[i].Quality);
      }
    }

    private const int SmoothingFactor = 10;
    double[] GetSmoothing(LinkQualityIndicator[] lqis)
    {
      var output = new double[_smoothing.Length];
      for (int i = 0; i < output.Length; i++)
      {
        var smoothing = _smoothing[i];
        var lqi = lqis[i];
        smoothing.Add(0, lqi.Quality);
        if(Double.IsNaN(lqi.Quality))
        {
          output[i] = Double.NaN;
          continue;
        }
        double sum = 0;
        int divisor = 0;
        for (int j = 0; j < SmoothingFactor; j++)
        {
          double d = smoothing[j].Y;
          if (Double.IsNaN(d) == false)
          {
            sum += d;
            divisor++;
          }
        }
        output[i] = sum/divisor;
      }
      return output;
    }

    void InitZedGraph(LinkQualityIndicator[] lqis)
    {
      CreateSmoothing(lqis);

      var pane = zedGraphControl.GraphPane;
      zedGraphControl.IsAntiAlias = true;
      pane.CurveList.Clear();
      _tickStart = Environment.TickCount;

      const int interval = 1000 / 40; // 40fps

      var rotator = new ColorSymbolRotator();

      foreach (var lqi in lqis)
      {
        var list = new RollingPointPairList(30000 / interval); // 1 min buffer

        // Initially, a curve is added with no data points (list is empty)
        // Color is blue, and there will be no symbols
        pane.AddCurve(lqi.EndPointA + "<>" + lqi.EndPointB, list, rotator.NextColor, SymbolType.None);

        // Just manually control the X axis range so it scrolls continuously
        // instead of discrete step-sized jumps
        var xScale = pane.XAxis.Scale;
        xScale.Min = 0;
        xScale.Max = 20;
        xScale.MinorStep = 1;
        xScale.MajorStep = 5;
      }
    }


    static void CleanLQIs(IEnumerable<LinkQualityIndicator> lqis)
    {
      foreach (var lqi in lqis)
        lqi.Quality = lqi.Quality != 0.0 ? lqi.Quality : double.NaN;
    }

    void UpdateZedGraph(LinkQualityIndicator[] lqis)
    {
      var pane = zedGraphControl.GraphPane;
      if (pane == null)
        return;
      if(pane.CurveList.Count != lqis.Length)
        InitZedGraph(lqis);

      CleanLQIs(lqis);
      double[] smoothing = GetSmoothing(lqis);

      for (int i = 0; i < smoothing.Length; i++)
      {
        var smooth = smoothing[i];
        var curve = pane.CurveList[i] as LineItem;
        if (curve == null)
          return;
        // Get the PointPairList
        var list = curve.Points as IPointListEdit;
        // If this is null, it means the reference at curve.Points does not
        // support IPointListEdit, so we won't be able to modify it
        if (list == null)
          return;

        // Time is measured in seconds
        double time = (Environment.TickCount - _tickStart) / 1000.0;

        // 3 seconds per cycle
        list.Add(time, smooth);

        // Keep the X scale at a rolling 30 second interval, with one
        // major step between the max X value and the end of the axis
        var xScale = pane.XAxis.Scale;
        if (time > xScale.Max)
        {
          xScale.Max = time;
          xScale.Min = xScale.Max - 20.0;
        }
      }
    }

    void ConnectionSweepCompleted(LinkQualityIndicator[] linkQualityIndicators)
    {
      this.BeginInvokeIfRequired(mainForm => mainForm.UpdateZedGraph(linkQualityIndicators));
    }

    private void TimerTick(object sender, EventArgs e)
    {
      // Make sure the Y axis is rescaled to accommodate actual data
      zedGraphControl.AxisChange();
      // Force a redraw
      zedGraphControl.Invalidate();
    }
  }
}
