using System;
using System.Windows.Forms;

namespace RfSuitGraphCreator
{
  public partial class MainForm : Form
  {
    private readonly FileConverter _fileConverter;

    public MainForm()
    {
      InitializeComponent();
      _fileConverter = new FileConverter();
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
      _fileConverter.BeginProcessFiles(data);
    }

    private void TimerTick(object sender, EventArgs e)
    {
      var str = string.Format("{0}/{1}", _fileConverter.Processed, _fileConverter.Added);
      processedLabel.Text = str;
    }
  }
}
