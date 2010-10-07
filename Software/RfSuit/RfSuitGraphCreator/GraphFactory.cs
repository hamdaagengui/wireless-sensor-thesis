using System.Collections.Generic;
using System.Drawing;
using ZedGraph;

namespace RfSuitGraphCreator
{
  class GraphFactory
  {
    public double[] XAxis { get; set; }
    public double[] YAxis { get; set; }
    public string Title { get; set; }
    public string XTitle { get; set; }
    public string YTitle { get; set; }
    public string Filename { get; set; }
    public bool Reverse { get; set; }
    public List<BoxData> Boxes { get; private set; }
    public List<TextObj> TextObjs { get; private set; }
    public bool DroppedPackages { get; set; }

    public GraphFactory()
    {
      Boxes = new List<BoxData>();
      TextObjs = new List<TextObj>();
    }

    public GraphPane CreateGraph()
    {
      var myPane = new GraphPane(new RectangleF(0, 0, 1024, 768), Title, XTitle, YTitle);
      // Set the title and axis labels
      myPane.Title.IsVisible = false;
      myPane.Legend.IsVisible = false;
      myPane.YAxis.Title.IsVisible = true;
      myPane.XAxis.Title.IsVisible = true;
      myPane.XAxis.Scale.IsReverse = Reverse;
      myPane.XAxis.Scale.Min = -90;
      myPane.XAxis.Scale.Max = -40;
      myPane.YAxis.Scale.Min = 0;
      myPane.YAxis.Scale.Max = 1;
      myPane.YAxis.Scale.MajorStep = 0.25;
      myPane.YAxis.Scale.MinorStep = 0.05;
      myPane.YAxis.Scale.Format = "f2";
      myPane.YAxis.Scale.MajorStepAuto = false;
      myPane.YAxis.Scale.MinorStepAuto = false;
      myPane.YAxis.MajorGrid.IsVisible = true;
      myPane.YAxis.MinorGrid.IsVisible = true;
      myPane.YAxis.MajorGrid.PenWidth = 3f;
      myPane.YAxis.MajorGrid.DashOn = 7f;
      myPane.YAxis.MajorGrid.DashOff = 7f;
      myPane.YAxis.MinorGrid.PenWidth = 2f;

      myPane.Fill = new Fill(Color.White);
      myPane.Border.IsVisible = false;

      myPane.Chart.Fill = new Fill(Color.White);

      foreach (var boxData in Boxes)
      {
        // Draw a box item to highlight a value range
        var box = new BoxObj(0, boxData.Top, 1, boxData.Top - boxData.Bottom)
                  {
                    Fill = boxData.Fill,
                    ZOrder = ZOrder.E_BehindCurves,
                    IsClippedToChartRect = true,
                    Location = { CoordinateFrame = CoordType.XChartFractionYScale }
                  };
        myPane.GraphObjList.Add(box);
      }
      myPane.GraphObjList.AddRange(TextObjs);

      var list = new ClippedPointList(XAxis, YAxis, 0, DroppedPackages ? XAxis.Length - 1 : XAxis.Length);
      var curve = new LineItem("", list, Color.Black, SymbolType.None, 3f);
      myPane.CurveList.Add(curve);

      myPane.AxisChange();
      return myPane;
    }

    public class BoxData
    {
      public Fill Fill { get; set; }
      public double Top { get; set; }
      public double Bottom { get; set; }
    }
  }
}