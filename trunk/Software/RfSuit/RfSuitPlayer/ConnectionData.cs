using System.Drawing;
using System;
using System.Linq;
using System.Threading.Tasks;

namespace RfSuitPlayer
{
	public class ConnectionData
	{
		private readonly double[] _quality;
		public double[] Quality { get { return _quality; } }

    public double[] GetQuality(int smoothingFactor)
    {
      if (smoothingFactor < 1 || smoothingFactor % 2 != 1)
        throw new ArgumentOutOfRangeException("SmoothingFactor must be 1, 3, 5 .. and so on.");

      if (smoothingFactor == 1)
        return _quality;

      var o = new double[_quality.Length]; 

			int end = smoothingFactor / 2;
			for (int i = end; i < o.Length - end; i++)
			{
        if(double.IsNaN(_quality[i])) {
          o[i] = double.NaN;
          continue;
        }
				double temp = 0;
				int left = i - end;
				int right = i + end;
				int skipped = 0;
				for (int j = left; j <= right; j++)
				{
					double d = _quality[j];
          if (double.IsNaN(d)) {
            skipped++;
            continue;
          }
					temp += _quality[j];
				}
				int processed = smoothingFactor - skipped;
        o[i] = processed != 0 ? temp / processed : double.NaN;
			}

      for (int i = 0; i < end; i++ )
        o[i] = _quality[i];
      for (int i = _quality.Length - end; i < _quality.Length; i++)
  			o[i] = _quality[i];

			return o;
    }
		public int EndPointA { get; private set; }
		public int EndPointB { get; private set; }
		public Color Color { get; private set; }

		public ConnectionData(int endPointA, int endPointB, double[] data, Color color)
		{
			_quality = data;
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