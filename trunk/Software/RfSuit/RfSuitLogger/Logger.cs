using System.Drawing.Imaging;
using System.IO;
using System.Drawing;
using dk.iha;
using RfSuit;
using RfSuitLoggerInterfaces;
using System;
using System.Diagnostics;

namespace RfSuitLogger
{
  class Logger
  {
    private readonly object _sync = new object();
    private PrefixedWriter<Entry> _prefixedWriter;
    private readonly VisualSource _visualSource;
    public IConnection Connection { get; private set; }
    private Stopwatch _stopwatch;
    private long _counter;
    private long _totalCounter;
    public event EventHandler<UpdateEventArgs> UpdateStatus;

    public Logger(VisualSource vs, IConnection connection) {
      Connection = connection;
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
        _stopwatch = Stopwatch.StartNew();
        _counter = 0;
        _totalCounter = 0;
        Connection.SweepCompleted += ConnectionSweepCompleted;
        Connection.Start(connectionPort);
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

        _counter++;

        if (UpdateStatus != null && _stopwatch.ElapsedMilliseconds > 1000) {
          _totalCounter += _counter;
          UpdateStatus(this, new UpdateEventArgs(_counter, _totalCounter));
          _counter = 0;
          _stopwatch.Restart();
        }
      }
    }

    public void Stop() {
      lock (_sync)
      {
        if (IsLogging())
          _prefixedWriter.Close();
        Connection.SweepCompleted -= ConnectionSweepCompleted;
        if(_stopwatch != null)
          _stopwatch.Stop();
        Connection.Stop();
        _prefixedWriter = null;
      }
    }

    public class UpdateEventArgs : EventArgs {
      public long Count { get; private set; }
      public long TotalCount { get; private set; }
      public UpdateEventArgs(long count, long totalCount) {
        Count = count;
        TotalCount = totalCount;
      }
    }
  }
}
