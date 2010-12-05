namespace RouteSimulator
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
			this.dataGridViewLinkLosses = new System.Windows.Forms.DataGridView();
			this.propertyGridRouteBuildingInfo = new System.Windows.Forms.PropertyGrid();
			this.button1 = new System.Windows.Forms.Button();
			this.numericUpDownReceiver = new System.Windows.Forms.NumericUpDown();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.dataGridViewCosts = new System.Windows.Forms.DataGridView();
			this.tabPage2 = new System.Windows.Forms.TabPage();
			this.dataGridViewRoutes = new System.Windows.Forms.DataGridView();
			this.numericUpDownSender = new System.Windows.Forms.NumericUpDown();
			this.buttonReadData = new System.Windows.Forms.Button();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewLinkLosses)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownReceiver)).BeginInit();
			this.tabControl1.SuspendLayout();
			this.tabPage1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewCosts)).BeginInit();
			this.tabPage2.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewRoutes)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownSender)).BeginInit();
			this.SuspendLayout();
			// 
			// dataGridViewLinkLosses
			// 
			this.dataGridViewLinkLosses.AllowUserToAddRows = false;
			this.dataGridViewLinkLosses.AllowUserToDeleteRows = false;
			this.dataGridViewLinkLosses.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
									| System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridViewLinkLosses.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
			this.dataGridViewLinkLosses.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
			this.dataGridViewLinkLosses.BackgroundColor = System.Drawing.SystemColors.Window;
			this.dataGridViewLinkLosses.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.dataGridViewLinkLosses.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.None;
			this.dataGridViewLinkLosses.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewLinkLosses.Location = new System.Drawing.Point(12, 12);
			this.dataGridViewLinkLosses.MultiSelect = false;
			this.dataGridViewLinkLosses.Name = "dataGridViewLinkLosses";
			this.dataGridViewLinkLosses.RowHeadersVisible = false;
			this.dataGridViewLinkLosses.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.dataGridViewLinkLosses.Size = new System.Drawing.Size(990, 314);
			this.dataGridViewLinkLosses.StandardTab = true;
			this.dataGridViewLinkLosses.TabIndex = 1;
			// 
			// propertyGridRouteBuildingInfo
			// 
			this.propertyGridRouteBuildingInfo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.propertyGridRouteBuildingInfo.HelpVisible = false;
			this.propertyGridRouteBuildingInfo.Location = new System.Drawing.Point(1008, 12);
			this.propertyGridRouteBuildingInfo.Name = "propertyGridRouteBuildingInfo";
			this.propertyGridRouteBuildingInfo.PropertySort = System.Windows.Forms.PropertySort.Alphabetical;
			this.propertyGridRouteBuildingInfo.Size = new System.Drawing.Size(269, 314);
			this.propertyGridRouteBuildingInfo.TabIndex = 3;
			this.propertyGridRouteBuildingInfo.ToolbarVisible = false;
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.button1.Location = new System.Drawing.Point(1202, 734);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 4;
			this.button1.Text = "Build";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// numericUpDownReceiver
			// 
			this.numericUpDownReceiver.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.numericUpDownReceiver.Location = new System.Drawing.Point(1140, 737);
			this.numericUpDownReceiver.Maximum = new decimal(new int[] {
            14,
            0,
            0,
            0});
			this.numericUpDownReceiver.Name = "numericUpDownReceiver";
			this.numericUpDownReceiver.Size = new System.Drawing.Size(56, 20);
			this.numericUpDownReceiver.TabIndex = 5;
			this.numericUpDownReceiver.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
									| System.Windows.Forms.AnchorStyles.Left)
									| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl1.Controls.Add(this.tabPage1);
			this.tabControl1.Controls.Add(this.tabPage2);
			this.tabControl1.Location = new System.Drawing.Point(12, 332);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(1265, 399);
			this.tabControl1.TabIndex = 6;
			// 
			// tabPage1
			// 
			this.tabPage1.Controls.Add(this.dataGridViewCosts);
			this.tabPage1.Location = new System.Drawing.Point(4, 22);
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage1.Size = new System.Drawing.Size(1257, 373);
			this.tabPage1.TabIndex = 0;
			this.tabPage1.Text = "Costs";
			this.tabPage1.UseVisualStyleBackColor = true;
			// 
			// dataGridViewCosts
			// 
			this.dataGridViewCosts.AllowUserToAddRows = false;
			this.dataGridViewCosts.AllowUserToDeleteRows = false;
			this.dataGridViewCosts.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
			this.dataGridViewCosts.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
			this.dataGridViewCosts.BackgroundColor = System.Drawing.SystemColors.Window;
			this.dataGridViewCosts.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.dataGridViewCosts.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.None;
			this.dataGridViewCosts.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewCosts.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridViewCosts.Location = new System.Drawing.Point(3, 3);
			this.dataGridViewCosts.MultiSelect = false;
			this.dataGridViewCosts.Name = "dataGridViewCosts";
			this.dataGridViewCosts.ReadOnly = true;
			this.dataGridViewCosts.RowHeadersVisible = false;
			this.dataGridViewCosts.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.dataGridViewCosts.Size = new System.Drawing.Size(1251, 367);
			this.dataGridViewCosts.StandardTab = true;
			this.dataGridViewCosts.TabIndex = 3;
			// 
			// tabPage2
			// 
			this.tabPage2.Controls.Add(this.dataGridViewRoutes);
			this.tabPage2.Location = new System.Drawing.Point(4, 22);
			this.tabPage2.Name = "tabPage2";
			this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage2.Size = new System.Drawing.Size(1257, 373);
			this.tabPage2.TabIndex = 1;
			this.tabPage2.Text = "Routes";
			this.tabPage2.UseVisualStyleBackColor = true;
			// 
			// dataGridViewRoutes
			// 
			this.dataGridViewRoutes.AllowUserToAddRows = false;
			this.dataGridViewRoutes.AllowUserToDeleteRows = false;
			this.dataGridViewRoutes.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
			this.dataGridViewRoutes.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
			this.dataGridViewRoutes.BackgroundColor = System.Drawing.SystemColors.Window;
			this.dataGridViewRoutes.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.dataGridViewRoutes.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.None;
			this.dataGridViewRoutes.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewRoutes.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridViewRoutes.Location = new System.Drawing.Point(3, 3);
			this.dataGridViewRoutes.MultiSelect = false;
			this.dataGridViewRoutes.Name = "dataGridViewRoutes";
			this.dataGridViewRoutes.ReadOnly = true;
			this.dataGridViewRoutes.RowHeadersVisible = false;
			this.dataGridViewRoutes.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.dataGridViewRoutes.Size = new System.Drawing.Size(1251, 367);
			this.dataGridViewRoutes.StandardTab = true;
			this.dataGridViewRoutes.TabIndex = 4;
			// 
			// numericUpDownSender
			// 
			this.numericUpDownSender.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.numericUpDownSender.Location = new System.Drawing.Point(1078, 737);
			this.numericUpDownSender.Maximum = new decimal(new int[] {
            14,
            0,
            0,
            0});
			this.numericUpDownSender.Name = "numericUpDownSender";
			this.numericUpDownSender.Size = new System.Drawing.Size(56, 20);
			this.numericUpDownSender.TabIndex = 7;
			this.numericUpDownSender.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.numericUpDownSender.Value = new decimal(new int[] {
            4,
            0,
            0,
            0});
			// 
			// buttonReadData
			// 
			this.buttonReadData.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonReadData.Location = new System.Drawing.Point(997, 734);
			this.buttonReadData.Name = "buttonReadData";
			this.buttonReadData.Size = new System.Drawing.Size(75, 23);
			this.buttonReadData.TabIndex = 8;
			this.buttonReadData.Text = "Read data...";
			this.buttonReadData.UseVisualStyleBackColor = true;
			this.buttonReadData.Click += new System.EventHandler(this.buttonReadData_Click);
			// 
			// MainView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1289, 769);
			this.Controls.Add(this.buttonReadData);
			this.Controls.Add(this.numericUpDownSender);
			this.Controls.Add(this.tabControl1);
			this.Controls.Add(this.numericUpDownReceiver);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.propertyGridRouteBuildingInfo);
			this.Controls.Add(this.dataGridViewLinkLosses);
			this.Name = "MainView";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Route Simulator";
			this.Load += new System.EventHandler(this.Form1_Load);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewLinkLosses)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownReceiver)).EndInit();
			this.tabControl1.ResumeLayout(false);
			this.tabPage1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewCosts)).EndInit();
			this.tabPage2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewRoutes)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownSender)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.DataGridView dataGridViewLinkLosses;
		private System.Windows.Forms.PropertyGrid propertyGridRouteBuildingInfo;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.NumericUpDown numericUpDownReceiver;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPage1;
		private System.Windows.Forms.DataGridView dataGridViewCosts;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.DataGridView dataGridViewRoutes;
		private System.Windows.Forms.NumericUpDown numericUpDownSender;
		private System.Windows.Forms.Button buttonReadData;

	}
}

