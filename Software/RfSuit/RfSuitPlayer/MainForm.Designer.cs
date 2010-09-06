namespace RfSuitPlayer
{
  partial class MainForm
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.menuStrip = new System.Windows.Forms.MenuStrip();
      this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
      this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.pictureBox = new System.Windows.Forms.PictureBox();
      this.splitContainer = new System.Windows.Forms.SplitContainer();
      this.propertyGrid = new System.Windows.Forms.PropertyGrid();
      this.trackBar = new System.Windows.Forms.TrackBar();
      this.nextButton = new System.Windows.Forms.Button();
      this.previousButton = new System.Windows.Forms.Button();
      this.playButton = new System.Windows.Forms.Button();
      this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
      this.menuStrip.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).BeginInit();
      this.splitContainer.Panel1.SuspendLayout();
      this.splitContainer.Panel2.SuspendLayout();
      this.splitContainer.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.trackBar)).BeginInit();
      this.SuspendLayout();
      // 
      // menuStrip
      // 
      this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
      this.menuStrip.Location = new System.Drawing.Point(0, 0);
      this.menuStrip.Name = "menuStrip";
      this.menuStrip.Size = new System.Drawing.Size(633, 24);
      this.menuStrip.TabIndex = 0;
      // 
      // fileToolStripMenuItem
      // 
      this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
      this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
      this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
      this.fileToolStripMenuItem.Text = "&File";
      // 
      // openToolStripMenuItem
      // 
      this.openToolStripMenuItem.Name = "openToolStripMenuItem";
      this.openToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
      this.openToolStripMenuItem.Text = "&Open";
      this.openToolStripMenuItem.Click += new System.EventHandler(this.OpenToolStripMenuItemClick);
      // 
      // toolStripSeparator1
      // 
      this.toolStripSeparator1.Name = "toolStripSeparator1";
      this.toolStripSeparator1.Size = new System.Drawing.Size(131, 6);
      // 
      // exitToolStripMenuItem
      // 
      this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
      this.exitToolStripMenuItem.ShortcutKeyDisplayString = "Alt+F4";
      this.exitToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
      this.exitToolStripMenuItem.Text = "E&xit";
      this.exitToolStripMenuItem.Click += new System.EventHandler(this.ExitToolStripMenuItemClick);
      // 
      // pictureBox
      // 
      this.pictureBox.Dock = System.Windows.Forms.DockStyle.Fill;
      this.pictureBox.Location = new System.Drawing.Point(0, 0);
      this.pictureBox.Name = "pictureBox";
      this.pictureBox.Size = new System.Drawing.Size(420, 308);
      this.pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
      this.pictureBox.TabIndex = 0;
      this.pictureBox.TabStop = false;
      // 
      // splitContainer
      // 
      this.splitContainer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.splitContainer.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
      this.splitContainer.Location = new System.Drawing.Point(12, 107);
      this.splitContainer.Name = "splitContainer";
      // 
      // splitContainer.Panel1
      // 
      this.splitContainer.Panel1.Controls.Add(this.pictureBox);
      // 
      // splitContainer.Panel2
      // 
      this.splitContainer.Panel2.Controls.Add(this.propertyGrid);
      this.splitContainer.Size = new System.Drawing.Size(609, 308);
      this.splitContainer.SplitterDistance = 420;
      this.splitContainer.TabIndex = 10;
      // 
      // propertyGrid
      // 
      this.propertyGrid.Dock = System.Windows.Forms.DockStyle.Fill;
      this.propertyGrid.Location = new System.Drawing.Point(0, 0);
      this.propertyGrid.Name = "propertyGrid";
      this.propertyGrid.Size = new System.Drawing.Size(185, 308);
      this.propertyGrid.TabIndex = 0;
      // 
      // trackBar
      // 
      this.trackBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.trackBar.Location = new System.Drawing.Point(12, 56);
      this.trackBar.Name = "trackBar";
      this.trackBar.Size = new System.Drawing.Size(609, 45);
      this.trackBar.TabIndex = 9;
      this.trackBar.ValueChanged += new System.EventHandler(this.TrackBarValueChanged);
      // 
      // nextButton
      // 
      this.nextButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this.nextButton.Location = new System.Drawing.Point(546, 27);
      this.nextButton.Name = "nextButton";
      this.nextButton.Size = new System.Drawing.Size(75, 23);
      this.nextButton.TabIndex = 8;
      this.nextButton.Text = ">";
      this.nextButton.UseVisualStyleBackColor = true;
      this.nextButton.Click += new System.EventHandler(this.NextButtonClick);
      // 
      // previousButton
      // 
      this.previousButton.Location = new System.Drawing.Point(12, 27);
      this.previousButton.Name = "previousButton";
      this.previousButton.Size = new System.Drawing.Size(75, 23);
      this.previousButton.TabIndex = 7;
      this.previousButton.Text = "<";
      this.previousButton.UseVisualStyleBackColor = true;
      this.previousButton.Click += new System.EventHandler(this.PreviousButtonClick);
      // 
      // playButton
      // 
      this.playButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.playButton.Location = new System.Drawing.Point(93, 27);
      this.playButton.Name = "playButton";
      this.playButton.Size = new System.Drawing.Size(447, 23);
      this.playButton.TabIndex = 6;
      this.playButton.Text = "Play";
      this.playButton.UseVisualStyleBackColor = true;
      this.playButton.Click += new System.EventHandler(this.PlayButtonClick);
      // 
      // openFileDialog
      // 
      this.openFileDialog.DefaultExt = "rflog";
      this.openFileDialog.Filter = "RfSuit Log|*.rflog|All files|*.*";
      // 
      // MainForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(633, 427);
      this.Controls.Add(this.splitContainer);
      this.Controls.Add(this.trackBar);
      this.Controls.Add(this.nextButton);
      this.Controls.Add(this.previousButton);
      this.Controls.Add(this.playButton);
      this.Controls.Add(this.menuStrip);
      this.MainMenuStrip = this.menuStrip;
      this.Name = "MainForm";
      this.Text = "RfSuit Player";
      this.menuStrip.ResumeLayout(false);
      this.menuStrip.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
      this.splitContainer.Panel1.ResumeLayout(false);
      this.splitContainer.Panel1.PerformLayout();
      this.splitContainer.Panel2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).EndInit();
      this.splitContainer.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.trackBar)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.MenuStrip menuStrip;
    private System.Windows.Forms.PictureBox pictureBox;
    private System.Windows.Forms.SplitContainer splitContainer;
    private System.Windows.Forms.PropertyGrid propertyGrid;
    private System.Windows.Forms.TrackBar trackBar;
    private System.Windows.Forms.Button nextButton;
    private System.Windows.Forms.Button previousButton;
    private System.Windows.Forms.Button playButton;
    private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
    private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
    private System.Windows.Forms.OpenFileDialog openFileDialog;

  }
}

