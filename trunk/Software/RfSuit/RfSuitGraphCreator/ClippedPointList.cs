using System.Collections.Generic;
using ZedGraph;

namespace RfSuitGraphCreator
{
  class ClippedPointList : IPointList
  {
    private readonly PointPair[] _pointPairs;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="start">inclusive</param>
    /// <param name="stop">exclusive</param>
    public ClippedPointList(IList<double> x, IList<double> y, int start, int stop)
    {
      _pointPairs = new PointPair[stop - start];
      for (int i = start; i < stop; i++)
        _pointPairs[i-start] = new PointPair(x[i], y[i]);
    }

    private ClippedPointList(PointPair[] pointPairs)
    {
      _pointPairs = pointPairs;
    }

    #region IPointList Members

    public int Count
    {
      get { return _pointPairs.Length; }
    }

    public PointPair this[int index]
    {
      get { return _pointPairs[index]; }
    }

    #endregion

    #region ICloneable Members

    public object Clone()
    {
      return new ClippedPointList(_pointPairs);
    }

    #endregion
  }
}
