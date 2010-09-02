namespace RfSuitLogger
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
      this.startButton = new System.Windows.Forms.Button();
      this.previewPictureBox = new System.Windows.Forms.PictureBox();
      this.logTextBox = new System.Windows.Forms.TextBox();
      this.captureDevicesComboBox = new System.Windows.Forms.ComboBox();
      this.refreshButton = new System.Windows.Forms.Button();
      this.videoCapabilitiesComboBox = new System.Windows.Forms.ComboBox();
      ((System.ComponentModel.ISupportInitialize)(this.previewPictureBox)).BeginInit();
      this.SuspendLayout();
      // 
      // startButton
      // 
      this.startButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.startButton.Location = new System.Drawing.Point(12, 95);
      this.startButton.Name = "startButton";
      this.startButton.Size = new System.Drawing.Size(222, 157);
      this.startButton.TabIndex = 0;
      this.startButton.Text = "Start";
      this.startButton.UseVisualStyleBackColor = true;
      this.startButton.Click += new System.EventHandler(this.startButton_Click);
      // 
      // previewPictureBox
      // 
      this.previewPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this.previewPictureBox.Location = new System.Drawing.Point(240, 12);
      this.previewPictureBox.Name = "previewPictureBox";
      this.previewPictureBox.Size = new System.Drawing.Size(320, 240);
      this.previewPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
      this.previewPictureBox.TabIndex = 1;
      this.previewPictureBox.TabStop = false;
      // 
      // logTextBox
      // 
      this.logTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.logTextBox.Location = new System.Drawing.Point(12, 258);
      this.logTextBox.Multiline = true;
      this.logTextBox.Name = "logTextBox";
      this.logTextBox.Size = new System.Drawing.Size(548, 251);
      this.logTextBox.TabIndex = 2;
      // 
      // captureDevicesComboBox
      // 
      this.captureDevicesComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.captureDevicesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.captureDevicesComboBox.FormattingEnabled = true;
      this.captureDevicesComboBox.Location = new System.Drawing.Point(12, 41);
      this.captureDevicesComboBox.Name = "captureDevicesComboBox";
      this.captureDevicesComboBox.Size = new System.Drawing.Size(222, 21);
      this.captureDevicesComboBox.TabIndex = 3;
      this.captureDevicesComboBox.SelectedIndexChanged += new System.EventHandler(this.captureDevicesComboBox_SelectedIndexChanged);
      // 
      // refreshButton
      // 
      this.refreshButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.refreshButton.Location = new System.Drawing.Point(12, 12);
      this.refreshButton.Name = "refreshButton";
      this.refreshButton.Size = new System.Drawing.Size(222, 23);
      this.refreshButton.TabIndex = 4;
      this.refreshButton.Text = "Refresh Capture Devices";
      this.refreshButton.UseVisualStyleBackColor = true;
      this.refreshButton.Click += new System.EventHandler(this.refreshButton_Click);
      // 
      // videoCapabilitiesComboBox
      // 
      this.videoCapabilitiesComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.videoCapabilitiesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.videoCapabilitiesComboBox.FormattingEnabled = true;
      this.videoCapabilitiesComboBox.Location = new System.Drawing.Point(12, 68);
      this.videoCapabilitiesComboBox.Name = "videoCapabilitiesComboBox";
      this.videoCapabilitiesComboBox.Size = new System.Drawing.Size(222, 21);
      this.videoCapabilitiesComboBox.TabIndex = 5;
      this.videoCapabilitiesComboBox.SelectedIndexChanged += new System.EventHandler(this.videoCapabilitiesComboBox_SelectedIndexChanged);
      // 
      // MainForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(572, 521);
      this.Controls.Add(this.videoCapabilitiesComboBox);
      this.Controls.Add(this.refreshButton);
      this.Controls.Add(this.captureDevicesComboBox);
      this.Controls.Add(this.logTextBox);
      this.Controls.Add(this.previewPictureBox);
      this.Controls.Add(this.startButton);
      this.Name = "MainForm";
      this.Text = "RfSuitLogger";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.previewPictureBox)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Button startButton;
    private System.Windows.Forms.PictureBox previewPictureBox;
    private System.Windows.Forms.TextBox logTextBox;
    private System.Windows.Forms.ComboBox captureDevicesComboBox;
    private System.Windows.Forms.Button refreshButton;
    private System.Windows.Forms.ComboBox videoCapabilitiesComboBox;
  }
}

