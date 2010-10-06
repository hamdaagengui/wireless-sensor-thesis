﻿namespace RfSuitGraphCreator
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
      this.label1 = new System.Windows.Forms.Label();
      this.statusLabel = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.queueLabel = new System.Windows.Forms.Label();
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
      this.statusLabel.Location = new System.Drawing.Point(60, 9);
      this.statusLabel.Name = "statusLabel";
      this.statusLabel.Size = new System.Drawing.Size(0, 13);
      this.statusLabel.TabIndex = 1;
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(12, 31);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(42, 13);
      this.label2.TabIndex = 2;
      this.label2.Text = "Queue:";
      // 
      // queueLabel
      // 
      this.queueLabel.AutoSize = true;
      this.queueLabel.Location = new System.Drawing.Point(60, 31);
      this.queueLabel.Name = "queueLabel";
      this.queueLabel.Size = new System.Drawing.Size(0, 13);
      this.queueLabel.TabIndex = 3;
      // 
      // MainForm
      // 
      this.AllowDrop = true;
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(296, 57);
      this.Controls.Add(this.queueLabel);
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
    private System.Windows.Forms.Label queueLabel;


  }
}

