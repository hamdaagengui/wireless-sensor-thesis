using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using RfSuitLoggerInterfaces;
using RfSuitPlayer;
using System.Threading.Tasks;

namespace RfSuitGraphCreator
{
  public partial class MainForm : Form
  {
    private readonly FileToGraph _fileToGraph;

    public MainForm()
    {
      InitializeComponent();
      _fileToGraph = new FileToGraph(statusLabel, queueLabel);
    }

    private void MainForm_Load(object sender, EventArgs e)
    {
      TopLevel = true;
      TopMost = true;
    }

    private void MainFormDragEnter(object sender, DragEventArgs e)
    {
      if (e.Data.GetDataPresent(DataFormats.FileDrop))
        e.Effect = DragDropEffects.Copy;
    }

    private void MainFormDragDrop(object sender, DragEventArgs e)
    {
      var data = e.Data.GetData(DataFormats.FileDrop) as string[];
      if (data == null) return;
      _fileToGraph.BeginProcessFiles(data);
    }
  }
}
