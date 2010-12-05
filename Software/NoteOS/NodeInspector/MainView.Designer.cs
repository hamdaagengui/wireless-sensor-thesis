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
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.splitContainer2 = new System.Windows.Forms.SplitContainer();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.buttonReset = new System.Windows.Forms.Button();
			this.checkBoxDeltaTime = new System.Windows.Forms.CheckBox();
			this.checkBoxTrackNewest = new System.Windows.Forms.CheckBox();
			this.checkBoxRun = new System.Windows.Forms.CheckBox();
			this.comboBoxBaudrate = new System.Windows.Forms.ComboBox();
			this.comboBoxPort = new System.Windows.Forms.ComboBox();
			this.checkBoxShowErrors = new System.Windows.Forms.CheckBox();
			this.checkBoxShowWarnings = new System.Windows.Forms.CheckBox();
			this.checkBoxShowMessages = new System.Windows.Forms.CheckBox();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.listViewLogs = new NodeInspector.ListViewNoFlicker();
			this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.dataGridView1 = new System.Windows.Forms.DataGridView();
			this.tabPage2 = new System.Windows.Forms.TabPage();
			this.splitContainer3 = new System.Windows.Forms.SplitContainer();
			this.buttonClearTerminal = new System.Windows.Forms.Button();
			this.richTextBoxTerminal = new System.Windows.Forms.RichTextBox();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
			this.splitContainer2.Panel1.SuspendLayout();
			this.splitContainer2.Panel2.SuspendLayout();
			this.splitContainer2.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.tabPage1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
			this.tabPage2.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
			this.splitContainer3.Panel1.SuspendLayout();
			this.splitContainer3.Panel2.SuspendLayout();
			this.splitContainer3.SuspendLayout();
			this.SuspendLayout();
			// 
			// imageList1
			// 
			this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
			this.imageList1.ImageSize = new System.Drawing.Size(18, 18);
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// timer1
			// 
			this.timer1.Enabled = true;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// splitContainer2
			// 
			this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer2.IsSplitterFixed = true;
			this.splitContainer2.Location = new System.Drawing.Point(0, 0);
			this.splitContainer2.Name = "splitContainer2";
			this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this.splitContainer2.Panel1.Controls.Add(this.textBox1);
			this.splitContainer2.Panel1.Controls.Add(this.buttonReset);
			this.splitContainer2.Panel1.Controls.Add(this.checkBoxDeltaTime);
			this.splitContainer2.Panel1.Controls.Add(this.checkBoxTrackNewest);
			this.splitContainer2.Panel1.Controls.Add(this.checkBoxRun);
			this.splitContainer2.Panel1.Controls.Add(this.comboBoxBaudrate);
			this.splitContainer2.Panel1.Controls.Add(this.comboBoxPort);
			this.splitContainer2.Panel1.Controls.Add(this.checkBoxShowErrors);
			this.splitContainer2.Panel1.Controls.Add(this.checkBoxShowWarnings);
			this.splitContainer2.Panel1.Controls.Add(this.checkBoxShowMessages);
			// 
			// splitContainer2.Panel2
			// 
			this.splitContainer2.Panel2.Controls.Add(this.tabControl1);
			this.splitContainer2.Size = new System.Drawing.Size(991, 770);
			this.splitContainer2.SplitterDistance = 55;
			this.splitContainer2.TabIndex = 27;
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(349, 18);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(220, 20);
			this.textBox1.TabIndex = 36;
			this.textBox1.Visible = false;
			// 
			// buttonReset
			// 
			this.buttonReset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonReset.Location = new System.Drawing.Point(575, 16);
			this.buttonReset.Name = "buttonReset";
			this.buttonReset.Size = new System.Drawing.Size(46, 23);
			this.buttonReset.TabIndex = 35;
			this.buttonReset.Text = "Reset";
			this.buttonReset.UseVisualStyleBackColor = true;
			this.buttonReset.Click += new System.EventHandler(this.buttonReset_Click);
			// 
			// checkBoxDeltaTime
			// 
			this.checkBoxDeltaTime.AutoSize = true;
			this.checkBoxDeltaTime.Checked = global::NodeInspector.Properties.Settings.Default.DeltaTime;
			this.checkBoxDeltaTime.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::NodeInspector.Properties.Settings.Default, "DeltaTime", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.checkBoxDeltaTime.Location = new System.Drawing.Point(247, 20);
			this.checkBoxDeltaTime.Name = "checkBoxDeltaTime";
			this.checkBoxDeltaTime.Size = new System.Drawing.Size(73, 17);
			this.checkBoxDeltaTime.TabIndex = 34;
			this.checkBoxDeltaTime.Text = "Delta time";
			this.checkBoxDeltaTime.UseVisualStyleBackColor = true;
			this.checkBoxDeltaTime.Visible = false;
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
			this.checkBoxTrackNewest.TabIndex = 33;
			this.checkBoxTrackNewest.Text = "Track newest";
			this.checkBoxTrackNewest.UseVisualStyleBackColor = true;
			// 
			// checkBoxRun
			// 
			this.checkBoxRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.checkBoxRun.AutoSize = true;
			this.checkBoxRun.Checked = global::NodeInspector.Properties.Settings.Default.RunState;
			this.checkBoxRun.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxRun.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::NodeInspector.Properties.Settings.Default, "RunState", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.checkBoxRun.Location = new System.Drawing.Point(933, 20);
			this.checkBoxRun.Name = "checkBoxRun";
			this.checkBoxRun.Size = new System.Drawing.Size(46, 17);
			this.checkBoxRun.TabIndex = 32;
			this.checkBoxRun.Text = "Run";
			this.checkBoxRun.UseVisualStyleBackColor = true;
			this.checkBoxRun.CheckedChanged += new System.EventHandler(this.checkBoxRun_CheckedChanged);
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
			this.comboBoxBaudrate.Location = new System.Drawing.Point(806, 18);
			this.comboBoxBaudrate.Name = "comboBoxBaudrate";
			this.comboBoxBaudrate.Size = new System.Drawing.Size(121, 21);
			this.comboBoxBaudrate.TabIndex = 31;
			this.comboBoxBaudrate.Text = global::NodeInspector.Properties.Settings.Default.Baudrate;
			// 
			// comboBoxPort
			// 
			this.comboBoxPort.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.comboBoxPort.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::NodeInspector.Properties.Settings.Default, "Port", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.comboBoxPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxPort.FormattingEnabled = true;
			this.comboBoxPort.Location = new System.Drawing.Point(627, 18);
			this.comboBoxPort.Name = "comboBoxPort";
			this.comboBoxPort.Size = new System.Drawing.Size(173, 21);
			this.comboBoxPort.TabIndex = 30;
			this.comboBoxPort.Text = global::NodeInspector.Properties.Settings.Default.Port;
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
			this.checkBoxShowErrors.TabIndex = 29;
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
			this.checkBoxShowWarnings.TabIndex = 28;
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
			this.checkBoxShowMessages.TabIndex = 27;
			this.checkBoxShowMessages.UseVisualStyleBackColor = true;
			this.checkBoxShowMessages.CheckedChanged += new System.EventHandler(this.LevelCheckboxesChanged);
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this.tabPage1);
			this.tabControl1.Controls.Add(this.tabPage2);
			this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControl1.Location = new System.Drawing.Point(0, 0);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(991, 711);
			this.tabControl1.TabIndex = 0;
			// 
			// tabPage1
			// 
			this.tabPage1.Controls.Add(this.splitContainer1);
			this.tabPage1.Location = new System.Drawing.Point(4, 22);
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage1.Size = new System.Drawing.Size(983, 685);
			this.tabPage1.TabIndex = 0;
			this.tabPage1.Text = "Events";
			this.tabPage1.UseVisualStyleBackColor = true;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(3, 3);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.listViewLogs);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.dataGridView1);
			this.splitContainer1.Size = new System.Drawing.Size(977, 679);
			this.splitContainer1.SplitterDistance = 381;
			this.splitContainer1.SplitterWidth = 5;
			this.splitContainer1.TabIndex = 26;
			// 
			// listViewLogs
			// 
			this.listViewLogs.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.listViewLogs.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader6,
            this.columnHeader4});
			this.listViewLogs.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listViewLogs.FullRowSelect = true;
			this.listViewLogs.Location = new System.Drawing.Point(0, 0);
			this.listViewLogs.Name = "listViewLogs";
			this.listViewLogs.Size = new System.Drawing.Size(977, 381);
			this.listViewLogs.SmallImageList = this.imageList1;
			this.listViewLogs.TabIndex = 20;
			this.listViewLogs.UseCompatibleStateImageBehavior = false;
			this.listViewLogs.View = System.Windows.Forms.View.Details;
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
			// dataGridView1
			// 
			this.dataGridView1.AllowUserToAddRows = false;
			this.dataGridView1.AllowUserToDeleteRows = false;
			this.dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
			this.dataGridView1.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
			this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.Window;
			this.dataGridView1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.dataGridView1.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.None;
			this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridView1.Location = new System.Drawing.Point(0, 0);
			this.dataGridView1.MultiSelect = false;
			this.dataGridView1.Name = "dataGridView1";
			this.dataGridView1.RowHeadersVisible = false;
			this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.dataGridView1.Size = new System.Drawing.Size(977, 293);
			this.dataGridView1.StandardTab = true;
			this.dataGridView1.TabIndex = 0;
			// 
			// tabPage2
			// 
			this.tabPage2.Controls.Add(this.splitContainer3);
			this.tabPage2.Location = new System.Drawing.Point(4, 22);
			this.tabPage2.Name = "tabPage2";
			this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage2.Size = new System.Drawing.Size(983, 685);
			this.tabPage2.TabIndex = 1;
			this.tabPage2.Text = "Terminal";
			this.tabPage2.UseVisualStyleBackColor = true;
			// 
			// splitContainer3
			// 
			this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer3.IsSplitterFixed = true;
			this.splitContainer3.Location = new System.Drawing.Point(3, 3);
			this.splitContainer3.Name = "splitContainer3";
			this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer3.Panel1
			// 
			this.splitContainer3.Panel1.Controls.Add(this.buttonClearTerminal);
			// 
			// splitContainer3.Panel2
			// 
			this.splitContainer3.Panel2.Controls.Add(this.richTextBoxTerminal);
			this.splitContainer3.Size = new System.Drawing.Size(977, 679);
			this.splitContainer3.SplitterDistance = 30;
			this.splitContainer3.TabIndex = 1;
			// 
			// buttonClearTerminal
			// 
			this.buttonClearTerminal.Location = new System.Drawing.Point(3, 3);
			this.buttonClearTerminal.Name = "buttonClearTerminal";
			this.buttonClearTerminal.Size = new System.Drawing.Size(75, 23);
			this.buttonClearTerminal.TabIndex = 0;
			this.buttonClearTerminal.Text = "Clear";
			this.buttonClearTerminal.UseVisualStyleBackColor = true;
			this.buttonClearTerminal.Click += new System.EventHandler(this.buttonClearTerminal_Click);
			// 
			// richTextBoxTerminal
			// 
			this.richTextBoxTerminal.Dock = System.Windows.Forms.DockStyle.Fill;
			this.richTextBoxTerminal.Location = new System.Drawing.Point(0, 0);
			this.richTextBoxTerminal.Name = "richTextBoxTerminal";
			this.richTextBoxTerminal.Size = new System.Drawing.Size(977, 645);
			this.richTextBoxTerminal.TabIndex = 1;
			this.richTextBoxTerminal.Text = "";
			// 
			// MainView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(991, 770);
			this.Controls.Add(this.splitContainer2);
			this.DataBindings.Add(new System.Windows.Forms.Binding("Location", global::NodeInspector.Properties.Settings.Default, "MainViewLocation", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Location = global::NodeInspector.Properties.Settings.Default.MainViewLocation;
			this.MinimumSize = new System.Drawing.Size(708, 260);
			this.Name = "MainView";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Node Inspector";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainView_FormClosing);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.splitContainer2.Panel1.ResumeLayout(false);
			this.splitContainer2.Panel1.PerformLayout();
			this.splitContainer2.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
			this.splitContainer2.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.tabPage1.ResumeLayout(false);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
			this.tabPage2.ResumeLayout(false);
			this.splitContainer3.Panel1.ResumeLayout(false);
			this.splitContainer3.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
			this.splitContainer3.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.SplitContainer splitContainer2;
		private System.Windows.Forms.Button buttonReset;
		private System.Windows.Forms.CheckBox checkBoxDeltaTime;
		private System.Windows.Forms.CheckBox checkBoxTrackNewest;
		private System.Windows.Forms.CheckBox checkBoxRun;
		private System.Windows.Forms.ComboBox comboBoxBaudrate;
		private System.Windows.Forms.ComboBox comboBoxPort;
		private System.Windows.Forms.CheckBox checkBoxShowErrors;
		private System.Windows.Forms.CheckBox checkBoxShowWarnings;
		private System.Windows.Forms.CheckBox checkBoxShowMessages;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPage1;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private ListViewNoFlicker listViewLogs;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.DataGridView dataGridView1;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.SplitContainer splitContainer3;
		private System.Windows.Forms.Button buttonClearTerminal;
		private System.Windows.Forms.RichTextBox richTextBoxTerminal;
		private System.Windows.Forms.TextBox textBox1;
	}
}

