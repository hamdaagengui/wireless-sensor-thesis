namespace RfSuitPlayer {
  public class ConnectionData
  {
    public double[] Quality { get; private set; }
    public int EndPointA { get; private set; }
    public int EndPointB { get; private set; }

    public ConnectionData(int endPointA, int endPointB, double[] data)
    {
      Quality = data;
      EndPointA = endPointA;
      EndPointB = endPointB;
    }
  }
}