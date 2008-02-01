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
            this.button1 = new System.Windows.Forms.Button();
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
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(12, 25);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(512, 512);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(559, 507);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(127, 30);
            this.button1.TabIndex = 2;
            this.button1.Text = "Convert";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // loadBmpBtn
            // 
            this.loadBmpBtn.Location = new System.Drawing.Point(559, 38);
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
            this.sourceRadio.Location = new System.Drawing.Point(559, 182);
            this.sourceRadio.Name = "sourceRadio";
            this.sourceRadio.Size = new System.Drawing.Size(59, 17);
            this.sourceRadio.TabIndex = 5;
            this.sourceRadio.Text = "Source";
            this.sourceRadio.UseVisualStyleBackColor = true;
            // 
            // destRadio
            // 
            this.destRadio.AutoSize = true;
            this.destRadio.Location = new System.Drawing.Point(559, 205);
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
            this.bothRadio.Location = new System.Drawing.Point(559, 228);
            this.bothRadio.Name = "bothRadio";
            this.bothRadio.Size = new System.Drawing.Size(47, 17);
            this.bothRadio.TabIndex = 7;
            this.bothRadio.TabStop = true;
            this.bothRadio.Text = "Both";
            this.bothRadio.UseVisualStyleBackColor = true;
            // 
            // res512
            // 
            this.res512.Location = new System.Drawing.Point(559, 74);
            this.res512.Name = "res512";
            this.res512.Size = new System.Drawing.Size(62, 30);
            this.res512.TabIndex = 8;
            this.res512.Text = "512 x 512";
            this.res512.UseVisualStyleBackColor = true;
            this.res512.Click += new System.EventHandler(this.res512_Click);
            // 
            // res256
            // 
            this.res256.Location = new System.Drawing.Point(559, 110);
            this.res256.Name = "res256";
            this.res256.Size = new System.Drawing.Size(62, 30);
            this.res256.TabIndex = 9;
            this.res256.Text = "256 x 256";
            this.res256.UseVisualStyleBackColor = true;
            this.res256.Click += new System.EventHandler(this.res256_Click);
            // 
            // res128
            // 
            this.res128.Location = new System.Drawing.Point(559, 146);
            this.res128.Name = "res128";
            this.res128.Size = new System.Drawing.Size(62, 30);
            this.res128.TabIndex = 10;
            this.res128.Text = "128 x 128";
            this.res128.UseVisualStyleBackColor = true;
            this.res128.Click += new System.EventHandler(this.res128_Click);
            // 
            // res64
            // 
            this.res64.Location = new System.Drawing.Point(622, 74);
            this.res64.Name = "res64";
            this.res64.Size = new System.Drawing.Size(62, 30);
            this.res64.TabIndex = 11;
            this.res64.Text = "64 x 64";
            this.res64.UseVisualStyleBackColor = true;
            this.res64.Click += new System.EventHandler(this.res64_Click);
            // 
            // res32
            // 
            this.res32.Location = new System.Drawing.Point(622, 110);
            this.res32.Name = "res32";
            this.res32.Size = new System.Drawing.Size(62, 30);
            this.res32.TabIndex = 12;
            this.res32.Text = "32 x 32";
            this.res32.UseVisualStyleBackColor = true;
            this.res32.Click += new System.EventHandler(this.res32_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(698, 549);
            this.Controls.Add(this.res32);
            this.Controls.Add(this.res64);
            this.Controls.Add(this.res128);
            this.Controls.Add(this.res256);
            this.Controls.Add(this.res512);
            this.Controls.Add(this.bothRadio);
            this.Controls.Add(this.destRadio);
            this.Controls.Add(this.sourceRadio);
            this.Controls.Add(this.imageLabel);
            this.Controls.Add(this.loadBmpBtn);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.Text = "CS 370 A1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button button1;
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
    }
}

