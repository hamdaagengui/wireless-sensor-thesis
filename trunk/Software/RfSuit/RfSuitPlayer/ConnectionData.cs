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

      double[] tmp = new double[_quality.Length + (SmoothingFactor - 1)];
      double[] output = new double[_quality.Length];
      double[] window = new double[SmoothingFactor];

      int left = SmoothingFactor/2;
      Array.Copy(_quality, 0, tmp, left, _quality.Length); // Create an array with zero padding on each size to accomodate for the window

      for (int i = 0; i < _quality.Length; i++)
      {
        Array.Copy(tmp, i-left, window, 0, window.Length);
        double sum = window.Sum();
        output[i] = sum / SmoothingFactor;
      }
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