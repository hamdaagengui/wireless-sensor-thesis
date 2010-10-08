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
using System.Text.RegularExpressions;

namespace RfSuitGraphCreator
{
  class FileConverter
  {
    private readonly BlockingCollection<ConvertJob> _loadQueue = new BlockingCollection<ConvertJob>(20);
    private readonly BlockingCollection<GraphJob> _graphQueue = new BlockingCollection<GraphJob>(8);

    private static Fill RedFill { get { return new Fill(Color.FromArgb(100, Color.Red), Color.FromArgb(200, Color.Red), -90f); } }
    private static Fill GreenFill { get { return new Fill(Color.FromArgb(100, Color.Green), Color.FromArgb(200, Color.Green), -90f); } }

    public int Processed;
    public int Added;

    public FileConverter()
    {
      Process();
    }

    public void BeginProcessFiles(string[] files, bool mergeScenarios, ConvertTypes convertType)
    {
      Task.Factory.StartNew(() => _loadQueue.Add(new ConvertJob(convertType, files, mergeScenarios)));
    }

    private void Process()
    {
      // Stage 1: Create the graph
      
      Task.Factory.StartNew(() =>
      {
          foreach (var convertJob in _loadQueue.GetConsumingEnumerable())
          {
            switch (convertJob.ConvertType)
            {
              case ConvertTypes.AbsoluteRSSI:
                AbsoluteRSSI(convertJob);
                break;
              case ConvertTypes.LossRSSI:
                throw new NotImplementedException();
              default:
                throw new ArgumentOutOfRangeException();
            }
          }
      });

      // Stage 2: Create the new bitmap file
      Task.Factory.StartNew(() =>
      {
        var action = new Action<object>(obj =>
        {
          var job = (GraphJob)obj;
          var graph = job.GraphFactory.CreateGraph();
          var image = graph.GetImage();
          image.Save(job.Filename);
          Interlocked.Increment(ref Processed);
        });
        foreach (var createGraphWorkItem in _graphQueue.GetConsumingEnumerable())
          Task.Factory.StartNew(action, createGraphWorkItem);
      });
    }

    private void AbsoluteRSSI(ConvertJob convertJob)
    {
      if (convertJob.MergeScenarios)
      {
        var listConnectionDatas = convertJob.Files.Select(file => OpenLogFile(file).ConnectionDatas).ToList();
        var firstConnectionData = listConnectionDatas[0];
        var links = firstConnectionData.Length;

        var linkData = new List<double>[links];
        for (int i = 0; i < links; i++)
          linkData[i] = new List<double>();

        foreach (var connectionData in listConnectionDatas)
        {
          for (int i = 0; i < links; i++)
          {
            var data = connectionData[i];
            linkData[i].AddRange(data.Quality);
          }
        }

        for (int i = 0; i < linkData.Length; i++)
        {
          var data = linkData[i];
          var tuple = CreateCleanCumulativeDistribution(data.ToArray());
          var graphFactory = CreateGraphFactory(tuple, firstConnectionData[i].EndPointA, firstConnectionData[i].EndPointB);
          _graphQueue.Add(new GraphJob(convertJob.GetNewFullPathWithoutExtension(string.Format("{0:00}-{1:00}", firstConnectionData[i].EndPointA, firstConnectionData[i].EndPointB)) + ".png", graphFactory));
        }
      }
      else
      {
        foreach (var file in convertJob.Files)
        {
          var connectionDatas = OpenLogFile(file).ConnectionDatas;
          foreach (var connectionData in connectionDatas)
          {
            var tuple = CreateCleanCumulativeDistribution(connectionData.Quality);
            var graphFactory = CreateGraphFactory(tuple, connectionData.EndPointA, connectionData.EndPointB);
            _graphQueue.Add(new GraphJob(convertJob.GetNewFullPathWithoutExtension(string.Format("{0:00}-{1:00}", connectionData.EndPointA, connectionData.EndPointB)) + ".png", graphFactory));
          }
        }
      }
    }

    /// <summary>
    /// This will clean the data set
    /// </summary>
    /// <param name="data"></param>
    /// <returns>A tuple containing x and y axis information.</returns>
    private static Tuple<double[], double[]> CreateCleanCumulativeDistribution(IEnumerable<double> data)
    {
      return CreateCumulativeDistribution(data.Select(d => double.IsNaN(d) ? -100 : d), true);      
    }
    
    private GraphFactory CreateGraphFactory(Tuple<double[], double[]> tuple, int endPointA, int endPointB)
    {
        Interlocked.Increment(ref Added);
        var createGraphWorkItem = new GraphFactory
        {
          Title = String.Format("{0:00}-{1:00}", endPointA, endPointB),
          XAxis = tuple.Item1,
          YAxis = tuple.Item2,
          XTitle = "[dBm]",
          YTitle = "",
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
        return createGraphWorkItem;
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

    private static Tuple<double[], double[]> CreateCumulativeDistribution(IEnumerable<double> x, bool reverse = false)
    {
      if (x == null) throw new ArgumentNullException("x");
      var dict = new Dictionary<double, int>();
      var count = 0;
      foreach (var d in x)
      {
        count++;
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

        tuple.Item2[i] = (double)sum / count;
        i++;
      }

      return tuple;
    }
  }

  internal enum ConvertTypes
  {
    AbsoluteRSSI,
    LossRSSI,
  }

  internal class ConvertJob
  {
    public ConvertTypes ConvertType { get; private set; }
    public string[] Files { get; private set; }
    public bool MergeScenarios { get; private set; }

    public ConvertJob(ConvertTypes ct, string[] file, bool mergeScenarios)
    {
      ConvertType = ct;
      Files = file;
      MergeScenarios = mergeScenarios;
    }
    private readonly Regex _matchFile = new Regex(@"log(?<time>\d+)\[ch (?<channel>\d+), (?<dBm>-?\d+) dBm\]\s*(?<title>.*)");

    public string GetNewFullPathWithoutExtension(string subtitle)
    {
      // org: log1285938414696[ch 11, -3 dBm] Radiodøde rum_compressed.rflogz
      var match = _matchFile.Match(Path.GetFileNameWithoutExtension(Files[0]));
      var channel = int.Parse(match.Groups["channel"].Value);
      var dBm = int.Parse(match.Groups["dBm"].Value);
      var title = match.Groups["title"].Value;
      var time = long.Parse(match.Groups["time"].Value);
      var filename = string.Format("{0} {1} [{2}, {3}]{4}", MergeScenarios ? "Merged" : title, subtitle, channel, dBm, MergeScenarios ? "" : " " + Utils.MillisecondsSinceEpoch(time).ToString("yyyyMMddTHHmmss"));
      var directory = Path.GetDirectoryName(Files[0]);
      return Path.Combine(directory, filename);
    }
  }

  internal class GraphJob
  {
    public GraphFactory GraphFactory { get; private set; }
    public string Filename { get; private set; } 
    
    public GraphJob(string outputPath, GraphFactory gf)
    {
      Filename = outputPath;
      GraphFactory = gf;
    }
  }
}
