namespace cs370
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
            this.labelImagePath = new System.Windows.Forms.Label();
            this.btnLoadImg = new System.Windows.Forms.Button();
            this.btnRestoreImg = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btnGetObjs = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btnOpenImg = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            this.panel1 = new System.Windows.Forms.Panel();
            this.checkBW = new System.Windows.Forms.CheckBox();
            this.radioSquare5 = new System.Windows.Forms.RadioButton();
            this.radioCircle7 = new System.Windows.Forms.RadioButton();
            this.radioSquare = new System.Windows.Forms.RadioButton();
            this.radioCircle = new System.Windows.Forms.RadioButton();
            this.radioCross = new System.Windows.Forms.RadioButton();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
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
            // labelImagePath
            // 
            this.labelImagePath.AutoSize = true;
            this.labelImagePath.Location = new System.Drawing.Point(9, 9);
            this.labelImagePath.Name = "labelImagePath";
            this.labelImagePath.Size = new System.Drawing.Size(87, 13);
            this.labelImagePath.TabIndex = 1;
            this.labelImagePath.Text = "No image loaded";
            // 
            // btnLoadImg
            // 
            this.btnLoadImg.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnLoadImg.Location = new System.Drawing.Point(538, 37);
            this.btnLoadImg.Name = "btnLoadImg";
            this.btnLoadImg.Size = new System.Drawing.Size(75, 23);
            this.btnLoadImg.TabIndex = 2;
            this.btnLoadImg.Text = "Load Image";
            this.btnLoadImg.UseVisualStyleBackColor = true;
            this.btnLoadImg.Click += new System.EventHandler(this.btnLoadImg_Click);
            // 
            // btnRestoreImg
            // 
            this.btnRestoreImg.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRestoreImg.Location = new System.Drawing.Point(538, 66);
            this.btnRestoreImg.Name = "btnRestoreImg";
            this.btnRestoreImg.Size = new System.Drawing.Size(75, 23);
            this.btnRestoreImg.TabIndex = 3;
            this.btnRestoreImg.Text = "Restore";
            this.btnRestoreImg.UseVisualStyleBackColor = true;
            this.btnRestoreImg.Click += new System.EventHandler(this.btnRestoreImg_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(535, 122);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Part 1";
            // 
            // btnGetObjs
            // 
            this.btnGetObjs.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnGetObjs.Location = new System.Drawing.Point(538, 138);
            this.btnGetObjs.Name = "btnGetObjs";
            this.btnGetObjs.Size = new System.Drawing.Size(75, 23);
            this.btnGetObjs.TabIndex = 5;
            this.btnGetObjs.Text = "Get Objects";
            this.btnGetObjs.UseVisualStyleBackColor = true;
            this.btnGetObjs.Click += new System.EventHandler(this.btnGetObjs_Click);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(535, 194);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Part 2";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 242);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Part 3";
            // 
            // btnOpenImg
            // 
            this.btnOpenImg.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOpenImg.Location = new System.Drawing.Point(6, 371);
            this.btnOpenImg.Name = "btnOpenImg";
            this.btnOpenImg.Size = new System.Drawing.Size(75, 23);
            this.btnOpenImg.TabIndex = 9;
            this.btnOpenImg.Text = "Apply Open";
            this.btnOpenImg.UseVisualStyleBackColor = true;
            this.btnOpenImg.Click += new System.EventHandler(this.btnOpenImg_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2,
            this.toolStripProgressBar1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 548);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(626, 22);
            this.statusStrip1.TabIndex = 10;
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
            // toolStripProgressBar1
            // 
            this.toolStripProgressBar1.Name = "toolStripProgressBar1";
            this.toolStripProgressBar1.Size = new System.Drawing.Size(100, 16);
            this.toolStripProgressBar1.Visible = false;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.checkBW);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.radioSquare5);
            this.panel1.Controls.Add(this.radioCircle7);
            this.panel1.Controls.Add(this.radioSquare);
            this.panel1.Controls.Add(this.radioCircle);
            this.panel1.Controls.Add(this.radioCross);
            this.panel1.Controls.Add(this.btnOpenImg);
            this.panel1.Location = new System.Drawing.Point(530, 25);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(91, 512);
            this.panel1.TabIndex = 11;
            // 
            // checkBW
            // 
            this.checkBW.AutoSize = true;
            this.checkBW.Location = new System.Drawing.Point(6, 183);
            this.checkBW.Name = "checkBW";
            this.checkBW.Size = new System.Drawing.Size(55, 17);
            this.checkBW.TabIndex = 15;
            this.checkBW.Text = "Binary";
            this.checkBW.UseVisualStyleBackColor = true;
            this.checkBW.CheckedChanged += new System.EventHandler(this.checkBW_CheckedChanged);
            // 
            // radioSquare5
            // 
            this.radioSquare5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radioSquare5.AutoSize = true;
            this.radioSquare5.Location = new System.Drawing.Point(6, 302);
            this.radioSquare5.Name = "radioSquare5";
            this.radioSquare5.Size = new System.Drawing.Size(85, 17);
            this.radioSquare5.TabIndex = 14;
            this.radioSquare5.TabStop = true;
            this.radioSquare5.Text = "Square (5x5)";
            this.radioSquare5.UseVisualStyleBackColor = true;
            this.radioSquare5.CheckedChanged += new System.EventHandler(this.radioSquare5_CheckedChanged);
            // 
            // radioCircle7
            // 
            this.radioCircle7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radioCircle7.AutoSize = true;
            this.radioCircle7.Location = new System.Drawing.Point(6, 348);
            this.radioCircle7.Name = "radioCircle7";
            this.radioCircle7.Size = new System.Drawing.Size(77, 17);
            this.radioCircle7.TabIndex = 13;
            this.radioCircle7.TabStop = true;
            this.radioCircle7.Text = "Circle (7x7)";
            this.radioCircle7.UseVisualStyleBackColor = true;
            this.radioCircle7.CheckedChanged += new System.EventHandler(this.radioCircle7_CheckedChanged);
            // 
            // radioSquare
            // 
            this.radioSquare.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radioSquare.AutoSize = true;
            this.radioSquare.Location = new System.Drawing.Point(6, 279);
            this.radioSquare.Name = "radioSquare";
            this.radioSquare.Size = new System.Drawing.Size(85, 17);
            this.radioSquare.TabIndex = 12;
            this.radioSquare.Text = "Square (3x3)";
            this.radioSquare.UseVisualStyleBackColor = true;
            this.radioSquare.CheckedChanged += new System.EventHandler(this.radioSquare_CheckedChanged);
            // 
            // radioCircle
            // 
            this.radioCircle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radioCircle.AutoSize = true;
            this.radioCircle.Location = new System.Drawing.Point(6, 325);
            this.radioCircle.Name = "radioCircle";
            this.radioCircle.Size = new System.Drawing.Size(77, 17);
            this.radioCircle.TabIndex = 11;
            this.radioCircle.Text = "Circle (5x5)";
            this.radioCircle.UseVisualStyleBackColor = true;
            this.radioCircle.CheckedChanged += new System.EventHandler(this.radioCircle_CheckedChanged);
            // 
            // radioCross
            // 
            this.radioCross.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radioCross.AutoSize = true;
            this.radioCross.Checked = true;
            this.radioCross.Location = new System.Drawing.Point(7, 258);
            this.radioCross.Name = "radioCross";
            this.radioCross.Size = new System.Drawing.Size(77, 17);
            this.radioCross.TabIndex = 10;
            this.radioCross.TabStop = true;
            this.radioCross.Text = "Cross (3x3)";
            this.radioCross.UseVisualStyleBackColor = true;
            this.radioCross.CheckedChanged += new System.EventHandler(this.radioCross_CheckedChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(626, 570);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnGetObjs);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnRestoreImg);
            this.Controls.Add(this.btnLoadImg);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.labelImagePath);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label labelImagePath;
        private System.Windows.Forms.Button btnLoadImg;
        private System.Windows.Forms.Button btnRestoreImg;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnGetObjs;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnOpenImg;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RadioButton radioSquare;
        private System.Windows.Forms.RadioButton radioCircle;
        private System.Windows.Forms.RadioButton radioCross;
        private System.Windows.Forms.RadioButton radioCircle7;
        private System.Windows.Forms.RadioButton radioSquare5;
        private System.Windows.Forms.CheckBox checkBW;
    }
}

