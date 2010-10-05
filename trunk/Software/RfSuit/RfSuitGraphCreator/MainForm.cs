using System;
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
      var graphData = OpenLogFile("fil.rflogz");
      double[] timeline = graphData.Timeline;
      foreach (var connectionData in graphData.ConnectionDatas)
      {
        connectionData.ToString(); // fx. "1 <> 2"
        double[] linkQualities = connectionData.GetQuality(7); // Smoothing
      }
    }

    private GraphData OpenLogFile(string path)
    {
      var entries = EntrySerializer.Instance.OpenRead(path);
      return new GraphData(entries.ToArray());
    }

    private void CreateGraph(double[] x, double[] y, bool ascending = false, string label = "")
    {
      var zgc = zedGraphControl1;
      var myPane = zgc.GraphPane;
      Array.Sort(y);
      if(!ascending)
        Array.Reverse(y);
      myPane.CurveList.Add(new LineItem(label, x, y, Color.Black, SymbolType.Default)
                           {
                             
                           });
      zgc.Invalidate();
    }
  }
}
