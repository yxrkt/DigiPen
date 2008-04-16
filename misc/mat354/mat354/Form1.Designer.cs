namespace mat354
{
    partial class Form1
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
          System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager( typeof( Form1 ) );
          this.pictureBox = new System.Windows.Forms.PictureBox();
          this.toolStrip1 = new System.Windows.Forms.ToolStrip();
          this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
          ( (System.ComponentModel.ISupportInitialize)( this.pictureBox ) ).BeginInit();
          this.toolStrip1.SuspendLayout();
          this.SuspendLayout();
          // 
          // pictureBox
          // 
          this.pictureBox.Anchor = ( (System.Windows.Forms.AnchorStyles)( ( ( ( System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom )
                      | System.Windows.Forms.AnchorStyles.Left )
                      | System.Windows.Forms.AnchorStyles.Right ) ) );
          this.pictureBox.BackColor = System.Drawing.SystemColors.Window;
          this.pictureBox.Location = new System.Drawing.Point( 0, 28 );
          this.pictureBox.Name = "pictureBox";
          this.pictureBox.Size = new System.Drawing.Size( 512, 421 );
          this.pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
          this.pictureBox.TabIndex = 0;
          this.pictureBox.TabStop = false;
          this.pictureBox.Click += new System.EventHandler( this.pictureBox_Click );
          // 
          // toolStrip1
          // 
          this.toolStrip1.Items.AddRange( new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1} );
          this.toolStrip1.Location = new System.Drawing.Point( 0, 0 );
          this.toolStrip1.Name = "toolStrip1";
          this.toolStrip1.Size = new System.Drawing.Size( 512, 25 );
          this.toolStrip1.TabIndex = 1;
          this.toolStrip1.Text = "toolStrip1";
          // 
          // toolStripButton1
          // 
          this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
          this.toolStripButton1.Image = ( (System.Drawing.Image)( resources.GetObject( "toolStripButton1.Image" ) ) );
          this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
          this.toolStripButton1.Name = "toolStripButton1";
          this.toolStripButton1.Size = new System.Drawing.Size( 23, 22 );
          this.toolStripButton1.Text = "toolStripButton1";
          // 
          // Form1
          // 
          this.AutoScaleDimensions = new System.Drawing.SizeF( 6F, 13F );
          this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
          this.ClientSize = new System.Drawing.Size( 512, 449 );
          this.Controls.Add( this.toolStrip1 );
          this.Controls.Add( this.pictureBox );
          this.Name = "Form1";
          this.Text = "Voronoi";
          ( (System.ComponentModel.ISupportInitialize)( this.pictureBox ) ).EndInit();
          this.toolStrip1.ResumeLayout( false );
          this.toolStrip1.PerformLayout();
          this.ResumeLayout( false );
          this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox;
      private System.Windows.Forms.ToolStrip toolStrip1;
      private System.Windows.Forms.ToolStripButton toolStripButton1;
    }
}

