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
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
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
      this.serialPortsComboBox = new System.Windows.Forms.ComboBox();
      ((System.ComponentModel.ISupportInitialize)(this.previewPictureBox)).BeginInit();
      this.groupBox1.SuspendLayout();
      this.groupBox2.SuspendLayout();
      this.groupBox3.SuspendLayout();
      this.SuspendLayout();
      // 
      // previewPictureBox
      // 
      resources.ApplyResources(this.previewPictureBox, "previewPictureBox");
      this.previewPictureBox.Name = "previewPictureBox";
      this.previewPictureBox.TabStop = false;
      // 
      // logTextBox
      // 
      resources.ApplyResources(this.logTextBox, "logTextBox");
      this.logTextBox.Name = "logTextBox";
      // 
      // startLogButton
      // 
      resources.ApplyResources(this.startLogButton, "startLogButton");
      this.startLogButton.Name = "startLogButton";
      this.startLogButton.UseVisualStyleBackColor = true;
      this.startLogButton.Click += new System.EventHandler(this.StartLogButtonClick);
      // 
      // stopLogButton
      // 
      resources.ApplyResources(this.stopLogButton, "stopLogButton");
      this.stopLogButton.Name = "stopLogButton";
      this.stopLogButton.UseVisualStyleBackColor = true;
      this.stopLogButton.Click += new System.EventHandler(this.StopLogButtonClick);
      // 
      // groupBox1
      // 
      resources.ApplyResources(this.groupBox1, "groupBox1");
      this.groupBox1.Controls.Add(this.startLogButton);
      this.groupBox1.Controls.Add(this.stopLogButton);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.TabStop = false;
      // 
      // groupBox2
      // 
      resources.ApplyResources(this.groupBox2, "groupBox2");
      this.groupBox2.Controls.Add(this.propertiesButton);
      this.groupBox2.Controls.Add(this.videoCapabilitiesComboBox);
      this.groupBox2.Controls.Add(this.refreshButton);
      this.groupBox2.Controls.Add(this.captureDevicesComboBox);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.TabStop = false;
      // 
      // propertiesButton
      // 
      resources.ApplyResources(this.propertiesButton, "propertiesButton");
      this.propertiesButton.Name = "propertiesButton";
      this.propertiesButton.UseVisualStyleBackColor = true;
      this.propertiesButton.Click += new System.EventHandler(this.PropertiesButtonClick);
      // 
      // videoCapabilitiesComboBox
      // 
      resources.ApplyResources(this.videoCapabilitiesComboBox, "videoCapabilitiesComboBox");
      this.videoCapabilitiesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.videoCapabilitiesComboBox.FormattingEnabled = true;
      this.videoCapabilitiesComboBox.Name = "videoCapabilitiesComboBox";
      this.videoCapabilitiesComboBox.SelectedIndexChanged += new System.EventHandler(this.VideoCapabilitiesComboBoxSelectedIndexChanged);
      // 
      // refreshButton
      // 
      resources.ApplyResources(this.refreshButton, "refreshButton");
      this.refreshButton.Name = "refreshButton";
      this.refreshButton.UseVisualStyleBackColor = true;
      this.refreshButton.Click += new System.EventHandler(this.RefreshButtonClick);
      // 
      // captureDevicesComboBox
      // 
      resources.ApplyResources(this.captureDevicesComboBox, "captureDevicesComboBox");
      this.captureDevicesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.captureDevicesComboBox.FormattingEnabled = true;
      this.captureDevicesComboBox.Name = "captureDevicesComboBox";
      this.captureDevicesComboBox.SelectedIndexChanged += new System.EventHandler(this.CaptureDevicesComboBoxSelectedIndexChanged);
      this.captureDevicesComboBox.Click += new System.EventHandler(this.CaptureDevicesComboBoxSelectedIndexChanged);
      // 
      // groupBox3
      // 
      resources.ApplyResources(this.groupBox3, "groupBox3");
      this.groupBox3.Controls.Add(this.serialPortsComboBox);
      this.groupBox3.Name = "groupBox3";
      this.groupBox3.TabStop = false;
      // 
      // serialPortsComboBox
      // 
      resources.ApplyResources(this.serialPortsComboBox, "serialPortsComboBox");
      this.serialPortsComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.serialPortsComboBox.FormattingEnabled = true;
      this.serialPortsComboBox.Name = "serialPortsComboBox";
      this.serialPortsComboBox.DropDown += new System.EventHandler(this.SerialPortsComboBoxDropDown);
      // 
      // MainForm
      // 
      resources.ApplyResources(this, "$this");
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.groupBox3);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.groupBox1);
      this.Controls.Add(this.logTextBox);
      this.Controls.Add(this.previewPictureBox);
      this.Name = "MainForm";
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
  }
}

