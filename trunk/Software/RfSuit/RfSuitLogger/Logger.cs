using System.Drawing.Imaging;
using System.IO;
using System.Drawing;
using dk.iha;
using RfSuit;
using RfSuitLoggerInterfaces;

namespace RfSuitLogger
{
  class Logger
  {
    private readonly object _sync = new object();
    private PrefixedWriter<Entry> _prefixedWriter;
    private readonly VisualSource _visualSource;
    private readonly IConnection _connection;

    public Logger(VisualSource vs, IConnection connection) {
      _connection = connection;
      _visualSource = vs;
    }

    public bool IsLogging() {
      lock (_sync) {
        return _prefixedWriter != null;
      }
    }

    public void Start(PrefixedWriter<Entry> prefixedWriter, string connectionPort)
    {
      lock (_sync)
      {
        if (IsLogging())
          Stop();
        _prefixedWriter = prefixedWriter;
        _connection.SweepCompleted += ConnectionSweepCompleted;
        _connection.Start(connectionPort);
      }
    }

    private void ConnectionSweepCompleted(LinkQualityIndicator[] results)
    {
      lock (_sync)
      {
        if (_prefixedWriter == null)
          return;
        var entry = new Entry {timestamp = Utils.MillisecondsSinceEpoch()};

        entry.results.AddRange(results);

        Bitmap[] visuals = _visualSource.GetLastVisuals();
        foreach (var visual in visuals) {
          var memoryStream = new MemoryStream(10*1024*1024);
          visual.Save(memoryStream, ImageFormat.Jpeg);
          entry.pictures.Add(new Picture {data = memoryStream.ToArray()});
        }

        _prefixedWriter.Write(entry);
      }
    }

    public void Stop() {
      lock (_sync)
      {
        if (IsLogging())
          _prefixedWriter.Close();
        _connection.SweepCompleted -= ConnectionSweepCompleted;
        _connection.Stop();
        _prefixedWriter = null;
      }
    }
  }
}
