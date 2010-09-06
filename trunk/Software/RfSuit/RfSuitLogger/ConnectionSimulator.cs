using System;
using System.Threading;
using RfSuit;

namespace RfSuitLogger
{
  class ConnectionSimulator : IConnection
  {
    private readonly int _numberOfDevices;

    private Thread _thread;
    private volatile bool _isRunning;

    public ConnectionSimulator(int numberOfDevices)
    {
      _numberOfDevices = numberOfDevices;
    }

    public bool[] DetectDevices() {
      throw new NotImplementedException();
    }

    public bool Start(string portName) {
      if(_thread != null) {
        return false;
      }
      Console.WriteLine("Starting simulation [" + portName + "]");
      _thread = new Thread(() => {
        var sweepResults = new SweepResults[_numberOfDevices];
        for (int i = 0; i < _numberOfDevices; i++)
        {
          var tmp = new SweepResults {
            Rssis = new int[_numberOfDevices]
          };
          for (int j = 0; j < _numberOfDevices; j++)
          {
            tmp.Rssis[j] = i * _numberOfDevices + j;
          }
          sweepResults[i] = tmp;
        }
        _isRunning = true;
        while (_isRunning) {
          if (SweepCompleted != null) {
            SweepCompleted(sweepResults);
            Thread.Sleep(1000);
          }
        }
        _thread = null;
      });
      _thread.Start();
      return true;
    }

    public void Stop() {
      _isRunning = false;
    }

    public event SweepCompletedDelegate SweepCompleted;
  }
}
