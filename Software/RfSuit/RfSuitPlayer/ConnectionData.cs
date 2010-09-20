using System.Drawing;
using System;
using System.Linq;

namespace RfSuitPlayer
{
	public class ConnectionData
	{
		private readonly double[] _quality;
		public double[] Quality
		{
			get
			{
				if (SmoothingFactor == 1)
					return _quality;

				var o = new double[_quality.Length];

				int end = SmoothingFactor / 2;
				for (int i = 2; i < o.Length - 2; i++)
				{
					double temp = 0;
					int left = i - end;
					int right = i + end;
					for (int j = left; j <= right; j++)
					{
						temp += _quality[j];
					}
					o[i] = temp / SmoothingFactor;
				}

				o[0] = _quality[0];
				o[1] = _quality[1];
				o[o.Length - 2] = _quality[o.Length - 2];
				o[o.Length - 1] = _quality[o.Length - 1];

				return o;

				//var output = new double[_quality.Length];
				//var window = new double[SmoothingFactor];

				//int left = SmoothingFactor / 2;
				//int right = SmoothingFactor - (left + 1);

				//for (int i = left; i < _quality.Length - right; i++)
				//{
				//  Array.Copy(_quality, i - left, window, 0, window.Length);
				//  output[i] = window.Sum() / SmoothingFactor;
				//}

				//for (int i = 0; i < left; i++)
				//  output[i] = _quality[i];

				//for (int i = _quality.Length - right; i < _quality.Length; i++)
				//  output[i] = _quality[i];

				//return output;
			}
		}
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
			SmoothingFactor = 5;
		}

		public override string ToString()
		{
			return EndPointA + " <> " + EndPointB;
		}
	}
}