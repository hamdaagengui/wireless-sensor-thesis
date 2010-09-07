using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using dk.iha;

namespace RfSuitPlayer
{
  class Player
  {
    private volatile bool _isRunning;
    private Thread _thread;
    private readonly ThreadStart _threadStartPlayer;
    private int _position;
    private readonly TrackBar _control;

    public Player(IList<Entry> entries, TrackBar control) {
      _control = control;
      _isRunning = false;
      _threadStartPlayer = new ThreadStart(() =>
      {
        int nextPosition;
        while (_isRunning && (nextPosition = _position + 1) < entries.Count)
        {
          double now = entries[_position].timestamp;
          double next = entries[nextPosition].timestamp;
          ++_position;
          Thread.Sleep((int)(next - now));
          int copiedPosition = nextPosition;
          _control.BeginInvoke(new MethodInvoker(() =>
          {
            _control.Value = copiedPosition;
          }));
        }
        _isRunning = false;
      });
    }

    public void StartPlayer()
    {
      _thread = new Thread(_threadStartPlayer);
      _thread.Start();
      
    }

    public void StopPlayer()
    {
      _isRunning = false;
      if (_thread != null && _thread.Join(1000) == false)
        _thread.Interrupt();
      _thread = null;
    }

    public void TogglePlayer()
    {
      if (_isRunning) {
        StopPlayer();
      }  else {
        StartPlayer();
      }
    }
  }
}
