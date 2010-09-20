using System.Drawing;
using System;
using System.Linq;

namespace RfSuitPlayer {
  public class ConnectionData
  {
    private readonly double[] _quality;
    public double[] Quality { get
    {
      if(SmoothingFactor == 1)
        return _quality;

      var output = new double[_quality.Length];
      var window = new double[SmoothingFactor];

      int left = SmoothingFactor/2;
      int right = SmoothingFactor - (left + 1);

      for (int i = left; i < _quality.Length - (SmoothingFactor - (left + 1)); i++)
      {
        Array.Copy(_quality, i, window, 0, window.Length);
        output[i] = window.Sum() / SmoothingFactor;
      }

      for (int i = 0; i < left; i++)
        output[i] = _quality[i];

      for (int i = _quality.Length - right; i < _quality.Length; i++)
        output[i] = _quality[i];

      return output;
    }}
    public int EndPointA { get; private set; }
    public int EndPointB { get; private set; }
    public Color Color { get; private set; }
    private int _smoothingFactor;
    public int SmoothingFactor
    {
      get { return _smoothingFactor; }
      set
      {
        if (value < 1)
          throw new ArgumentOutOfRangeException();
        _smoothingFactor = value;
      }
    }

    public ConnectionData(int endPointA, int endPointB, double[] data, Color color)
    {
      _quality = data;
      EndPointA = endPointA;
      EndPointB = endPointB;
      Color = color;
      SmoothingFactor = 1;
    }

    public override string ToString()
    {
      return EndPointA + " <> " + EndPointB;
    }
  }
}