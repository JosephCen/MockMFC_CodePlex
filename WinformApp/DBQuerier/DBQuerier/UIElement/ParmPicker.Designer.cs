namespace DBQuerier.UIElement
{
    partial class ParmPicker
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.txtParm = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // txtParm
            // 
            this.txtParm.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtParm.Location = new System.Drawing.Point(3, 3);
            this.txtParm.Multiline = true;
            this.txtParm.Name = "txtParm";
            this.txtParm.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtParm.Size = new System.Drawing.Size(144, 144);
            this.txtParm.TabIndex = 0;
            // 
            // ParmPicker
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.txtParm);
            this.Name = "ParmPicker";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtParm;
    }
}
