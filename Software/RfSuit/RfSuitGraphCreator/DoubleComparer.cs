using System.Collections.Generic;

namespace RfSuitGraphCreator
{
  class DoubleComparer : IComparer<double>
  {
    private delegate int CompareDelegate(double x, double y);

    private static readonly CompareDelegate compare = (x, y) => x.CompareTo(y);
    private static readonly CompareDelegate compareReverse = (x, y) => y.CompareTo(x);

    private readonly CompareDelegate comp;

    public DoubleComparer(bool reverse = false)
    {
      comp = reverse ? compareReverse : compare;
    }

    public int Compare(double x, double y)
    {
      return comp(x, y);
    }
  }
}