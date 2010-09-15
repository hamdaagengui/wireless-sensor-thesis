using System;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace RfSuitLoggerInterfaces
{
  public static class Utils
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

    public static readonly DateTime Epoch = new DateTime(1970, 1, 1);
    public static double MillisecondsSinceEpoch()
    {
      return MillisecondsSinceEpoch(DateTime.Now);
    }
    public static double MillisecondsSinceEpoch(DateTime dt)
    {
      return (dt - Epoch).TotalMilliseconds;
    }
    public static DateTime MillisecondsSinceEpoch(double ms)
    {
      return Epoch + TimeSpan.FromMilliseconds(ms);
    }
  }

  public class SerialPortInfo {
    [DllImport("kernel32.dll")]
    private static extern uint QueryDosDevice(string lpDeviceName, StringBuilder lpTargetPath, int ucchMax);

    public string Name { get; private set; }
    public string Info { get; private set; }

    public SerialPortInfo(string name) {
      Name = name;
      var portInfo = new StringBuilder(byte.MaxValue);
      var result = QueryDosDevice(name, portInfo, portInfo.Capacity);
      Info = (result != 0 ? portInfo.ToString().Replace(@"\Device\", "") : "N/A " + Marshal.GetLastWin32Error());
    }

    public override string ToString()
    {
      return Name + " (" + Info + ")";
    }

    public static SerialPortInfo[] GetSerialPorts()
    {
      return Array.ConvertAll(SerialPort.GetPortNames(), portName => new SerialPortInfo(portName));
    }
  }
}
