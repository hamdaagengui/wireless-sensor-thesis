using System.Drawing.Imaging;
using System.IO;
using System.Drawing;
using dk.iha;
using System;
using RfSuit;
using ProtoBuf;
using System.Diagnostics;

namespace RfSuitLogger
{
  class Logger
  {
    private readonly object _sync = new object();
    private Stream _stream;
    private readonly VisualSource _visualSource;
    private readonly Connection _connection;

    public Logger(VisualSource vs) {
      _connection = new Connection();
      _visualSource = vs;
    }

    public bool IsLogging() {
      lock (_sync) {
        return _stream != null;
      }
    }

    public void Start(Stream s, string connectionPort) {
      lock (_sync)
      {
        if (IsLogging())
          stop();
        _stream = s;
        _connection.SweepCompleted += ConnectionSweepCompleted;
        _connection.Start(connectionPort);
      }
    }

    private void ConnectionSweepCompleted(RfSuit.SweepResults[] results)
    {
      lock (_sync)
      {
        if (_stream == null)
          return;
        var stopwatch = Stopwatch.StartNew();
        var entry = new Entry {timestamp = (int) Utils.UnixTime()};

        foreach (var sweepResults in results) {
          var tmp = new SweepResult();
          tmp.rssis.AddRange(sweepResults.Rssis); 
          entry.results.Add(tmp);
        }

        Bitmap[] visuals = _visualSource.GetLastVisuals();
        foreach (var visual in visuals) {
          var memoryStream = new MemoryStream(10*1024*1024);
          visual.Save(memoryStream, ImageFormat.Jpeg);
          entry.pictures.Add(new Picture {data = memoryStream.ToArray()});
        }

        Serializer.SerializeWithLengthPrefix(_stream, entry, PrefixStyle.Base128);
        stopwatch.Stop();

        Console.WriteLine("Logged an entry in " + TimeSpan.FromMilliseconds(stopwatch.ElapsedMilliseconds));
      }
    }

    public void stop() {
      lock (_sync)
      {
        if (IsLogging())
          _stream.Close();
        _connection.SweepCompleted -= ConnectionSweepCompleted;
        _connection.Stop();
        _stream = null;
      }
    }
  }
}
