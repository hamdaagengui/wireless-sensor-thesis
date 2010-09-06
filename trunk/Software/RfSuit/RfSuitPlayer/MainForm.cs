using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using dk.iha;
using ProtoBuf;
using System.Drawing;
using System.Threading;

namespace RfSuitPlayer
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();

      _isRunning = false;
      _threadStartPlayer = new ThreadStart(() =>
      {
        int nextPosition;
        while (_isRunning && (nextPosition = _position + 1) < _entries.Length)
        {
          double now = _entries[_position].timestamp;
          double next = _entries[nextPosition].timestamp;
          ++_position;
          Thread.Sleep((int) (next - now));
          int position = nextPosition;
          trackBar.BeginInvoke(new MethodInvoker(() =>
          {
            trackBar.Value = position;
          }));
        }
        _isRunning = false;
      });
    }

    private Entry[] _entries;
    private int _position;
    private volatile bool _isRunning;
    private Thread _thread;
    private readonly ThreadStart _threadStartPlayer;

    private void OpenToolStripMenuItemClick(object sender, EventArgs e)
    {
      if(openFileDialog.ShowDialog(this) != DialogResult.OK) return;
      var fileStream = File.OpenRead(openFileDialog.FileName);
      
      var entries = new List<Entry>(1000);
      while(fileStream.Position < fileStream.Length) {
        var entry = Serializer.DeserializeWithLengthPrefix<Entry>(fileStream, PrefixStyle.Base128);
        entries.Add(entry);
      }
      _entries = entries.ToArray();
      trackBar.Minimum = 0;
      trackBar.Maximum = _entries.Length - 1;
      trackBar.Value = 0;
    }

    private void ExitToolStripMenuItemClick(object sender, EventArgs e)
    {
      Close();
    }

    private void TrackBarValueChanged(object sender, EventArgs e)
    {
      if (_entries == null) return;
      var entry = _entries[trackBar.Value];
      pictureBox.Image = Image.FromStream(new MemoryStream(entry.pictures[0].data));
      propertyGrid.SelectedObjects = entry.results.ToArray();
    }

    private void PreviousButtonClick(object sender, EventArgs e)
    {
      _isRunning = false;
      if (trackBar.Value > trackBar.Minimum)
        --trackBar.Value;
    }

    private void NextButtonClick(object sender, EventArgs e)
    {
      _isRunning = false;
      if(trackBar.Value < trackBar.Maximum)
        ++trackBar.Value;
    }

    private void PlayButtonClick(object sender, EventArgs e)
    {
      if (_isRunning)
      {
        StopPlayer();
      }
      else
      {
        StartPlayer();
      }
    }

    private void StartPlayer()
    {
      _thread = new Thread(_threadStartPlayer);
      _thread.Start();
    }

    private void StopPlayer()
    {
      _isRunning = false;
      if(_thread.Join(1000) == false)
        _thread.Interrupt();
      _thread = null;
    }
  }
}
