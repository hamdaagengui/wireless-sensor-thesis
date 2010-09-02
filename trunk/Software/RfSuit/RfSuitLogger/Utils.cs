using System;
using System.Windows.Forms;

namespace RfSuitLogger
{
  static class Utils
  {
    public static void BeginInvokeIfRequired<T>(this T c, Action<T> action) where T : Control
    {
      if (c.InvokeRequired)
      {
        c.BeginInvoke(new MethodInvoker(() => action(c)));
      }
      else
      {
        action(c);
      }
    }
  }
}
