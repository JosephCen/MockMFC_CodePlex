namespace DBQuerier
{
    partial class MainFrm
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
            this.splitConMain = new System.Windows.Forms.SplitContainer();
            this.cboQueryDef = new System.Windows.Forms.ComboBox();
            this.lblQueryDef = new System.Windows.Forms.Label();
            this.btnReset = new System.Windows.Forms.Button();
            this.btnQuery = new System.Windows.Forms.Button();
            this.parmPicker = new DBQuerier.UIElement.ParmPicker();
            this.dataGridVw = new System.Windows.Forms.DataGridView();
            this.cmuDataGrid = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.openXmlToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.splitConMain)).BeginInit();
            this.splitConMain.Panel1.SuspendLayout();
            this.splitConMain.Panel2.SuspendLayout();
            this.splitConMain.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridVw)).BeginInit();
            this.cmuDataGrid.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitConMain
            // 
            this.splitConMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitConMain.Location = new System.Drawing.Point(0, 0);
            this.splitConMain.Name = "splitConMain";
            this.splitConMain.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitConMain.Panel1
            // 
            this.splitConMain.Panel1.Controls.Add(this.cboQueryDef);
            this.splitConMain.Panel1.Controls.Add(this.lblQueryDef);
            this.splitConMain.Panel1.Controls.Add(this.btnReset);
            this.splitConMain.Panel1.Controls.Add(this.btnQuery);
            this.splitConMain.Panel1.Controls.Add(this.parmPicker);
            // 
            // splitConMain.Panel2
            // 
            this.splitConMain.Panel2.Controls.Add(this.dataGridVw);
            this.splitConMain.Size = new System.Drawing.Size(464, 337);
            this.splitConMain.SplitterDistance = 154;
            this.splitConMain.TabIndex = 0;
            // 
            // cboQueryDef
            // 
            this.cboQueryDef.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.cboQueryDef.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboQueryDef.FormattingEnabled = true;
            this.cboQueryDef.Location = new System.Drawing.Point(103, 128);
            this.cboQueryDef.Name = "cboQueryDef";
            this.cboQueryDef.Size = new System.Drawing.Size(196, 21);
            this.cboQueryDef.TabIndex = 4;
            this.cboQueryDef.SelectedIndexChanged += new System.EventHandler(this.cboQueryDef_SelectedIndexChanged);
            // 
            // lblQueryDef
            // 
            this.lblQueryDef.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblQueryDef.AutoSize = true;
            this.lblQueryDef.Location = new System.Drawing.Point(12, 131);
            this.lblQueryDef.Name = "lblQueryDef";
            this.lblQueryDef.Size = new System.Drawing.Size(85, 13);
            this.lblQueryDef.TabIndex = 3;
            this.lblQueryDef.Text = "Query Definition:";
            // 
            // btnReset
            // 
            this.btnReset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnReset.Location = new System.Drawing.Point(305, 126);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(75, 23);
            this.btnReset.TabIndex = 2;
            this.btnReset.Text = "Reset";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // btnQuery
            // 
            this.btnQuery.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnQuery.Location = new System.Drawing.Point(386, 126);
            this.btnQuery.Name = "btnQuery";
            this.btnQuery.Size = new System.Drawing.Size(75, 23);
            this.btnQuery.TabIndex = 1;
            this.btnQuery.Text = "Query";
            this.btnQuery.UseVisualStyleBackColor = true;
            this.btnQuery.Click += new System.EventHandler(this.btnQuery_Click);
            // 
            // parmPicker
            // 
            this.parmPicker.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.parmPicker.Location = new System.Drawing.Point(3, 3);
            this.parmPicker.Name = "parmPicker";
            this.parmPicker.ParameterDefs = null;
            this.parmPicker.Size = new System.Drawing.Size(458, 117);
            this.parmPicker.TabIndex = 0;
            // 
            // dataGridVw
            // 
            this.dataGridVw.AllowUserToAddRows = false;
            this.dataGridVw.AllowUserToDeleteRows = false;
            this.dataGridVw.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridVw.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridVw.ContextMenuStrip = this.cmuDataGrid;
            this.dataGridVw.Location = new System.Drawing.Point(3, 3);
            this.dataGridVw.Name = "dataGridVw";
            this.dataGridVw.ReadOnly = true;
            this.dataGridVw.Size = new System.Drawing.Size(458, 173);
            this.dataGridVw.TabIndex = 0;
            // 
            // cmuDataGrid
            // 
            this.cmuDataGrid.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openXmlToolStripMenuItem});
            this.cmuDataGrid.Name = "cmuDataGrid";
            this.cmuDataGrid.Size = new System.Drawing.Size(128, 26);
            this.cmuDataGrid.Opening += new System.ComponentModel.CancelEventHandler(this.cmuDataGrid_Opening);
            this.cmuDataGrid.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.cmuDataGrid_ItemClicked);
            // 
            // openXmlToolStripMenuItem
            // 
            this.openXmlToolStripMenuItem.Name = "openXmlToolStripMenuItem";
            this.openXmlToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
            this.openXmlToolStripMenuItem.Text = "Open Xml";
            // 
            // MainFrm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(464, 337);
            this.Controls.Add(this.splitConMain);
            this.MinimumSize = new System.Drawing.Size(480, 375);
            this.Name = "MainFrm";
            this.Text = "DB Querier";
            this.Load += new System.EventHandler(this.MainFrm_Load);
            this.splitConMain.Panel1.ResumeLayout(false);
            this.splitConMain.Panel1.PerformLayout();
            this.splitConMain.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitConMain)).EndInit();
            this.splitConMain.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridVw)).EndInit();
            this.cmuDataGrid.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitConMain;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.Button btnQuery;
        private UIElement.ParmPicker parmPicker;
        private System.Windows.Forms.DataGridView dataGridVw;
        private System.Windows.Forms.ComboBox cboQueryDef;
        private System.Windows.Forms.Label lblQueryDef;
        private System.Windows.Forms.ContextMenuStrip cmuDataGrid;
        private System.Windows.Forms.ToolStripMenuItem openXmlToolStripMenuItem;

    }
}

