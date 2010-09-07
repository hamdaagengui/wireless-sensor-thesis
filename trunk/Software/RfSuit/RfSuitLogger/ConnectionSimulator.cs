using System;
using System.Threading;
using RfSuit;
using dk.iha;

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
        var links = new LinkQualityIndicator[_numberOfDevices];
        for (int i = 0; i < links.Length; i++) {
          links[i] = new LinkQualityIndicator {EndPointA = i, EndPointB = i + 1, Quality = i};
        }

        _isRunning = true;
        while (_isRunning) {
          if (SweepCompleted != null) {
            SweepCompleted(links);
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
