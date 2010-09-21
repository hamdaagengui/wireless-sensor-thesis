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
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.splitContainer2 = new System.Windows.Forms.SplitContainer();
			this.propertyGridNodeStatistics = new System.Windows.Forms.PropertyGrid();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.tabPage2 = new System.Windows.Forms.TabPage();
			this.checkBoxShowFramework = new System.Windows.Forms.CheckBox();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.checkBoxShowApplication = new System.Windows.Forms.CheckBox();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.checkBoxShowErrors = new System.Windows.Forms.CheckBox();
			this.checkBoxShowWarnings = new System.Windows.Forms.CheckBox();
			this.checkBoxShowMessages = new System.Windows.Forms.CheckBox();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.toolStripStatusLabel4 = new System.Windows.Forms.ToolStripStatusLabel();
			this.toolStripStatusLabelFrames = new System.Windows.Forms.ToolStripStatusLabel();
			this.toolStripStatusLabelMessages = new System.Windows.Forms.ToolStripStatusLabel();
			this.toolStripStatusLabelLost = new System.Windows.Forms.ToolStripStatusLabel();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.sdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.clearAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.clearToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enableToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
			this.clearToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.enableInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enablewarningsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enableerrorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enableApplicationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enableframeworkToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.comboBoxPorts = new System.Windows.Forms.ToolStripComboBox();
			this.textBoxBaudrate = new System.Windows.Forms.ToolStripTextBox();
			this.toolStripMenuItemShowDeltaTimes = new System.Windows.Forms.ToolStripMenuItem();
			this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.disconnectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.testToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.timerRefreshLists = new System.Windows.Forms.Timer(this.components);
			this.listViewNodes = new NodeInspector.ListViewNoFlicker();
			this.splitContainer3 = new System.Windows.Forms.SplitContainer();
			this.listViewFrames = new NodeInspector.ListViewNoFlicker();
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.richTextBoxFrameInfo = new System.Windows.Forms.RichTextBox();
			this.listViewLog = new NodeInspector.ListViewNoFlicker();
			this.columnHeader10 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader12 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader9 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader8 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
			this.splitContainer2.Panel1.SuspendLayout();
			this.splitContainer2.Panel2.SuspendLayout();
			this.splitContainer2.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.tabPage1.SuspendLayout();
			this.tabPage2.SuspendLayout();
			this.statusStrip1.SuspendLayout();
			this.menuStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
			this.splitContainer3.Panel1.SuspendLayout();
			this.splitContainer3.Panel2.SuspendLayout();
			this.splitContainer3.SuspendLayout();
			this.SuspendLayout();
			// 
			// splitContainer1
			// 
			resources.ApplyResources(this.splitContainer1, "splitContainer1");
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.tabControl1);
			// 
			// splitContainer2
			// 
			resources.ApplyResources(this.splitContainer2, "splitContainer2");
			this.splitContainer2.Name = "splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this.splitContainer2.Panel1.Controls.Add(this.listViewNodes);
			// 
			// splitContainer2.Panel2
			// 
			this.splitContainer2.Panel2.Controls.Add(this.propertyGridNodeStatistics);
			// 
			// propertyGridNodeStatistics
			// 
			resources.ApplyResources(this.propertyGridNodeStatistics, "propertyGridNodeStatistics");
			this.propertyGridNodeStatistics.Name = "propertyGridNodeStatistics";
			this.propertyGridNodeStatistics.PropertySort = System.Windows.Forms.PropertySort.Alphabetical;
			this.propertyGridNodeStatistics.ToolbarVisible = false;
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this.tabPage1);
			this.tabControl1.Controls.Add(this.tabPage2);
			resources.ApplyResources(this.tabControl1, "tabControl1");
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			// 
			// tabPage1
			// 
			this.tabPage1.Controls.Add(this.splitContainer3);
			resources.ApplyResources(this.tabPage1, "tabPage1");
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this.tabPage2.Controls.Add(this.checkBoxShowFramework);
			this.tabPage2.Controls.Add(this.checkBoxShowApplication);
			this.tabPage2.Controls.Add(this.textBox1);
			this.tabPage2.Controls.Add(this.button1);
			this.tabPage2.Controls.Add(this.checkBoxShowErrors);
			this.tabPage2.Controls.Add(this.checkBoxShowWarnings);
			this.tabPage2.Controls.Add(this.checkBoxShowMessages);
			this.tabPage2.Controls.Add(this.listViewLog);
			resources.ApplyResources(this.tabPage2, "tabPage2");
			this.tabPage2.Name = "tabPage2";
			this.tabPage2.UseVisualStyleBackColor = true;
			// 
			// checkBoxShowFramework
			// 
			this.checkBoxShowFramework.Checked = true;
			this.checkBoxShowFramework.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowFramework.ImageList = this.imageList1;
			resources.ApplyResources(this.checkBoxShowFramework, "checkBoxShowFramework");
			this.checkBoxShowFramework.Name = "checkBoxShowFramework";
			this.checkBoxShowFramework.UseVisualStyleBackColor = true;
			// 
			// imageList1
			// 
			this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
			resources.ApplyResources(this.imageList1, "imageList1");
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// checkBoxShowApplication
			// 
			this.checkBoxShowApplication.Checked = true;
			this.checkBoxShowApplication.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowApplication.ImageList = this.imageList1;
			resources.ApplyResources(this.checkBoxShowApplication, "checkBoxShowApplication");
			this.checkBoxShowApplication.Name = "checkBoxShowApplication";
			this.checkBoxShowApplication.UseVisualStyleBackColor = true;
			// 
			// textBox1
			// 
			resources.ApplyResources(this.textBox1, "textBox1");
			this.textBox1.Name = "textBox1";
			// 
			// button1
			// 
			resources.ApplyResources(this.button1, "button1");
			this.button1.Name = "button1";
			this.button1.UseVisualStyleBackColor = true;
			// 
			// checkBoxShowErrors
			// 
			this.checkBoxShowErrors.Checked = true;
			this.checkBoxShowErrors.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowErrors.ImageList = this.imageList1;
			resources.ApplyResources(this.checkBoxShowErrors, "checkBoxShowErrors");
			this.checkBoxShowErrors.Name = "checkBoxShowErrors";
			this.checkBoxShowErrors.UseVisualStyleBackColor = true;
			// 
			// checkBoxShowWarnings
			// 
			this.checkBoxShowWarnings.Checked = true;
			this.checkBoxShowWarnings.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowWarnings.ImageList = this.imageList1;
			resources.ApplyResources(this.checkBoxShowWarnings, "checkBoxShowWarnings");
			this.checkBoxShowWarnings.Name = "checkBoxShowWarnings";
			this.checkBoxShowWarnings.UseVisualStyleBackColor = true;
			// 
			// checkBoxShowMessages
			// 
			this.checkBoxShowMessages.Checked = true;
			this.checkBoxShowMessages.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowMessages.ImageList = this.imageList1;
			resources.ApplyResources(this.checkBoxShowMessages, "checkBoxShowMessages");
			this.checkBoxShowMessages.Name = "checkBoxShowMessages";
			this.checkBoxShowMessages.UseVisualStyleBackColor = true;
			// 
			// statusStrip1
			// 
			this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel4,
            this.toolStripStatusLabelFrames,
            this.toolStripStatusLabelMessages,
            this.toolStripStatusLabelLost});
			resources.ApplyResources(this.statusStrip1, "statusStrip1");
			this.statusStrip1.Name = "statusStrip1";
			// 
			// toolStripStatusLabel4
			// 
			this.toolStripStatusLabel4.Name = "toolStripStatusLabel4";
			resources.ApplyResources(this.toolStripStatusLabel4, "toolStripStatusLabel4");
			this.toolStripStatusLabel4.Spring = true;
			// 
			// toolStripStatusLabelFrames
			// 
			resources.ApplyResources(this.toolStripStatusLabelFrames, "toolStripStatusLabelFrames");
			this.toolStripStatusLabelFrames.Name = "toolStripStatusLabelFrames";
			// 
			// toolStripStatusLabelMessages
			// 
			resources.ApplyResources(this.toolStripStatusLabelMessages, "toolStripStatusLabelMessages");
			this.toolStripStatusLabelMessages.Name = "toolStripStatusLabelMessages";
			// 
			// toolStripStatusLabelLost
			// 
			resources.ApplyResources(this.toolStripStatusLabelLost, "toolStripStatusLabelLost");
			this.toolStripStatusLabelLost.Name = "toolStripStatusLabelLost";
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sdToolStripMenuItem,
            this.viewsToolStripMenuItem,
            this.optionsToolStripMenuItem,
            this.connectToolStripMenuItem,
            this.disconnectToolStripMenuItem,
            this.testToolStripMenuItem});
			resources.ApplyResources(this.menuStrip1, "menuStrip1");
			this.menuStrip1.Name = "menuStrip1";
			// 
			// sdToolStripMenuItem
			// 
			this.sdToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
			this.sdToolStripMenuItem.Name = "sdToolStripMenuItem";
			resources.ApplyResources(this.sdToolStripMenuItem, "sdToolStripMenuItem");
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			resources.ApplyResources(this.exitToolStripMenuItem, "exitToolStripMenuItem");
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
			// 
			// viewsToolStripMenuItem
			// 
			this.viewsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clearAllToolStripMenuItem,
            this.toolStripMenuItem1,
            this.toolStripMenuItem2});
			this.viewsToolStripMenuItem.Name = "viewsToolStripMenuItem";
			resources.ApplyResources(this.viewsToolStripMenuItem, "viewsToolStripMenuItem");
			// 
			// clearAllToolStripMenuItem
			// 
			this.clearAllToolStripMenuItem.Name = "clearAllToolStripMenuItem";
			resources.ApplyResources(this.clearAllToolStripMenuItem, "clearAllToolStripMenuItem");
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clearToolStripMenuItem,
            this.enableToolStripMenuItem});
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			resources.ApplyResources(this.toolStripMenuItem1, "toolStripMenuItem1");
			// 
			// clearToolStripMenuItem
			// 
			this.clearToolStripMenuItem.Name = "clearToolStripMenuItem";
			resources.ApplyResources(this.clearToolStripMenuItem, "clearToolStripMenuItem");
			// 
			// enableToolStripMenuItem
			// 
			this.enableToolStripMenuItem.Name = "enableToolStripMenuItem";
			resources.ApplyResources(this.enableToolStripMenuItem, "enableToolStripMenuItem");
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clearToolStripMenuItem1,
            this.enableInfoToolStripMenuItem,
            this.enablewarningsToolStripMenuItem,
            this.enableerrorsToolStripMenuItem,
            this.enableApplicationToolStripMenuItem,
            this.enableframeworkToolStripMenuItem});
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			resources.ApplyResources(this.toolStripMenuItem2, "toolStripMenuItem2");
			// 
			// clearToolStripMenuItem1
			// 
			this.clearToolStripMenuItem1.Name = "clearToolStripMenuItem1";
			resources.ApplyResources(this.clearToolStripMenuItem1, "clearToolStripMenuItem1");
			// 
			// enableInfoToolStripMenuItem
			// 
			this.enableInfoToolStripMenuItem.Name = "enableInfoToolStripMenuItem";
			resources.ApplyResources(this.enableInfoToolStripMenuItem, "enableInfoToolStripMenuItem");
			// 
			// enablewarningsToolStripMenuItem
			// 
			this.enablewarningsToolStripMenuItem.Name = "enablewarningsToolStripMenuItem";
			resources.ApplyResources(this.enablewarningsToolStripMenuItem, "enablewarningsToolStripMenuItem");
			// 
			// enableerrorsToolStripMenuItem
			// 
			this.enableerrorsToolStripMenuItem.Name = "enableerrorsToolStripMenuItem";
			resources.ApplyResources(this.enableerrorsToolStripMenuItem, "enableerrorsToolStripMenuItem");
			// 
			// enableApplicationToolStripMenuItem
			// 
			this.enableApplicationToolStripMenuItem.Name = "enableApplicationToolStripMenuItem";
			resources.ApplyResources(this.enableApplicationToolStripMenuItem, "enableApplicationToolStripMenuItem");
			// 
			// enableframeworkToolStripMenuItem
			// 
			this.enableframeworkToolStripMenuItem.Name = "enableframeworkToolStripMenuItem";
			resources.ApplyResources(this.enableframeworkToolStripMenuItem, "enableframeworkToolStripMenuItem");
			// 
			// optionsToolStripMenuItem
			// 
			this.optionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.comboBoxPorts,
            this.textBoxBaudrate,
            this.toolStripMenuItemShowDeltaTimes});
			this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
			resources.ApplyResources(this.optionsToolStripMenuItem, "optionsToolStripMenuItem");
			this.optionsToolStripMenuItem.DropDownOpening += new System.EventHandler(this.optionsToolStripMenuItem_DropDownOpening);
			// 
			// comboBoxPorts
			// 
			this.comboBoxPorts.Name = "comboBoxPorts";
			resources.ApplyResources(this.comboBoxPorts, "comboBoxPorts");
			// 
			// textBoxBaudrate
			// 
			this.textBoxBaudrate.Name = "textBoxBaudrate";
			resources.ApplyResources(this.textBoxBaudrate, "textBoxBaudrate");
			// 
			// toolStripMenuItemShowDeltaTimes
			// 
			this.toolStripMenuItemShowDeltaTimes.CheckOnClick = true;
			this.toolStripMenuItemShowDeltaTimes.Name = "toolStripMenuItemShowDeltaTimes";
			resources.ApplyResources(this.toolStripMenuItemShowDeltaTimes, "toolStripMenuItemShowDeltaTimes");
			// 
			// connectToolStripMenuItem
			// 
			this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
			resources.ApplyResources(this.connectToolStripMenuItem, "connectToolStripMenuItem");
			this.connectToolStripMenuItem.Click += new System.EventHandler(this.connectToolStripMenuItem_Click);
			// 
			// disconnectToolStripMenuItem
			// 
			resources.ApplyResources(this.disconnectToolStripMenuItem, "disconnectToolStripMenuItem");
			this.disconnectToolStripMenuItem.Name = "disconnectToolStripMenuItem";
			this.disconnectToolStripMenuItem.Click += new System.EventHandler(this.disconnectToolStripMenuItem_Click);
			// 
			// testToolStripMenuItem
			// 
			this.testToolStripMenuItem.Name = "testToolStripMenuItem";
			resources.ApplyResources(this.testToolStripMenuItem, "testToolStripMenuItem");
			this.testToolStripMenuItem.Click += new System.EventHandler(this.testToolStripMenuItem_Click);
			// 
			// timerRefreshLists
			// 
			this.timerRefreshLists.Interval = 250;
			this.timerRefreshLists.Tick += new System.EventHandler(this.timerRefreshLists_Tick);
			// 
			// listViewNodes
			// 
			resources.ApplyResources(this.listViewNodes, "listViewNodes");
			this.listViewNodes.FullRowSelect = true;
			this.listViewNodes.Name = "listViewNodes";
			this.listViewNodes.UseCompatibleStateImageBehavior = false;
			this.listViewNodes.View = System.Windows.Forms.View.Details;
			// 
			// splitContainer3
			// 
			resources.ApplyResources(this.splitContainer3, "splitContainer3");
			this.splitContainer3.Name = "splitContainer3";
			// 
			// splitContainer3.Panel1
			// 
			this.splitContainer3.Panel1.Controls.Add(this.listViewFrames);
			// 
			// splitContainer3.Panel2
			// 
			this.splitContainer3.Panel2.Controls.Add(this.richTextBoxFrameInfo);
			// 
			// listViewFrames
			// 
			this.listViewFrames.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
			resources.ApplyResources(this.listViewFrames, "listViewFrames");
			this.listViewFrames.FullRowSelect = true;
			this.listViewFrames.MultiSelect = false;
			this.listViewFrames.Name = "listViewFrames";
			this.listViewFrames.UseCompatibleStateImageBehavior = false;
			this.listViewFrames.View = System.Windows.Forms.View.Details;
			this.listViewFrames.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewFrames_ItemSelectionChanged);
			// 
			// columnHeader1
			// 
			resources.ApplyResources(this.columnHeader1, "columnHeader1");
			// 
			// columnHeader2
			// 
			resources.ApplyResources(this.columnHeader2, "columnHeader2");
			// 
			// columnHeader3
			// 
			resources.ApplyResources(this.columnHeader3, "columnHeader3");
			// 
			// columnHeader4
			// 
			resources.ApplyResources(this.columnHeader4, "columnHeader4");
			// 
			// columnHeader5
			// 
			resources.ApplyResources(this.columnHeader5, "columnHeader5");
			// 
			// richTextBoxFrameInfo
			// 
			resources.ApplyResources(this.richTextBoxFrameInfo, "richTextBoxFrameInfo");
			this.richTextBoxFrameInfo.Name = "richTextBoxFrameInfo";
			this.richTextBoxFrameInfo.ReadOnly = true;
			// 
			// listViewLog
			// 
			resources.ApplyResources(this.listViewLog, "listViewLog");
			this.listViewLog.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader10,
            this.columnHeader6,
            this.columnHeader7,
            this.columnHeader12,
            this.columnHeader9,
            this.columnHeader8});
			this.listViewLog.FullRowSelect = true;
			this.listViewLog.Name = "listViewLog";
			this.listViewLog.SmallImageList = this.imageList1;
			this.listViewLog.UseCompatibleStateImageBehavior = false;
			this.listViewLog.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader10
			// 
			resources.ApplyResources(this.columnHeader10, "columnHeader10");
			// 
			// columnHeader6
			// 
			resources.ApplyResources(this.columnHeader6, "columnHeader6");
			// 
			// columnHeader7
			// 
			resources.ApplyResources(this.columnHeader7, "columnHeader7");
			// 
			// columnHeader12
			// 
			resources.ApplyResources(this.columnHeader12, "columnHeader12");
			// 
			// columnHeader9
			// 
			resources.ApplyResources(this.columnHeader9, "columnHeader9");
			// 
			// columnHeader8
			// 
			resources.ApplyResources(this.columnHeader8, "columnHeader8");
			// 
			// MainView
			// 
			resources.ApplyResources(this, "$this");
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.statusStrip1);
			this.Controls.Add(this.menuStrip1);
			this.DoubleBuffered = true;
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "MainView";
			this.Load += new System.EventHandler(this.MainView_Load);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			this.splitContainer2.Panel1.ResumeLayout(false);
			this.splitContainer2.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
			this.splitContainer2.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.tabPage1.ResumeLayout(false);
			this.tabPage2.ResumeLayout(false);
			this.tabPage2.PerformLayout();
			this.statusStrip1.ResumeLayout(false);
			this.statusStrip1.PerformLayout();
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.splitContainer3.Panel1.ResumeLayout(false);
			this.splitContainer3.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
			this.splitContainer3.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem sdToolStripMenuItem;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.SplitContainer splitContainer2;
		private System.Windows.Forms.PropertyGrid propertyGridNodeStatistics;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPage1;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
		private System.Windows.Forms.ToolStripComboBox comboBoxPorts;
		private System.Windows.Forms.ToolStripTextBox textBoxBaudrate;
		private System.Windows.Forms.SplitContainer splitContainer3;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.RichTextBox richTextBoxFrameInfo;
		private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem disconnectToolStripMenuItem;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ColumnHeader columnHeader7;
		private System.Windows.Forms.ColumnHeader columnHeader8;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemShowDeltaTimes;
		private System.Windows.Forms.Timer timerRefreshLists;
		private System.Windows.Forms.ToolStripMenuItem viewsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem clearAllToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem testToolStripMenuItem;
		private System.Windows.Forms.ColumnHeader columnHeader12;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel4;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelFrames;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelMessages;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelLost;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem clearToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem clearToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem enableInfoToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem enablewarningsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem enableerrorsToolStripMenuItem;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.CheckBox checkBoxShowErrors;
		private System.Windows.Forms.CheckBox checkBoxShowWarnings;
		private System.Windows.Forms.CheckBox checkBoxShowMessages;
		private System.Windows.Forms.ToolStripMenuItem enableApplicationToolStripMenuItem;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.CheckBox checkBoxShowFramework;
		private System.Windows.Forms.CheckBox checkBoxShowApplication;
		private System.Windows.Forms.ColumnHeader columnHeader9;
		private System.Windows.Forms.ToolStripMenuItem enableToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem enableframeworkToolStripMenuItem;
		private ListViewNoFlicker listViewNodes;
		private ListViewNoFlicker listViewLog;
		private ListViewNoFlicker listViewFrames;
		private System.Windows.Forms.ColumnHeader columnHeader10;
	}
}

