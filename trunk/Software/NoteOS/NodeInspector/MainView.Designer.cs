namespace NodeInspector
{
	partial class MainView
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainView));
			this.checkBoxShowFramework = new System.Windows.Forms.CheckBox();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.checkBoxShowApplication = new System.Windows.Forms.CheckBox();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.checkBoxShowErrors = new System.Windows.Forms.CheckBox();
			this.checkBoxShowWarnings = new System.Windows.Forms.CheckBox();
			this.checkBoxShowMessages = new System.Windows.Forms.CheckBox();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.comboBoxPort = new System.Windows.Forms.ComboBox();
			this.comboBoxBaudrate = new System.Windows.Forms.ComboBox();
			this.checkBoxRun = new System.Windows.Forms.CheckBox();
			this.checkBoxTrackNewest = new System.Windows.Forms.CheckBox();
			this.checkBoxDeltaTime = new System.Windows.Forms.CheckBox();
			this.listViewLogs = new NodeInspector.ListViewNoFlicker();
			this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.SuspendLayout();
			// 
			// checkBoxShowFramework
			// 
			this.checkBoxShowFramework.Checked = true;
			this.checkBoxShowFramework.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowFramework.ImageList = this.imageList1;
			this.checkBoxShowFramework.ImeMode = System.Windows.Forms.ImeMode.NoControl;
			this.checkBoxShowFramework.Location = new System.Drawing.Point(75, 93);
			this.checkBoxShowFramework.Name = "checkBoxShowFramework";
			this.checkBoxShowFramework.Size = new System.Drawing.Size(40, 32);
			this.checkBoxShowFramework.TabIndex = 18;
			this.checkBoxShowFramework.UseVisualStyleBackColor = true;
			this.checkBoxShowFramework.Visible = false;
			// 
			// imageList1
			// 
			this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
			this.imageList1.ImageSize = new System.Drawing.Size(18, 18);
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// checkBoxShowApplication
			// 
			this.checkBoxShowApplication.Checked = true;
			this.checkBoxShowApplication.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowApplication.ImageList = this.imageList1;
			this.checkBoxShowApplication.ImeMode = System.Windows.Forms.ImeMode.NoControl;
			this.checkBoxShowApplication.Location = new System.Drawing.Point(29, 93);
			this.checkBoxShowApplication.Name = "checkBoxShowApplication";
			this.checkBoxShowApplication.Size = new System.Drawing.Size(40, 32);
			this.checkBoxShowApplication.TabIndex = 17;
			this.checkBoxShowApplication.UseVisualStyleBackColor = true;
			this.checkBoxShowApplication.Visible = false;
			// 
			// textBox1
			// 
			this.textBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
									| System.Windows.Forms.AnchorStyles.Right)));
			this.textBox1.Location = new System.Drawing.Point(146, 99);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(328, 20);
			this.textBox1.TabIndex = 16;
			this.textBox1.Visible = false;
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.button1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
			this.button1.Location = new System.Drawing.Point(480, 97);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(39, 23);
			this.button1.TabIndex = 15;
			this.button1.Text = "Filter";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Visible = false;
			// 
			// checkBoxShowErrors
			// 
			this.checkBoxShowErrors.Checked = true;
			this.checkBoxShowErrors.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowErrors.ImageList = this.imageList1;
			this.checkBoxShowErrors.ImeMode = System.Windows.Forms.ImeMode.NoControl;
			this.checkBoxShowErrors.Location = new System.Drawing.Point(104, 12);
			this.checkBoxShowErrors.Name = "checkBoxShowErrors";
			this.checkBoxShowErrors.Size = new System.Drawing.Size(40, 32);
			this.checkBoxShowErrors.TabIndex = 14;
			this.checkBoxShowErrors.UseVisualStyleBackColor = true;
			this.checkBoxShowErrors.CheckedChanged += new System.EventHandler(this.LevelCheckboxesChanged);
			// 
			// checkBoxShowWarnings
			// 
			this.checkBoxShowWarnings.Checked = true;
			this.checkBoxShowWarnings.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowWarnings.ImageList = this.imageList1;
			this.checkBoxShowWarnings.ImeMode = System.Windows.Forms.ImeMode.NoControl;
			this.checkBoxShowWarnings.Location = new System.Drawing.Point(58, 12);
			this.checkBoxShowWarnings.Name = "checkBoxShowWarnings";
			this.checkBoxShowWarnings.Size = new System.Drawing.Size(40, 32);
			this.checkBoxShowWarnings.TabIndex = 13;
			this.checkBoxShowWarnings.UseVisualStyleBackColor = true;
			this.checkBoxShowWarnings.CheckedChanged += new System.EventHandler(this.LevelCheckboxesChanged);
			// 
			// checkBoxShowMessages
			// 
			this.checkBoxShowMessages.Checked = true;
			this.checkBoxShowMessages.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowMessages.ImageList = this.imageList1;
			this.checkBoxShowMessages.ImeMode = System.Windows.Forms.ImeMode.NoControl;
			this.checkBoxShowMessages.Location = new System.Drawing.Point(12, 12);
			this.checkBoxShowMessages.Name = "checkBoxShowMessages";
			this.checkBoxShowMessages.Size = new System.Drawing.Size(40, 32);
			this.checkBoxShowMessages.TabIndex = 12;
			this.checkBoxShowMessages.UseVisualStyleBackColor = true;
			this.checkBoxShowMessages.CheckedChanged += new System.EventHandler(this.LevelCheckboxesChanged);
			// 
			// timer1
			// 
			this.timer1.Enabled = true;
			this.timer1.Interval = 1000;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// comboBoxPort
			// 
			this.comboBoxPort.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.comboBoxPort.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::NodeInspector.Properties.Settings.Default, "Port", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.comboBoxPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxPort.FormattingEnabled = true;
			this.comboBoxPort.Location = new System.Drawing.Point(490, 18);
			this.comboBoxPort.Name = "comboBoxPort";
			this.comboBoxPort.Size = new System.Drawing.Size(173, 21);
			this.comboBoxPort.TabIndex = 20;
			this.comboBoxPort.Text = global::NodeInspector.Properties.Settings.Default.Port;
			// 
			// comboBoxBaudrate
			// 
			this.comboBoxBaudrate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.comboBoxBaudrate.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::NodeInspector.Properties.Settings.Default, "Baudrate", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.comboBoxBaudrate.FormattingEnabled = true;
			this.comboBoxBaudrate.Items.AddRange(new object[] {
            "9600",
            "38400",
            "57600",
            "115200",
            "250000",
            "500000",
            "1000000"});
			this.comboBoxBaudrate.Location = new System.Drawing.Point(669, 18);
			this.comboBoxBaudrate.Name = "comboBoxBaudrate";
			this.comboBoxBaudrate.Size = new System.Drawing.Size(121, 21);
			this.comboBoxBaudrate.TabIndex = 21;
			this.comboBoxBaudrate.Text = global::NodeInspector.Properties.Settings.Default.Baudrate;
			// 
			// checkBoxRun
			// 
			this.checkBoxRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.checkBoxRun.AutoSize = true;
			this.checkBoxRun.Checked = global::NodeInspector.Properties.Settings.Default.RunState;
			this.checkBoxRun.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxRun.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::NodeInspector.Properties.Settings.Default, "RunState", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.checkBoxRun.Location = new System.Drawing.Point(796, 20);
			this.checkBoxRun.Name = "checkBoxRun";
			this.checkBoxRun.Size = new System.Drawing.Size(46, 17);
			this.checkBoxRun.TabIndex = 22;
			this.checkBoxRun.Text = "Run";
			this.checkBoxRun.UseVisualStyleBackColor = true;
			this.checkBoxRun.CheckedChanged += new System.EventHandler(this.checkBoxRun_CheckedChanged);
			// 
			// checkBoxTrackNewest
			// 
			this.checkBoxTrackNewest.AutoSize = true;
			this.checkBoxTrackNewest.Checked = global::NodeInspector.Properties.Settings.Default.TrackNewest;
			this.checkBoxTrackNewest.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxTrackNewest.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::NodeInspector.Properties.Settings.Default, "TrackNewest", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.checkBoxTrackNewest.Location = new System.Drawing.Point(150, 20);
			this.checkBoxTrackNewest.Name = "checkBoxTrackNewest";
			this.checkBoxTrackNewest.Size = new System.Drawing.Size(91, 17);
			this.checkBoxTrackNewest.TabIndex = 23;
			this.checkBoxTrackNewest.Text = "Track newest";
			this.checkBoxTrackNewest.UseVisualStyleBackColor = true;
			// 
			// checkBoxDeltaTime
			// 
			this.checkBoxDeltaTime.AutoSize = true;
			this.checkBoxDeltaTime.Checked = global::NodeInspector.Properties.Settings.Default.DeltaTime;
			this.checkBoxDeltaTime.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::NodeInspector.Properties.Settings.Default, "DeltaTime", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.checkBoxDeltaTime.Location = new System.Drawing.Point(247, 20);
			this.checkBoxDeltaTime.Name = "checkBoxDeltaTime";
			this.checkBoxDeltaTime.Size = new System.Drawing.Size(73, 17);
			this.checkBoxDeltaTime.TabIndex = 24;
			this.checkBoxDeltaTime.Text = "Delta time";
			this.checkBoxDeltaTime.UseVisualStyleBackColor = true;
			this.checkBoxDeltaTime.Visible = false;
			// 
			// listViewLogs
			// 
			this.listViewLogs.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
									| System.Windows.Forms.AnchorStyles.Left)
									| System.Windows.Forms.AnchorStyles.Right)));
			this.listViewLogs.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader6,
            this.columnHeader4});
			this.listViewLogs.FullRowSelect = true;
			this.listViewLogs.Location = new System.Drawing.Point(12, 50);
			this.listViewLogs.Name = "listViewLogs";
			this.listViewLogs.Size = new System.Drawing.Size(830, 542);
			this.listViewLogs.SmallImageList = this.imageList1;
			this.listViewLogs.TabIndex = 19;
			this.listViewLogs.UseCompatibleStateImageBehavior = false;
			this.listViewLogs.View = System.Windows.Forms.View.Details;
			this.listViewLogs.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewLogs_MouseDoubleClick);
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Type";
			this.columnHeader5.Width = 40;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Number";
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Time";
			this.columnHeader2.Width = 110;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Subsystem";
			this.columnHeader3.Width = 100;
			// 
			// columnHeader6
			// 
			this.columnHeader6.Text = "Module";
			this.columnHeader6.Width = 140;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Details";
			this.columnHeader4.Width = 600;
			// 
			// MainView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(854, 604);
			this.Controls.Add(this.checkBoxDeltaTime);
			this.Controls.Add(this.checkBoxTrackNewest);
			this.Controls.Add(this.checkBoxRun);
			this.Controls.Add(this.comboBoxBaudrate);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.checkBoxShowFramework);
			this.Controls.Add(this.checkBoxShowApplication);
			this.Controls.Add(this.comboBoxPort);
			this.Controls.Add(this.listViewLogs);
			this.Controls.Add(this.checkBoxShowErrors);
			this.Controls.Add(this.checkBoxShowWarnings);
			this.Controls.Add(this.checkBoxShowMessages);
			this.DataBindings.Add(new System.Windows.Forms.Binding("Location", global::NodeInspector.Properties.Settings.Default, "MainViewLocation", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.DataBindings.Add(new System.Windows.Forms.Binding("WindowState", global::NodeInspector.Properties.Settings.Default, "MainViewWindowState", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Location = global::NodeInspector.Properties.Settings.Default.MainViewLocation;
			this.MinimumSize = new System.Drawing.Size(708, 260);
			this.Name = "MainView";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Node Inspector";
			this.WindowState = global::NodeInspector.Properties.Settings.Default.MainViewWindowState;
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainView_FormClosing);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.CheckBox checkBoxShowFramework;
		private System.Windows.Forms.CheckBox checkBoxShowApplication;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.CheckBox checkBoxShowErrors;
		private System.Windows.Forms.CheckBox checkBoxShowWarnings;
		private System.Windows.Forms.CheckBox checkBoxShowMessages;
		private ListViewNoFlicker listViewLogs;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.ComboBox comboBoxPort;
		private System.Windows.Forms.ComboBox comboBoxBaudrate;
		private System.Windows.Forms.CheckBox checkBoxRun;
		private System.Windows.Forms.CheckBox checkBoxTrackNewest;
		private System.Windows.Forms.CheckBox checkBoxDeltaTime;
	}
}

