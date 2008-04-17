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
          this.toolStripButtonClear = new System.Windows.Forms.ToolStripButton();
          this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
          this.toolStripButtonAdd = new System.Windows.Forms.ToolStripButton();
          this.toolStripButtonRemove = new System.Windows.Forms.ToolStripButton();
          this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
          this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
          this.toolStripTextBox1 = new System.Windows.Forms.ToolStripTextBox();
          this.toolStripButtonUpdate = new System.Windows.Forms.ToolStripButton();
          this.statusStrip1 = new System.Windows.Forms.StatusStrip();
          this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
          this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
          this.toolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
          ( (System.ComponentModel.ISupportInitialize)( this.pictureBox ) ).BeginInit();
          this.toolStrip1.SuspendLayout();
          this.statusStrip1.SuspendLayout();
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
          this.pictureBox.Size = new System.Drawing.Size( 512, 388 );
          this.pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
          this.pictureBox.TabIndex = 0;
          this.pictureBox.TabStop = false;
          this.pictureBox.Click += new System.EventHandler( this.pictureBox_Click );
          // 
          // toolStrip1
          // 
          this.toolStrip1.Items.AddRange( new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonClear,
            this.toolStripSeparator2,
            this.toolStripButtonAdd,
            this.toolStripButtonRemove,
            this.toolStripSeparator1,
            this.toolStripLabel1,
            this.toolStripTextBox1,
            this.toolStripButtonUpdate} );
          this.toolStrip1.Location = new System.Drawing.Point( 0, 0 );
          this.toolStrip1.Name = "toolStrip1";
          this.toolStrip1.Size = new System.Drawing.Size( 512, 25 );
          this.toolStrip1.TabIndex = 1;
          this.toolStrip1.Text = "toolStrip1";
          // 
          // toolStripButtonClear
          // 
          this.toolStripButtonClear.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
          this.toolStripButtonClear.Image = ( (System.Drawing.Image)( resources.GetObject( "toolStripButtonClear.Image" ) ) );
          this.toolStripButtonClear.ImageTransparentColor = System.Drawing.Color.Magenta;
          this.toolStripButtonClear.Name = "toolStripButtonClear";
          this.toolStripButtonClear.Size = new System.Drawing.Size( 23, 22 );
          this.toolStripButtonClear.Text = "Reset";
          this.toolStripButtonClear.Click += new System.EventHandler( this.toolStripButtonClear_Click );
          // 
          // toolStripSeparator2
          // 
          this.toolStripSeparator2.Name = "toolStripSeparator2";
          this.toolStripSeparator2.Size = new System.Drawing.Size( 6, 25 );
          // 
          // toolStripButtonAdd
          // 
          this.toolStripButtonAdd.Checked = true;
          this.toolStripButtonAdd.CheckState = System.Windows.Forms.CheckState.Checked;
          this.toolStripButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
          this.toolStripButtonAdd.Image = ( (System.Drawing.Image)( resources.GetObject( "toolStripButtonAdd.Image" ) ) );
          this.toolStripButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
          this.toolStripButtonAdd.Name = "toolStripButtonAdd";
          this.toolStripButtonAdd.Size = new System.Drawing.Size( 23, 22 );
          this.toolStripButtonAdd.Text = "Add Points";
          this.toolStripButtonAdd.Click += new System.EventHandler( this.toolStripButtonAdd_Click );
          // 
          // toolStripButtonRemove
          // 
          this.toolStripButtonRemove.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
          this.toolStripButtonRemove.Image = ( (System.Drawing.Image)( resources.GetObject( "toolStripButtonRemove.Image" ) ) );
          this.toolStripButtonRemove.ImageTransparentColor = System.Drawing.Color.Magenta;
          this.toolStripButtonRemove.Name = "toolStripButtonRemove";
          this.toolStripButtonRemove.Size = new System.Drawing.Size( 23, 22 );
          this.toolStripButtonRemove.Text = "Remove Region";
          this.toolStripButtonRemove.Click += new System.EventHandler( this.toolStripButtonRemove_Click );
          // 
          // toolStripSeparator1
          // 
          this.toolStripSeparator1.Name = "toolStripSeparator1";
          this.toolStripSeparator1.Size = new System.Drawing.Size( 6, 25 );
          // 
          // toolStripLabel1
          // 
          this.toolStripLabel1.Name = "toolStripLabel1";
          this.toolStripLabel1.Size = new System.Drawing.Size( 16, 22 );
          this.toolStripLabel1.Text = "k:";
          // 
          // toolStripTextBox1
          // 
          this.toolStripTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
          this.toolStripTextBox1.MaxLength = 2;
          this.toolStripTextBox1.Name = "toolStripTextBox1";
          this.toolStripTextBox1.Size = new System.Drawing.Size( 25, 25 );
          // 
          // toolStripButtonUpdate
          // 
          this.toolStripButtonUpdate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
          this.toolStripButtonUpdate.Image = ( (System.Drawing.Image)( resources.GetObject( "toolStripButtonUpdate.Image" ) ) );
          this.toolStripButtonUpdate.ImageTransparentColor = System.Drawing.Color.Magenta;
          this.toolStripButtonUpdate.Name = "toolStripButtonUpdate";
          this.toolStripButtonUpdate.Size = new System.Drawing.Size( 23, 22 );
          this.toolStripButtonUpdate.Text = "Update";
          this.toolStripButtonUpdate.Click += new System.EventHandler( this.toolStripButtonUpdate_Click );
          // 
          // statusStrip1
          // 
          this.statusStrip1.Items.AddRange( new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2,
            this.toolStripProgressBar1} );
          this.statusStrip1.Location = new System.Drawing.Point( 0, 419 );
          this.statusStrip1.Name = "statusStrip1";
          this.statusStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
          this.statusStrip1.Size = new System.Drawing.Size( 512, 22 );
          this.statusStrip1.TabIndex = 2;
          this.statusStrip1.Text = "statusStrip1";
          // 
          // toolStripStatusLabel1
          // 
          this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
          this.toolStripStatusLabel1.Size = new System.Drawing.Size( 0, 17 );
          // 
          // toolStripStatusLabel2
          // 
          this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
          this.toolStripStatusLabel2.Size = new System.Drawing.Size( 0, 17 );
          // 
          // toolStripProgressBar1
          // 
          this.toolStripProgressBar1.Name = "toolStripProgressBar1";
          this.toolStripProgressBar1.Size = new System.Drawing.Size( 100, 16 );
          this.toolStripProgressBar1.Visible = false;
          // 
          // Form1
          // 
          this.AutoScaleDimensions = new System.Drawing.SizeF( 6F, 13F );
          this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
          this.ClientSize = new System.Drawing.Size( 512, 441 );
          this.Controls.Add( this.statusStrip1 );
          this.Controls.Add( this.toolStrip1 );
          this.Controls.Add( this.pictureBox );
          this.Name = "Form1";
          this.Text = "Voronoi";
          this.Load += new System.EventHandler( this.Form1_Load );
          ( (System.ComponentModel.ISupportInitialize)( this.pictureBox ) ).EndInit();
          this.toolStrip1.ResumeLayout( false );
          this.toolStrip1.PerformLayout();
          this.statusStrip1.ResumeLayout( false );
          this.statusStrip1.PerformLayout();
          this.ResumeLayout( false );
          this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox;
      private System.Windows.Forms.ToolStrip toolStrip1;
      private System.Windows.Forms.ToolStripButton toolStripButtonAdd;
      private System.Windows.Forms.StatusStrip statusStrip1;
      private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
      private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
      private System.Windows.Forms.ToolStripTextBox toolStripTextBox1;
      private System.Windows.Forms.ToolStripLabel toolStripLabel1;
      private System.Windows.Forms.ToolStripButton toolStripButtonUpdate;
      private System.Windows.Forms.ToolStripButton toolStripButtonRemove;
      private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
      private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar1;
      private System.Windows.Forms.ToolStripButton toolStripButtonClear;
      private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
    }
}

