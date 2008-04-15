namespace aserio_cs370_project
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
    protected override void Dispose( bool disposing )
    {
      if ( disposing && ( components != null ) )
      {
        components.Dispose();
      }
      base.Dispose( disposing );
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.pictureBox = new System.Windows.Forms.PictureBox();
      this.videoPanel = new System.Windows.Forms.Panel();
      ( (System.ComponentModel.ISupportInitialize)( this.pictureBox ) ).BeginInit();
      this.SuspendLayout();
      // 
      // pictureBox
      // 
      this.pictureBox.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
      this.pictureBox.Dock = System.Windows.Forms.DockStyle.Fill;
      this.pictureBox.Location = new System.Drawing.Point( 0, 0 );
      this.pictureBox.Name = "pictureBox";
      this.pictureBox.Size = new System.Drawing.Size( 320, 240 );
      this.pictureBox.TabIndex = 0;
      this.pictureBox.TabStop = false;
      this.pictureBox.Click += new System.EventHandler( this.pictureBox_Click );
      // 
      // videoPanel
      // 
      this.videoPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.videoPanel.Enabled = false;
      this.videoPanel.Location = new System.Drawing.Point( 0, 0 );
      this.videoPanel.Name = "videoPanel";
      this.videoPanel.Size = new System.Drawing.Size( 320, 240 );
      this.videoPanel.TabIndex = 1;
      this.videoPanel.Visible = false;
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF( 6F, 13F );
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size( 320, 240 );
      this.Controls.Add( this.pictureBox );
      this.Controls.Add( this.videoPanel );
      this.Name = "Form1";
      this.Text = "GumBall!";
      this.TopMost = true;
      this.Load += new System.EventHandler( this.Form1_Load );
      ( (System.ComponentModel.ISupportInitialize)( this.pictureBox ) ).EndInit();
      this.ResumeLayout( false );

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBox;
    private System.Windows.Forms.Panel videoPanel;

  }
}

