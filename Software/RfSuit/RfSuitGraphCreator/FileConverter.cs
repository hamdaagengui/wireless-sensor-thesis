using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using RfSuitLoggerInterfaces;
using RfSuitPlayer;
using ZedGraph;
using System.Threading;

namespace RfSuitGraphCreator
{
  class FileConverter
  {
    enum ConvertTypes
    {
      AbsoluteRSSI,
      LossRSSI,
    }


    private readonly BlockingCollection<string> _fileCollection = new BlockingCollection<string>(20);
    private readonly BlockingCollection<GraphFactory> _graphCollection = new BlockingCollection<GraphFactory>(8);

    private static Fill RedFill { get { return new Fill(Color.FromArgb(100, Color.Red), Color.FromArgb(200, Color.Red), -90f); } }
    private static Fill GreenFill { get { return new Fill(Color.FromArgb(100, Color.Green), Color.FromArgb(200, Color.Green), -90f); } }

    public int Processed;
    public int Added;

    public FileConverter()
    {
      Process();
    }

    public void BeginProcessFiles(IEnumerable<string> files)
    {
      Task.Factory.StartNew(() =>
      {
        foreach (var file in files)
          _fileCollection.Add(file);
      });
    }

    private void Process()
    {
      // Stage 1: Create the graph
      
      Task.Factory.StartNew(() =>
      {
          foreach (var file in _fileCollection.GetConsumingEnumerable())
          {
            var filename = file;
            var graphData = OpenLogFile(filename);
            foreach(var data in graphData.ConnectionDatas)
            {
              Interlocked.Increment(ref Added);
              var cleanQualities = CleanQuality(data.Quality);
              var tuple = CreateCumulativeDistribution(cleanQualities, true);
              var createGraphWorkItem = new GraphFactory
              {
                Title = String.Format("{0:00}-{1:00}", data.EndPointA, data.EndPointB),
                XAxis = tuple.Item1,
                YAxis = tuple.Item2,
                XTitle = "[dBm]",
                YTitle = "",
                Filename = filename,
                Reverse = true,
              };

              var lastIndex = tuple.Item1.Length - 1;

              var q = 0.25;
              foreach (var dBm in FindQuartiles(tuple))
              {
                if (double.IsNaN(dBm))
                {
                  q += 0.25;
                  continue;
                }
                var textObj = new TextObj(string.Format("{0:0.0} [dBm]", dBm), dBm, q) { Location = { AlignH = AlignH.Right, AlignV = AlignV.Bottom } };
                createGraphWorkItem.TextObjs.Add(textObj);
                q += 0.25;
              }
              // create red and green boxes
              if (tuple.Item1[lastIndex] == -100)
              {
                var boxData = new GraphFactory.BoxData
                {
                  Top = tuple.Item2[lastIndex],
                  Bottom = tuple.Item1.Length > 1 ? tuple.Item2[lastIndex - 1] : 0,
                  Fill = RedFill
                };
                createGraphWorkItem.Boxes.Add(boxData);
                createGraphWorkItem.DroppedPackages = true;
                createGraphWorkItem.TextObjs.Add(new TextObj(string.Format("{0:0.0%} frame loss", 1 - boxData.Bottom), 0, boxData.Bottom) { Location = { AlignH = AlignH.Left, AlignV = AlignV.Top, CoordinateFrame = CoordType.XChartFractionYScale } });
                if (boxData.Bottom != 0)
                {
                  createGraphWorkItem.Boxes.Add(new GraphFactory.BoxData
                  {
                    Top = boxData.Bottom,
                    Bottom = 0,
                    Fill = GreenFill
                  });
                }
              }
              else
              {
                createGraphWorkItem.Boxes.Add(new GraphFactory.BoxData
                {
                  Top = 1,
                  Bottom = 0,
                  Fill = GreenFill
                });
              }
              _graphCollection.Add(createGraphWorkItem);
            }
          }
      });

      // Stage 2: Create the new bitmap file
      Task.Factory.StartNew(() =>
      {
        var action = new Action<object>(obj =>
        {
          var item = (GraphFactory) obj;
          var filename = Path.ChangeExtension(item.Filename, "." + item.Title + ".png");
          var graph = item.CreateGraph();
          var image = graph.GetImage();
          image.Save(filename);
          Interlocked.Increment(ref Processed);
        });
        foreach (var createGraphWorkItem in _graphCollection.GetConsumingEnumerable())
          Task.Factory.StartNew(action, createGraphWorkItem);
      });
    }

    private static IEnumerable<double> FindQuartiles(Tuple<double[], double[]> tuple)
    {
      var quartiles = new List<double>();
      // first quartile to find
      var q = 0.25;
      for (var i = 1; i < tuple.Item2.Length-1 && q < 1; i++)
      {
        var value = tuple.Item2[i];
        while (q < value && q < 1)
        {
          if (q < tuple.Item2[i-1])
          {
            q += 0.25;
            quartiles.Add(double.NaN);
            continue;
          }
          var x1 = tuple.Item1[i - 1];
          var x2 = tuple.Item1[i];
          var y1 = tuple.Item2[i - 1];
          var y2 = tuple.Item2[i];
          // y1 = a*x1 + b
          // y2 = a*x2 + b
          // b = y1 - a*x1;
          // b = y2 - a*x2;
          // y2 - a*x2 = y1 - a*x1
          // y2 - y1 = a*x2 - a*x1
          // y2 - y1 = (x2 - x1)*a
          var a = (y2 - y1)/(x2 - x1);
          var b = y1 - a*x1;
          // q = a*x + b
          // q - b = a*x
          var x = (q - b)/a;
          quartiles.Add(x);
          q += 0.25;
        }
      }
      return quartiles.ToArray();
    }

    private static GraphData OpenLogFile(string path)
    {
      var entries = EntrySerializer.Instance.OpenRead(path);
      return new GraphData(entries.ToArray());
    }

    private static double[] CleanQuality(double[] qualities)
    {
      return Array.ConvertAll(qualities, d => double.IsNaN(d) ? -100 : d);
    }

    private static Tuple<double[], double[]> CreateCumulativeDistribution(double[] x, bool reverse = false)
    {
      if (x == null) throw new ArgumentNullException("x");
      var length = x.Length;
      var dict = new Dictionary<double, int>();
      foreach (var d in x)
      {
        if (dict.ContainsKey(d))
          dict[d]++;
        else
          dict.Add(d, 1);
      }

      var keys = dict.Keys.ToArray();
      Array.Sort(keys, new DoubleComparer(reverse));
      var tuple = Tuple.Create(new double[dict.Count], new double[dict.Count]);
      var i = 0;
      var sum = 0;
      foreach (var key in keys)
      {
        sum += dict[key];
        tuple.Item1[i] = key;

        tuple.Item2[i] = (double)sum / length;
        i++;
      }

      return tuple;
    }
  }
}
