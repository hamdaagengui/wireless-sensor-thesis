using dk.iha;
using RfSuitLoggerInterfaces;
using ZedGraph;
using System;

namespace RfSuitPlayer {
  public class GraphData {
    public ConnectionData[] ConnectionDatas { get; private set; }
    public double[] Timeline { get; private set; }
    public int DeviceCount { get; private set; }

    public int GetNearestIndex(double timestamp) {
      int i = Array.BinarySearch(Timeline, timestamp);
      if (i < 0) {
        i = ~i;
        if (i >= Timeline.Length)
          i = Timeline.Length - 1;
        if (i == 0)
          return i;
        double prev = Timeline[i - 1];
        double next = Timeline[i];
        if (timestamp - prev < next - timestamp)
          return i - 1;
        return i;
      } else {
        return i;
      }

    }

    public GraphData(Entry[] entries) {
      var rotator = new ColorSymbolRotator();

      var firstEntry = entries[0];
      DeviceCount = firstEntry.results.Count;

      Timeline = new double[entries.Length];
      var data = new double[DeviceCount][];
      for (int i = 0; i < data.Length; i++) {
        data[i] = new double[entries.Length];
      }

      var firstTimestamp = firstEntry.timestamp;
      for (int i = 0; i < entries.Length; i++)
      {
        var entry = entries[i];
        Timeline[i] = Utils.MillisecondsSinceEpoch(entry.timestamp - firstTimestamp).ToOADate();
        for (int j = 0; j < DeviceCount; j++)
        {
          var q = entry.results[j].Quality;
          data[j][i] = (q != 0.0 ? q : double.NaN);
        }
      }

      ConnectionDatas = new ConnectionData[DeviceCount];
      for (int i = 0; i < ConnectionDatas.Length; i++) {
        ConnectionDatas[i] = new ConnectionData(firstEntry.results[i].EndPointA, firstEntry.results[i].EndPointB, data[i], rotator.NextColor);
      }
    }
  }
}