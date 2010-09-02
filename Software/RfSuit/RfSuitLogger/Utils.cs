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

    public static readonly DateTime EPOCH = new DateTime(1970, 1, 1);
    public static double UnixTime() {
      return UnixTime(DateTime.Now);
    }
    public static double UnixTime(DateTime dt)
    {
      return (dt - EPOCH).TotalSeconds;
    }
  }
}
