namespace RfSuitLive
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
      this.components = new System.ComponentModel.Container();
      this.comPortsComboBox = new System.Windows.Forms.ComboBox();
      this.channelComboBox = new System.Windows.Forms.ComboBox();
      this.powerComboBox = new System.Windows.Forms.ComboBox();
      this.connectButton = new System.Windows.Forms.Button();
      this.zedGraphControl = new ZedGraph.ZedGraphControl();
      this.timer = new System.Windows.Forms.Timer(this.components);
      this.SuspendLayout();
      // 
      // comPortsComboBox
      // 
      this.comPortsComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comPortsComboBox.FormattingEnabled = true;
      this.comPortsComboBox.Location = new System.Drawing.Point(12, 12);
      this.comPortsComboBox.Name = "comPortsComboBox";
      this.comPortsComboBox.Size = new System.Drawing.Size(140, 21);
      this.comPortsComboBox.TabIndex = 0;
      this.comPortsComboBox.DropDown += new System.EventHandler(this.ComPortsComboBoxDropDown);
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
      this.channelComboBox.Location = new System.Drawing.Point(304, 12);
      this.channelComboBox.Name = "channelComboBox";
      this.channelComboBox.Size = new System.Drawing.Size(141, 21);
      this.channelComboBox.TabIndex = 4;
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
      this.powerComboBox.Location = new System.Drawing.Point(158, 12);
      this.powerComboBox.Name = "powerComboBox";
      this.powerComboBox.Size = new System.Drawing.Size(140, 21);
      this.powerComboBox.TabIndex = 3;
      // 
      // connectButton
      // 
      this.connectButton.Location = new System.Drawing.Point(451, 10);
      this.connectButton.Name = "connectButton";
      this.connectButton.Size = new System.Drawing.Size(141, 23);
      this.connectButton.TabIndex = 5;
      this.connectButton.Text = "Connect";
      this.connectButton.UseVisualStyleBackColor = true;
      this.connectButton.Click += new System.EventHandler(this.ConnectButtonClick);
      // 
      // zedGraphControl
      // 
      this.zedGraphControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.zedGraphControl.Location = new System.Drawing.Point(12, 39);
      this.zedGraphControl.Name = "zedGraphControl";
      this.zedGraphControl.ScrollGrace = 0D;
      this.zedGraphControl.ScrollMaxX = 0D;
      this.zedGraphControl.ScrollMaxY = 0D;
      this.zedGraphControl.ScrollMaxY2 = 0D;
      this.zedGraphControl.ScrollMinX = 0D;
      this.zedGraphControl.ScrollMinY = 0D;
      this.zedGraphControl.ScrollMinY2 = 0D;
      this.zedGraphControl.Size = new System.Drawing.Size(833, 468);
      this.zedGraphControl.TabIndex = 6;
      // 
      // timer
      // 
      this.timer.Enabled = true;
      this.timer.Interval = 33;
      this.timer.Tick += new System.EventHandler(this.TimerTick);
      // 
      // MainForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(857, 519);
      this.Controls.Add(this.zedGraphControl);
      this.Controls.Add(this.connectButton);
      this.Controls.Add(this.channelComboBox);
      this.Controls.Add(this.powerComboBox);
      this.Controls.Add(this.comPortsComboBox);
      this.Name = "MainForm";
      this.Text = "RfSuit Live";
      this.Load += new System.EventHandler(this.MainForm_Load);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.ComboBox comPortsComboBox;
    private System.Windows.Forms.ComboBox channelComboBox;
    private System.Windows.Forms.ComboBox powerComboBox;
    private System.Windows.Forms.Button connectButton;
    private ZedGraph.ZedGraphControl zedGraphControl;
    private System.Windows.Forms.Timer timer;
  }
}

