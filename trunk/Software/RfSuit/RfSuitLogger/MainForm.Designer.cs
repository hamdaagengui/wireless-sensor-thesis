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
      this.previewPictureBox = new System.Windows.Forms.PictureBox();
      this.logTextBox = new System.Windows.Forms.TextBox();
      this.startLogButton = new System.Windows.Forms.Button();
      this.stopLogButton = new System.Windows.Forms.Button();
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this.propertiesButton = new System.Windows.Forms.Button();
      this.videoCapabilitiesComboBox = new System.Windows.Forms.ComboBox();
      this.refreshButton = new System.Windows.Forms.Button();
      this.captureDevicesComboBox = new System.Windows.Forms.ComboBox();
      this.groupBox3 = new System.Windows.Forms.GroupBox();
      this.channelComboBox = new System.Windows.Forms.ComboBox();
      this.powerComboBox = new System.Windows.Forms.ComboBox();
      this.serialPortsComboBox = new System.Windows.Forms.ComboBox();
      ((System.ComponentModel.ISupportInitialize)(this.previewPictureBox)).BeginInit();
      this.groupBox1.SuspendLayout();
      this.groupBox2.SuspendLayout();
      this.groupBox3.SuspendLayout();
      this.SuspendLayout();
      // 
      // previewPictureBox
      // 
      this.previewPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.previewPictureBox.Location = new System.Drawing.Point(172, 12);
      this.previewPictureBox.Name = "previewPictureBox";
      this.previewPictureBox.Size = new System.Drawing.Size(606, 411);
      this.previewPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
      this.previewPictureBox.TabIndex = 1;
      this.previewPictureBox.TabStop = false;
      // 
      // logTextBox
      // 
      this.logTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.logTextBox.Location = new System.Drawing.Point(12, 347);
      this.logTextBox.Multiline = true;
      this.logTextBox.Name = "logTextBox";
      this.logTextBox.Size = new System.Drawing.Size(700, 188);
      this.logTextBox.TabIndex = 2;
      // 
      // startLogButton
      // 
      this.startLogButton.Location = new System.Drawing.Point(6, 19);
      this.startLogButton.Name = "startLogButton";
      this.startLogButton.Size = new System.Drawing.Size(141, 23);
      this.startLogButton.TabIndex = 7;
      this.startLogButton.Text = "Start";
      this.startLogButton.UseVisualStyleBackColor = true;
      this.startLogButton.Click += new System.EventHandler(this.StartLogButtonClick);
      // 
      // stopLogButton
      // 
      this.stopLogButton.Location = new System.Drawing.Point(6, 48);
      this.stopLogButton.Name = "stopLogButton";
      this.stopLogButton.Size = new System.Drawing.Size(141, 23);
      this.stopLogButton.TabIndex = 8;
      this.stopLogButton.Text = "Stop";
      this.stopLogButton.UseVisualStyleBackColor = true;
      this.stopLogButton.Click += new System.EventHandler(this.StopLogButtonClick);
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this.startLogButton);
      this.groupBox1.Controls.Add(this.stopLogButton);
      this.groupBox1.Location = new System.Drawing.Point(12, 153);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(154, 78);
      this.groupBox1.TabIndex = 9;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Logging";
      // 
      // groupBox2
      // 
      this.groupBox2.Controls.Add(this.propertiesButton);
      this.groupBox2.Controls.Add(this.videoCapabilitiesComboBox);
      this.groupBox2.Controls.Add(this.refreshButton);
      this.groupBox2.Controls.Add(this.captureDevicesComboBox);
      this.groupBox2.Location = new System.Drawing.Point(12, 12);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(154, 135);
      this.groupBox2.TabIndex = 10;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Camera";
      // 
      // propertiesButton
      // 
      this.propertiesButton.Location = new System.Drawing.Point(6, 102);
      this.propertiesButton.Name = "propertiesButton";
      this.propertiesButton.Size = new System.Drawing.Size(141, 23);
      this.propertiesButton.TabIndex = 10;
      this.propertiesButton.Text = "Properties";
      this.propertiesButton.UseVisualStyleBackColor = true;
      this.propertiesButton.Click += new System.EventHandler(this.PropertiesButtonClick);
      // 
      // videoCapabilitiesComboBox
      // 
      this.videoCapabilitiesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.videoCapabilitiesComboBox.FormattingEnabled = true;
      this.videoCapabilitiesComboBox.Location = new System.Drawing.Point(6, 75);
      this.videoCapabilitiesComboBox.Name = "videoCapabilitiesComboBox";
      this.videoCapabilitiesComboBox.Size = new System.Drawing.Size(141, 21);
      this.videoCapabilitiesComboBox.TabIndex = 9;
      this.videoCapabilitiesComboBox.SelectedIndexChanged += new System.EventHandler(this.VideoCapabilitiesComboBoxSelectedIndexChanged);
      // 
      // refreshButton
      // 
      this.refreshButton.Location = new System.Drawing.Point(6, 19);
      this.refreshButton.Name = "refreshButton";
      this.refreshButton.Size = new System.Drawing.Size(141, 23);
      this.refreshButton.TabIndex = 8;
      this.refreshButton.Text = "Refresh";
      this.refreshButton.UseVisualStyleBackColor = true;
      this.refreshButton.Click += new System.EventHandler(this.RefreshButtonClick);
      // 
      // captureDevicesComboBox
      // 
      this.captureDevicesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.captureDevicesComboBox.FormattingEnabled = true;
      this.captureDevicesComboBox.Location = new System.Drawing.Point(6, 48);
      this.captureDevicesComboBox.Name = "captureDevicesComboBox";
      this.captureDevicesComboBox.Size = new System.Drawing.Size(141, 21);
      this.captureDevicesComboBox.TabIndex = 7;
      this.captureDevicesComboBox.SelectedIndexChanged += new System.EventHandler(this.CaptureDevicesComboBoxSelectedIndexChanged);
      this.captureDevicesComboBox.Click += new System.EventHandler(this.CaptureDevicesComboBoxSelectedIndexChanged);
      // 
      // groupBox3
      // 
      this.groupBox3.Controls.Add(this.channelComboBox);
      this.groupBox3.Controls.Add(this.powerComboBox);
      this.groupBox3.Controls.Add(this.serialPortsComboBox);
      this.groupBox3.Location = new System.Drawing.Point(12, 237);
      this.groupBox3.Name = "groupBox3";
      this.groupBox3.Size = new System.Drawing.Size(154, 90);
      this.groupBox3.TabIndex = 11;
      this.groupBox3.TabStop = false;
      this.groupBox3.Text = "Connection";
      // 
      // channelComboBox
      // 
      this.channelComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.channelComboBox.FormattingEnabled = true;
      this.channelComboBox.Items.AddRange(new object[] {
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26"});
      this.channelComboBox.Location = new System.Drawing.Point(6, 73);
      this.channelComboBox.Name = "channelComboBox";
      this.channelComboBox.Size = new System.Drawing.Size(121, 21);
      this.channelComboBox.TabIndex = 2;
      this.channelComboBox.SelectionChangeCommitted += new System.EventHandler(this.ChannelComboBoxSelectionChangeCommitted);
      // 
      // powerComboBox
      // 
      this.powerComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.powerComboBox.FormattingEnabled = true;
      this.powerComboBox.Items.AddRange(new object[] {
            "3 dBm",
            "2.8 dBm",
            "2.3 dBm",
            "1.8 dBm",
            "1.3 dBm",
            "0.7 dBm",
            "0 dBm",
            "-1 dBm",
            "-2 dBm",
            "-3 dBm",
            "-4 dBm",
            "-5 dBm",
            "-7 dBm",
            "-9 dBm",
            "-12 dBm",
            "-17 dBm"});
      this.powerComboBox.Location = new System.Drawing.Point(7, 46);
      this.powerComboBox.Name = "powerComboBox";
      this.powerComboBox.Size = new System.Drawing.Size(121, 21);
      this.powerComboBox.TabIndex = 1;
      this.powerComboBox.SelectionChangeCommitted += new System.EventHandler(this.PowerComboBoxSelectionChangeCommitted);
      // 
      // serialPortsComboBox
      // 
      this.serialPortsComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.serialPortsComboBox.FormattingEnabled = true;
      this.serialPortsComboBox.Location = new System.Drawing.Point(6, 19);
      this.serialPortsComboBox.Name = "serialPortsComboBox";
      this.serialPortsComboBox.Size = new System.Drawing.Size(141, 21);
      this.serialPortsComboBox.TabIndex = 0;
      this.serialPortsComboBox.DropDown += new System.EventHandler(this.SerialPortsComboBoxDropDown);
      // 
      // MainForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(724, 547);
      this.Controls.Add(this.groupBox3);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.groupBox1);
      this.Controls.Add(this.logTextBox);
      this.Controls.Add(this.previewPictureBox);
      this.Name = "MainForm";
      this.Text = "RfSuitLogger";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.previewPictureBox)).EndInit();
      this.groupBox1.ResumeLayout(false);
      this.groupBox2.ResumeLayout(false);
      this.groupBox3.ResumeLayout(false);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.PictureBox previewPictureBox;
    private System.Windows.Forms.TextBox logTextBox;
    private System.Windows.Forms.Button startLogButton;
    private System.Windows.Forms.Button stopLogButton;
    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.Button propertiesButton;
    private System.Windows.Forms.ComboBox videoCapabilitiesComboBox;
    private System.Windows.Forms.Button refreshButton;
    private System.Windows.Forms.ComboBox captureDevicesComboBox;
    private System.Windows.Forms.GroupBox groupBox3;
    private System.Windows.Forms.ComboBox serialPortsComboBox;
    private System.Windows.Forms.ComboBox channelComboBox;
    private System.Windows.Forms.ComboBox powerComboBox;
  }
}

