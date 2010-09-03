using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace RfSuitLogger
{
  class VisualSource
  {
    private readonly Bitmap[] _visuals;

    public VisualSource(int numberOfSources) {
      _visuals = new Bitmap[numberOfSources];
    }

    public void SetVisual(int i, Bitmap bmp) {
      _visuals[i] = bmp;
    }

    public Bitmap[] GetLastVisuals() {
      return (Bitmap[]) _visuals.Clone(); // Array.ConvertAll(_visuals, visual => (Bitmap)visual.Clone());
    }
  }
}
