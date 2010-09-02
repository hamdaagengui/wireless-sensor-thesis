using System;
using System.Windows.Forms;

namespace RfSuitLogger
{
  static class Utils
  {
    public static void InvokeIfRequired<T>(this T c, Action<T> action) where T : Control
    {
      if (c.InvokeRequired)
      {
        c.Invoke(new MethodInvoker(() => action(c)));
      }
      else
      {
        action(c);
      }
    }
  }
}
