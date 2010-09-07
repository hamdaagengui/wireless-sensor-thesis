using dk.iha;

namespace RfSuitPlayer {
  public class GraphData {
    public ConnectionData[] ConnectionDatas { get; private set; }
    public int DeviceCount { get; private set; }

    public GraphData(Entry[] entries) {
      var firstEntry = entries[0];
      DeviceCount = firstEntry.results.Count;
      ConnectionDatas = new ConnectionData[DeviceCount];
      for (int i = 0; i < ConnectionDatas.Length; i++) {
        ConnectionDatas[i] = new ConnectionData(firstEntry.results[i].EndPointA, firstEntry.results[i].EndPointB, new double[entries.Length]);
      }

      for (int i = 0; i < entries.Length; i++) {
        var entry = entries[i];
        for (int j = 0; j < ConnectionDatas.Length; j++) {
          ConnectionDatas[j].Quality[i] = entry.results[j].Quality;
        }
      }
    }
  }
}