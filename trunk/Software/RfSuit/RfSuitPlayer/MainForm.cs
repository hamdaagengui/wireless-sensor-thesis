using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using dk.iha;
using ProtoBuf;
using System.Drawing;

namespace RfSuitPlayer
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();
    }

    private Entry[] _entries;

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
      if (trackBar.Value > trackBar.Minimum)
        --trackBar.Value;
    }

    private void NextButtonClick(object sender, EventArgs e)
    {
      if(trackBar.Value < trackBar.Maximum)
        ++trackBar.Value;
    }
  }
}
