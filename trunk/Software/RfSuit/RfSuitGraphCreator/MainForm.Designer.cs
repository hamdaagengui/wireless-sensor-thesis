namespace RfSuitGraphCreator
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
      this.label1 = new System.Windows.Forms.Label();
      this.statusLabel = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.processedLabel = new System.Windows.Forms.Label();
      this.timer = new System.Windows.Forms.Timer(this.components);
      this.mergeScenariosCheckBox = new System.Windows.Forms.CheckBox();
      this.absoluteRSSIRadioButton = new System.Windows.Forms.RadioButton();
      this.linkLossRadioButton = new System.Windows.Forms.RadioButton();
      this.SuspendLayout();
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(12, 9);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(40, 13);
      this.label1.TabIndex = 0;
      this.label1.Text = "Status:";
      // 
      // statusLabel
      // 
      this.statusLabel.AutoSize = true;
      this.statusLabel.Location = new System.Drawing.Point(59, 9);
      this.statusLabel.Name = "statusLabel";
      this.statusLabel.Size = new System.Drawing.Size(0, 13);
      this.statusLabel.TabIndex = 1;
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(12, 22);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(60, 13);
      this.label2.TabIndex = 2;
      this.label2.Text = "Processed:";
      // 
      // processedLabel
      // 
      this.processedLabel.AutoSize = true;
      this.processedLabel.Location = new System.Drawing.Point(78, 22);
      this.processedLabel.Name = "processedLabel";
      this.processedLabel.Size = new System.Drawing.Size(0, 13);
      this.processedLabel.TabIndex = 3;
      // 
      // timer
      // 
      this.timer.Enabled = true;
      this.timer.Tick += new System.EventHandler(this.TimerTick);
      // 
      // mergeScenariosCheckBox
      // 
      this.mergeScenariosCheckBox.AutoSize = true;
      this.mergeScenariosCheckBox.Location = new System.Drawing.Point(15, 39);
      this.mergeScenariosCheckBox.Name = "mergeScenariosCheckBox";
      this.mergeScenariosCheckBox.Size = new System.Drawing.Size(104, 17);
      this.mergeScenariosCheckBox.TabIndex = 4;
      this.mergeScenariosCheckBox.Text = "Merge scenarios";
      this.mergeScenariosCheckBox.UseVisualStyleBackColor = true;
      // 
      // absoluteRSSIRadioButton
      // 
      this.absoluteRSSIRadioButton.AutoSize = true;
      this.absoluteRSSIRadioButton.Checked = true;
      this.absoluteRSSIRadioButton.Location = new System.Drawing.Point(15, 62);
      this.absoluteRSSIRadioButton.Name = "absoluteRSSIRadioButton";
      this.absoluteRSSIRadioButton.Size = new System.Drawing.Size(94, 17);
      this.absoluteRSSIRadioButton.TabIndex = 5;
      this.absoluteRSSIRadioButton.TabStop = true;
      this.absoluteRSSIRadioButton.Text = "Absolute RSSI";
      this.absoluteRSSIRadioButton.UseVisualStyleBackColor = true;
      // 
      // linkLossRadioButton
      // 
      this.linkLossRadioButton.AutoSize = true;
      this.linkLossRadioButton.Location = new System.Drawing.Point(15, 85);
      this.linkLossRadioButton.Name = "linkLossRadioButton";
      this.linkLossRadioButton.Size = new System.Drawing.Size(70, 17);
      this.linkLossRadioButton.TabIndex = 6;
      this.linkLossRadioButton.Text = "Link Loss";
      this.linkLossRadioButton.UseVisualStyleBackColor = true;
      // 
      // MainForm
      // 
      this.AllowDrop = true;
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(259, 111);
      this.Controls.Add(this.linkLossRadioButton);
      this.Controls.Add(this.absoluteRSSIRadioButton);
      this.Controls.Add(this.mergeScenariosCheckBox);
      this.Controls.Add(this.processedLabel);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.statusLabel);
      this.Controls.Add(this.label1);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "MainForm";
      this.Text = "RfSuit Graph Creator";
      this.TopMost = true;
      this.Load += new System.EventHandler(this.MainForm_Load);
      this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainFormDragDrop);
      this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainFormDragEnter);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Label statusLabel;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label processedLabel;
    private System.Windows.Forms.Timer timer;
    private System.Windows.Forms.CheckBox mergeScenariosCheckBox;
    private System.Windows.Forms.RadioButton absoluteRSSIRadioButton;
    private System.Windows.Forms.RadioButton linkLossRadioButton;


  }
}

