using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Windows.Forms;
using dk.iha;
using System.IO;
using System.Threading.Tasks;
using RfSuitLoggerInterfaces;
using System.Diagnostics;

namespace RfSuitCompressLog
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();
    }

    private void button1_Click(object sender, EventArgs e)
    {
      if (openFileDialog.ShowDialog(this) != DialogResult.OK) return;
      compressFiles(openFileDialog.FileNames);
    }

    private void compressFiles(IEnumerable<string> files)
    {
      Enabled = false;
      Task.Factory.StartNew(() => 
      {
        realCompressFiles(files);
        this.BeginInvokeIfRequired(mainForm => mainForm.Enabled = true);
      });
    }

    private void realCompressFiles(IEnumerable<string> files)
    {
      int width, height;
      if (!int.TryParse(maskedTextBox1.Text, out width) || !int.TryParse(maskedTextBox2.Text, out height))
      {
        this.BeginInvokeIfRequired(f => MessageBox.Show(f, "Bad image size", "Please enter a valid size!", MessageBoxButtons.OK, MessageBoxIcon.Error));
        return;
      }

      EntrySerializer serializer = EntrySerializer.Instance;

      foreach (var filename in files)
      {
        try
        {
          using (var input = serializer.OpenRead(filename))
          {
            string path = Path.Combine(Path.GetDirectoryName(filename), Path.GetFileNameWithoutExtension(filename)) + "_compressed";

            using (var output = serializer.OpenWrite(path, true))
            {
              this.BeginInvokeIfRequired(f => f.progressBar1.Maximum = (int)input.Length);
              int logicalProcessorCount = Environment.ProcessorCount * 2;
              var entries = new List<Entry>(logicalProcessorCount);
              int i = 0;

              long position = 0;
              foreach (var entry in input) {
                i++;
                entries.Add(entry);
                position = input.Position;
                if (entries.Count == logicalProcessorCount)
                  work(entries, width, height, position, output);
              }
              work(entries, width, height, position, output);
            }
          }
        }
        catch (Exception ex)
        {
          this.BeginInvokeIfRequired(f => MessageBox.Show(f, ex.GetType().Name, ex.Message, MessageBoxButtons.OK, MessageBoxIcon.Error));
        }
        UpdateProgress(0, true);
      }
    }

    private void UpdateProgress(int value, bool progressBarOnly = false) {
      this.BeginInvokeIfRequired(f => f.progressBar1.Value = value);
      if (progressBarOnly == false)
        this.BeginInvokeIfRequired(f => f.label2.Text = value.ToString());
    }

    private void work(List<Entry> entries, int width, int height, long position, PrefixedWriter<Entry> output)
    {
      var outEntries = new Entry[entries.Count];
      Parallel.For(0, outEntries.Length, i => outEntries[i] = work(entries[i], width, height));
      foreach (var entry in entries)
        output.Write(entry);
      
      entries.Clear();
      GC.Collect();
      UpdateProgress((int)position);
      Application.DoEvents();
    }

    private static Entry work(Entry entry, int width, int height)
    {
      var pictures = new List<Picture>();
      foreach (var picture in entry.pictures)
      {
        var memoryStream = new MemoryStream(picture.data);
        var bitmap = new Bitmap(memoryStream);
        memoryStream.Seek(0, SeekOrigin.Begin);
        memoryStream.SetLength(0);
        memoryStream = new MemoryStream(memoryStream.Capacity);
        var outputBitmap = Scale(bitmap, new Size(width, height));
        outputBitmap.Save(memoryStream, ImageFormat.Jpeg);
        pictures.Add(new Picture
        {
          data = memoryStream.ToArray()
        });
      }

      entry.pictures.Clear();
      entry.pictures.AddRange(pictures);
      return entry;
    }

    private static Bitmap Scale(Bitmap image, Size newSize)
    {
      if (image == null)
        throw new ArgumentNullException("image", "The Bitmap cannot be null");

      if (newSize == Size.Empty)
        throw new ArgumentException("The new size cannot be zero");

      float xScaleFactor = image.Size.Width / (float)newSize.Width;
      float yScaleFactor = image.Size.Height / (float)newSize.Height;
      float scaleFactor = xScaleFactor > yScaleFactor ? xScaleFactor : yScaleFactor;

      //return new Bitmap(image, new Size((int)(image.Width * scaleFactor), (int)(image.Height * scaleFactor)));
      Bitmap newBmp = new Bitmap(
        (int)(image.Size.Width / scaleFactor),
        (int)(image.Size.Height / scaleFactor),
        PixelFormat.Format32bppArgb);//Graphics.FromImage doesn't like Indexed pixel format

      //Create a graphics object attached to the new bitmap
      using(Graphics graphics = Graphics.FromImage(newBmp))
      {
/*        //Set the interpolation mode to high quality bicubic 
        //interpolation, to maximize the quality of the scaled image
        graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;

        graphics.ScaleTransform(scaleFactor, scaleFactor);

        //Draw the bitmap in the graphics object, which will apply
        //the scale transform
        //Note that pixel units must be specified to ensure the framework doesn't attempt
        //to compensate for varying horizontal resolutions in images by resizing; in this case,
        //that's the opposite of what we want.
        Rectangle drawRect = new Rectangle(new Point(), image.Size);
        graphics.DrawImage(image, drawRect, drawRect, GraphicsUnit.Pixel);

        //Return the bitmap, as the operations on the graphics object
        //are applied to the bitmap*/
//        graphics.CompositingMode    = CompositingMode.SourceOver;
//        graphics.CompositingQuality = CompositingQuality.HighQuality;
//        graphics.SmoothingMode      = SmoothingMode.HighQuality;
        graphics.InterpolationMode  = InterpolationMode.HighQualityBicubic;
//        graphics.PixelOffsetMode    = PixelOffsetMode.HighQuality;

        graphics.DrawImage(image, new Rectangle(new Point(), newBmp.Size));
      }

      return newBmp;
    }

    private void MainForm_DragEnter(object sender, DragEventArgs e)
    {
      if (e.Data.GetDataPresent(DataFormats.FileDrop))
        e.Effect = DragDropEffects.Copy;
    }

    private void MainForm_DragDrop(object sender, DragEventArgs e)
    {
      button1.Enabled = false;
      string[] files = e.Data.GetData(DataFormats.FileDrop) as string[];
      if (files == null) return;
      compressFiles(files);
      button1.Enabled = true;
    }
  }
}
