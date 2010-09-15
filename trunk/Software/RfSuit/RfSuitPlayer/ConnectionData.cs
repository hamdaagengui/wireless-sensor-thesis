using System.Drawing;

namespace RfSuitPlayer {
  public class ConnectionData
  {
    public double[] Quality { get; private set; }
    public int EndPointA { get; private set; }
    public int EndPointB { get; private set; }
    public Color Color { get; private set; }

    public ConnectionData(int endPointA, int endPointB, double[] data, Color color)
    {
      Quality = data;
      EndPointA = endPointA;
      EndPointB = endPointB;
      Color = color;
    }

    public override string ToString()
    {
      return EndPointA + " <> " + EndPointB;
    }
  }
}