using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using RfSuitLoggerInterfaces;
using RfSuitPlayer;
using ZedGraph;

namespace RfSuitGraphCreator
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();
    }

    private void MainForm_Load(object sender, EventArgs e)
    {
      CreateGraph(CreateCumulativeDistribution(new double[] {1, 2, 3, 4, 5}), "Graph 1");
/*
      var graphData = OpenLogFile("fil.rflogz");
      double[] timeline = graphData.Timeline;
      foreach (var connectionData in graphData.ConnectionDatas)
      {
        connectionData.ToString(); // fx. "1 <> 2"
        double[] linkQualities = connectionData.GetQuality(7); // Smoothing
      }
*/
    }

    private GraphData OpenLogFile(string path)
    {
      var entries = EntrySerializer.Instance.OpenRead(path);
      return new GraphData(entries.ToArray());
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <returns></returns>
    private static Tuple<double[], double[]> CreateCumulativeDistribution(double[] x)
    {
      var length = x.Length;
      Array.Sort(x);
      var dict = new Dictionary<double, int>();
      foreach (var d in x)
      {
        if(dict.ContainsKey(d))
          dict[d]++;
        else
          dict.Add(d, 1);
      }
      var tuple = Tuple.Create(new double[dict.Count], new double[dict.Count]);
      int i = 0;
      int value = 0; 
      foreach (var d in dict)
      {
        tuple.Item1[i] = d.Key;

        value += d.Value;
        tuple.Item2[i] = (double)value/length;
        i++;
      }
      return tuple;
    }

    private void CreateGraph(Tuple<double[], double[]> tuple, string title, string xtitle = "", string ytitle = "Quantiles")
    {
      var zgc = zedGraphControl1;
      var myPane = zgc.GraphPane;

      var x = tuple.Item1;
      var y = tuple.Item2;
      
      // Set the title and axis labels
      myPane.Title.Text = title;
      myPane.Title.IsVisible = false;
      myPane.YAxis.Title.Text = ytitle;
      myPane.Legend.IsVisible = false;
      myPane.YAxis.Title.IsVisible = true;
      myPane.XAxis.Title.IsVisible = true;
      myPane.XAxis.Title.Text = xtitle;

      myPane.Fill = new Fill(Color.Transparent);
      myPane.Border.IsVisible = false;

      myPane.Chart.Fill = new Fill(Color.White);

      myPane.CurveList.Add(new LineItem("", x, y, Color.Black, SymbolType.Default));
      
      zgc.AxisChange();
      zgc.Invalidate();
    }
  }
}
