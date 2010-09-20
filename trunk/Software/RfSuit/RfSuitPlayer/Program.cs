using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace RfSuitPlayer
{
  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main()
    {
      Console.WriteLine(0.1 + 0.1 + 0.1 + Double.NaN + 0.1);
      
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new MainForm());
    }
  }
}
