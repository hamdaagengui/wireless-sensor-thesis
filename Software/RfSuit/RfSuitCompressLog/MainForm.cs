using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Windows.Forms;
using dk.iha;
using ProtoBuf;
using System.IO;
using System.Threading.Tasks;
using RfSuitLoggerInterfaces;

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

      foreach (var filename in files)
      {
        try
        {
          using (FileStream input = File.OpenRead(filename))
          {
            string path = Path.Combine(Path.GetDirectoryName(filename), Path.GetFileNameWithoutExtension(filename)) + "_compressed.rflog";
            if (File.Exists(path))
              File.Delete(path);
            using (FileStream output = File.OpenWrite(Path.Combine(Path.GetDirectoryName(filename), Path.GetFileNameWithoutExtension(filename)) + "_compressed.rflog"))
            {
              this.BeginInvokeIfRequired(f => f.progressBar1.Maximum = (int)input.Length);
              int logicalProcessorCount = Environment.ProcessorCount * 2;
              List<Entry> entries = new List<Entry>(logicalProcessorCount);
              int i = 0;
              while (input.Position != input.Length)
              {
                i++;
                entries.Add(Serializer.DeserializeWithLengthPrefix<Entry>(input, PrefixStyle.Base128));
                if (entries.Count == logicalProcessorCount)
                  work(entries, width, height, input, output);
              }
              work(entries, width, height, input, output);
              Console.WriteLine("Processed " + i + " entries!");
            }
          }
        }
        catch (Exception ex)
        {
          this.BeginInvokeIfRequired(f => MessageBox.Show(f, ex.GetType().Name, ex.Message, MessageBoxButtons.OK, MessageBoxIcon.Error));
        }
        this.BeginInvokeIfRequired(f => f.progressBar1.Value = 0);
      }
    }

    private void work(List<Entry> entries, int width, int height, FileStream input, FileStream output)
    {
      Entry[] outEntries = new Entry[entries.Count];
      Parallel.For(0, outEntries.Length, i => outEntries[i] = work(entries[i], width, height));
      foreach (var entry in entries)
        Serializer.SerializeWithLengthPrefix(output, entry, PrefixStyle.Base128);
      entries.Clear();

      GC.Collect();
      this.BeginInvokeIfRequired(f => f.progressBar1.Value = (int)input.Position);
      Application.DoEvents();
    }

    private static Entry work(Entry entry, int width, int height)
    {
      List<Picture> pictures = new List<Picture>();
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
