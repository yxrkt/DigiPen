namespace ImageApp
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
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.loadBmpBtn = new System.Windows.Forms.Button();
            this.imageLabel = new System.Windows.Forms.Label();
            this.sourceRadio = new System.Windows.Forms.RadioButton();
            this.destRadio = new System.Windows.Forms.RadioButton();
            this.bothRadio = new System.Windows.Forms.RadioButton();
            this.res512 = new System.Windows.Forms.Button();
            this.res256 = new System.Windows.Forms.Button();
            this.res128 = new System.Windows.Forms.Button();
            this.res64 = new System.Windows.Forms.Button();
            this.res32 = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.objectsRadio = new System.Windows.Forms.RadioButton();
            this.polarizedRadio = new System.Windows.Forms.RadioButton();
            this.applyBtn = new System.Windows.Forms.Button();
            this.thresholdText = new System.Windows.Forms.Label();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.Info;
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pictureBox1.Location = new System.Drawing.Point(12, 25);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(512, 512);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // loadBmpBtn
            // 
            this.loadBmpBtn.Location = new System.Drawing.Point(19, 37);
            this.loadBmpBtn.Name = "loadBmpBtn";
            this.loadBmpBtn.Size = new System.Drawing.Size(127, 30);
            this.loadBmpBtn.TabIndex = 3;
            this.loadBmpBtn.Text = "Load Bitmap";
            this.loadBmpBtn.UseVisualStyleBackColor = true;
            this.loadBmpBtn.Click += new System.EventHandler(this.loadBmpBtn_Click);
            // 
            // imageLabel
            // 
            this.imageLabel.AutoSize = true;
            this.imageLabel.Location = new System.Drawing.Point(9, 9);
            this.imageLabel.Name = "imageLabel";
            this.imageLabel.Size = new System.Drawing.Size(95, 13);
            this.imageLabel.TabIndex = 4;
            this.imageLabel.Text = "No Bitmap Loaded";
            // 
            // sourceRadio
            // 
            this.sourceRadio.AutoSize = true;
            this.sourceRadio.Location = new System.Drawing.Point(19, 181);
            this.sourceRadio.Name = "sourceRadio";
            this.sourceRadio.Size = new System.Drawing.Size(59, 17);
            this.sourceRadio.TabIndex = 5;
            this.sourceRadio.Text = "Source";
            this.sourceRadio.UseVisualStyleBackColor = true;
            // 
            // destRadio
            // 
            this.destRadio.AutoSize = true;
            this.destRadio.Location = new System.Drawing.Point(19, 204);
            this.destRadio.Name = "destRadio";
            this.destRadio.Size = new System.Drawing.Size(78, 17);
            this.destRadio.TabIndex = 6;
            this.destRadio.Text = "Destination";
            this.destRadio.UseVisualStyleBackColor = true;
            // 
            // bothRadio
            // 
            this.bothRadio.AutoSize = true;
            this.bothRadio.Checked = true;
            this.bothRadio.Location = new System.Drawing.Point(19, 227);
            this.bothRadio.Name = "bothRadio";
            this.bothRadio.Size = new System.Drawing.Size(47, 17);
            this.bothRadio.TabIndex = 7;
            this.bothRadio.TabStop = true;
            this.bothRadio.Text = "Both";
            this.bothRadio.UseVisualStyleBackColor = true;
            // 
            // res512
            // 
            this.res512.Location = new System.Drawing.Point(19, 73);
            this.res512.Name = "res512";
            this.res512.Size = new System.Drawing.Size(62, 30);
            this.res512.TabIndex = 8;
            this.res512.Text = "512 x 512";
            this.res512.UseVisualStyleBackColor = true;
            this.res512.Click += new System.EventHandler(this.res512_Click);
            // 
            // res256
            // 
            this.res256.Location = new System.Drawing.Point(19, 109);
            this.res256.Name = "res256";
            this.res256.Size = new System.Drawing.Size(62, 30);
            this.res256.TabIndex = 9;
            this.res256.Text = "256 x 256";
            this.res256.UseVisualStyleBackColor = true;
            this.res256.Click += new System.EventHandler(this.res256_Click);
            // 
            // res128
            // 
            this.res128.Location = new System.Drawing.Point(19, 145);
            this.res128.Name = "res128";
            this.res128.Size = new System.Drawing.Size(62, 30);
            this.res128.TabIndex = 10;
            this.res128.Text = "128 x 128";
            this.res128.UseVisualStyleBackColor = true;
            this.res128.Click += new System.EventHandler(this.res128_Click);
            // 
            // res64
            // 
            this.res64.Location = new System.Drawing.Point(82, 73);
            this.res64.Name = "res64";
            this.res64.Size = new System.Drawing.Size(62, 30);
            this.res64.TabIndex = 11;
            this.res64.Text = "64 x 64";
            this.res64.UseVisualStyleBackColor = true;
            this.res64.Click += new System.EventHandler(this.res64_Click);
            // 
            // res32
            // 
            this.res32.Location = new System.Drawing.Point(82, 109);
            this.res32.Name = "res32";
            this.res32.Size = new System.Drawing.Size(62, 30);
            this.res32.TabIndex = 12;
            this.res32.Text = "32 x 32";
            this.res32.UseVisualStyleBackColor = true;
            this.res32.Click += new System.EventHandler(this.res32_Click);
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.res32);
            this.panel1.Controls.Add(this.res64);
            this.panel1.Controls.Add(this.res128);
            this.panel1.Controls.Add(this.res256);
            this.panel1.Controls.Add(this.res512);
            this.panel1.Controls.Add(this.bothRadio);
            this.panel1.Controls.Add(this.destRadio);
            this.panel1.Controls.Add(this.sourceRadio);
            this.panel1.Controls.Add(this.loadBmpBtn);
            this.panel1.Location = new System.Drawing.Point(534, 25);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(162, 254);
            this.panel1.TabIndex = 13;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 13);
            this.label1.TabIndex = 13;
            this.label1.Text = "Zoom-Shrink";
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel2.Controls.Add(this.objectsRadio);
            this.panel2.Controls.Add(this.polarizedRadio);
            this.panel2.Controls.Add(this.applyBtn);
            this.panel2.Controls.Add(this.thresholdText);
            this.panel2.Controls.Add(this.trackBar1);
            this.panel2.Location = new System.Drawing.Point(534, 285);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(161, 251);
            this.panel2.TabIndex = 14;
            // 
            // objectsRadio
            // 
            this.objectsRadio.AutoSize = true;
            this.objectsRadio.Location = new System.Drawing.Point(19, 181);
            this.objectsRadio.Name = "objectsRadio";
            this.objectsRadio.Size = new System.Drawing.Size(61, 17);
            this.objectsRadio.TabIndex = 4;
            this.objectsRadio.Text = "Objects";
            this.objectsRadio.UseVisualStyleBackColor = true;
            // 
            // polarizedRadio
            // 
            this.polarizedRadio.AutoSize = true;
            this.polarizedRadio.Checked = true;
            this.polarizedRadio.Location = new System.Drawing.Point(19, 158);
            this.polarizedRadio.Name = "polarizedRadio";
            this.polarizedRadio.Size = new System.Drawing.Size(68, 17);
            this.polarizedRadio.TabIndex = 3;
            this.polarizedRadio.TabStop = true;
            this.polarizedRadio.Text = "Polarized";
            this.polarizedRadio.UseVisualStyleBackColor = true;
            // 
            // applyBtn
            // 
            this.applyBtn.Location = new System.Drawing.Point(17, 204);
            this.applyBtn.Name = "applyBtn";
            this.applyBtn.Size = new System.Drawing.Size(124, 32);
            this.applyBtn.TabIndex = 2;
            this.applyBtn.Text = "Apply";
            this.applyBtn.UseVisualStyleBackColor = true;
            this.applyBtn.Click += new System.EventHandler(this.applyBtn_Click);
            // 
            // thresholdText
            // 
            this.thresholdText.AutoSize = true;
            this.thresholdText.Location = new System.Drawing.Point(14, 12);
            this.thresholdText.Name = "thresholdText";
            this.thresholdText.Size = new System.Drawing.Size(69, 13);
            this.thresholdText.TabIndex = 1;
            this.thresholdText.Text = "Threshold (0)";
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(17, 28);
            this.trackBar1.Maximum = 255;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBar1.Size = new System.Drawing.Size(45, 124);
            this.trackBar1.SmallChange = 17;
            this.trackBar1.TabIndex = 0;
            this.trackBar1.TickFrequency = 17;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2,
            this.toolStripStatusLabel3,
            this.toolStripProgressBar1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 555);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(708, 22);
            this.statusStrip1.TabIndex = 16;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripStatusLabel3
            // 
            this.toolStripStatusLabel3.Name = "toolStripStatusLabel3";
            this.toolStripStatusLabel3.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripProgressBar1
            // 
            this.toolStripProgressBar1.Name = "toolStripProgressBar1";
            this.toolStripProgressBar1.Size = new System.Drawing.Size(100, 16);
            this.toolStripProgressBar1.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.toolStripProgressBar1.Visible = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(708, 577);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.imageLabel);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.Text = "CS 370 A1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button loadBmpBtn;
        private System.Windows.Forms.Label imageLabel;
        private System.Windows.Forms.RadioButton sourceRadio;
        private System.Windows.Forms.RadioButton destRadio;
        private System.Windows.Forms.RadioButton bothRadio;
        private System.Windows.Forms.Button res512;
        private System.Windows.Forms.Button res256;
        private System.Windows.Forms.Button res128;
        private System.Windows.Forms.Button res64;
        private System.Windows.Forms.Button res32;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.Label thresholdText;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.RadioButton polarizedRadio;
        private System.Windows.Forms.Button applyBtn;
        private System.Windows.Forms.RadioButton objectsRadio;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel3;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar1;
    }
}

