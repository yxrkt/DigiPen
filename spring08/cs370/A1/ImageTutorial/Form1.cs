using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.IO;


namespace ImageApp
{
    public partial class Form1 : Form
    {
        Bitmap cachedBmp, sourceBmp, destBmp;
        int threshold = 195;
        int[] idTable;
        int nObjects = 0;

        private void shrink_zoom(ref Bitmap src, ref Bitmap dest, int destRes)
        {
            int factor;

            // dest size < source size
            if (src.Height >= destRes)
            {
                factor = src.Height / destRes;
                dest = new Bitmap(destRes, destRes);

                //
                BitmapData destData = dest.LockBits(new Rectangle(0, 0, dest.Width, dest.Height),
                                                    ImageLockMode.ReadWrite,
                                                    PixelFormat.Format24bppRgb);
                BitmapData srcData  = src.LockBits(new Rectangle(0, 0, src.Width, src.Height),
                                                   ImageLockMode.ReadOnly,
                                                   PixelFormat.Format24bppRgb);
                System.IntPtr destPtr = destData.Scan0;
                System.IntPtr srcPtr = srcData.Scan0;
                int nBytes = destRes * destRes * 3;
                int srcWidth = src.Width, srcHeight = src.Height;
                byte[] destColors = new byte[nBytes];
                byte[] srcColors = new byte[srcWidth * srcHeight * 3];

                Marshal.Copy(srcPtr, srcColors, 0, srcWidth * srcHeight * 3);
                //

                int xWalk = destRes * 3;

                for (int y = 0; y < destRes; ++y)
                {
                    for (int x = 0; x < xWalk; x += 3)
                    {
                        int iB = x + xWalk * y,     iB2 = factor * (x + 3 * srcWidth * y);
                        int iG = x + xWalk * y + 1, iG2 = factor * (x + 3 * srcWidth * y) + 1;
                        int iR = x + xWalk * y + 2, iR2 = factor * (x + 3 * srcWidth * y) + 2;

                        destColors[iR] = srcColors[iR2];
                        destColors[iG] = srcColors[iG2];
                        destColors[iB] = srcColors[iB2];
                    }
                }
                //
                Marshal.Copy(destColors, 0, destPtr, nBytes);
                dest.UnlockBits(destData);
                src.UnlockBits(srcData);
                //
            }

            // dest size > source size
            else
            {
                factor = destRes / src.Height;

                //
                BitmapData destData = dest.LockBits(new Rectangle(0, 0, dest.Width, dest.Height),
                                                    ImageLockMode.ReadWrite,
                                                    PixelFormat.Format24bppRgb);
                BitmapData srcData = src.LockBits(new Rectangle(0, 0, src.Width, src.Height),
                                                  ImageLockMode.ReadOnly,
                                                  PixelFormat.Format24bppRgb);
                System.IntPtr destPtr = destData.Scan0;
                System.IntPtr srcPtr = srcData.Scan0;
                int nBytes = destRes * destRes * 3;
                int srcWidth = src.Width, srcHeight = src.Height;
                byte[] destColors = new byte[nBytes];
                byte[] srcColors = new byte[srcWidth * srcHeight * 3];

                Marshal.Copy(srcPtr, srcColors, 0, srcWidth * srcHeight * 3);
                //

                for (int y = 0; y < srcHeight; ++y)
                {
                    for (int x = 0; x < srcWidth * 3; x+=3)
                    {
                        int iBSrc = x + y * srcWidth * 3;
                        int iGSrc = iBSrc + 1;
                        int iRSrc = iBSrc + 2;

                        for (int i = 0; i < factor; ++i)
                        {
                            for (int j = 0; j < factor * 3; j+=3)
                            {
                                int iB = (factor * x + j) + (factor * y + i) * destRes * 3;
                                int iG = iB + 1;
                                int iR = iB + 2;

                                destColors[iR] = srcColors[iRSrc];
                                destColors[iG] = srcColors[iGSrc];
                                destColors[iB] = srcColors[iBSrc];
                            }
                        }
                    }
                }

                Marshal.Copy(destColors, 0, destPtr, nBytes);
                dest.UnlockBits(destData);
                src.UnlockBits(srcData);
            }

            updateStatusBar();
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            trackBar1.Value = threshold;
            thresholdText.Text = "Threshold: " + threshold.ToString();
        }

        private void loadBmpBtn_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Bitmap files (*.bmp)|*.bmp";
            ofd.FilterIndex = 0;
            if (ofd.ShowDialog() != DialogResult.OK) return;

            Stream stream;

            try
            {
                stream = ofd.OpenFile();
            }

            catch (Exception exception)
            {
                string errMsg = exception.Message + "\nCannot open " + ofd.FileName;
                MessageBox.Show(errMsg, "Open error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            cachedBmp = new Bitmap(ofd.FileName);
            sourceBmp = new Bitmap(ofd.FileName);
            destBmp   = new Bitmap(ofd.FileName);
            pictureBox1.Image = destBmp;
            imageLabel.Text = ofd.FileName;
            
            updateStatusBar();
        }

        private void buttonClicked(int res)
        {
            if (sourceRadio.Checked)
            {
                shrink_zoom(ref cachedBmp, ref sourceBmp, res);
                shrink_zoom(ref sourceBmp, ref destBmp, destBmp.Width);
            }

            else if (destRadio.Checked)
            {
                shrink_zoom(ref sourceBmp, ref destBmp, res);
            }

            else
            {
                shrink_zoom(ref cachedBmp, ref sourceBmp, res);
                shrink_zoom(ref sourceBmp, ref destBmp, res);
            }

            pictureBox1.Image = destBmp;
        }

        private void res512_Click(object sender, EventArgs e)
        {
            buttonClicked(512);
        }

        private void res256_Click(object sender, EventArgs e)
        {
            buttonClicked(256);
        }

        private void res128_Click(object sender, EventArgs e)
        {
            buttonClicked(128);
        }

        private void res64_Click(object sender, EventArgs e)
        {
            buttonClicked(64);
        }

        private void res32_Click(object sender, EventArgs e)
        {
            buttonClicked(32);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            pictureBox1.Image = sourceBmp;
        }

        private void updateStatusBar()
        {
            string srcResString = "Source image at ";
            srcResString += ( sourceBmp.Height + "x" + sourceBmp.Width );
            toolStripStatusLabel1.Text = srcResString;

            string destResString = "Destination image at ";
            destResString += ( destBmp.Height + "x" + destBmp.Width );
            toolStripStatusLabel2.Text = destResString;

            if (nObjects != 0)
            {
                string objsString = "Objects total: ";
                objsString += nObjects.ToString();
                toolStripStatusLabel3.Text = objsString;
            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            threshold = trackBar1.Value;
            thresholdText.Text = "Threshold : " + trackBar1.Value.ToString();
        }

        private void updateLabels(int labelKeep, int labelReplace, int xLast, int yLast)
        {
            int dim = destBmp.Width;
            int xLim = dim * 3;

            for (int y = 0; y <= yLast; ++y)
            {
                if (y == yLast) xLim = xLast + 3;

                for (int x = 0; x < xLim; x += 3)
                {
                    int index = x / 3 + y * dim;

                    if (idTable[index] == labelReplace)
                        idTable[index] = labelKeep;
                }
            }

            nObjects--;
        }

        private void applyBtn_Click(object sender, EventArgs e)
        {
            int width = destBmp.Width, height = destBmp.Height;

            BitmapData srcData = destBmp.LockBits(new Rectangle(0, 0, width, height),
                                                  ImageLockMode.ReadOnly,
                                                  PixelFormat.Format24bppRgb);

            Bitmap polar = new Bitmap(width, height);

            BitmapData polarData = polar.LockBits(new Rectangle(0, 0, width, height),
                                                  ImageLockMode.WriteOnly,
                                                  PixelFormat.Format24bppRgb);

            System.IntPtr srcPtr = srcData.Scan0;
            System.IntPtr polarPtr = polarData.Scan0;
            int nBytes = width * height * 3;
            byte[] srcColors = new byte[nBytes];
            byte[] polarColors = new byte[nBytes];

            Marshal.Copy(srcPtr, srcColors, 0, nBytes);
            
            //
            int xLim = width * 3;

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < xLim; x += 3)
                {
                    int i = x + y * xLim;
                    byte grayVal = (byte)(((int) srcColors[i + 2] + 
                                           (int) srcColors[i + 1] + 
                                           (int) srcColors[i]) / 3);
                    if (grayVal <= threshold)
                    {
                        polarColors[i + 2] = 0;
                        polarColors[i + 1] = 0;
                        polarColors[i]     = 0;
                    }

                    else
                    {
                        polarColors[i + 2] = 255;
                        polarColors[i + 1] = 255;
                        polarColors[i]     = 255;
                    }
                }
            }

            //
            Marshal.Copy(polarColors, 0, polarPtr, nBytes);
            destBmp.UnlockBits(srcData);
            polar.UnlockBits(polarData);

            if (polarizedRadio.Checked)
            {
                pictureBox1.Image = polar;
            }

            else
            {
                idTable = new int[width * height];

                polarData = polar.LockBits(new Rectangle(0, 0, width, height),
                                           ImageLockMode.ReadOnly,
                                           PixelFormat.Format24bppRgb);

                Bitmap objs = new Bitmap(width, height);

                BitmapData objsData = objs.LockBits(new Rectangle(0, 0, width, height),
                                                    ImageLockMode.WriteOnly,
                                                    PixelFormat.Format24bppRgb);

                polarPtr = polarData.Scan0;
                System.IntPtr objsPtr = objsData.Scan0;
                byte[] objsColors = new byte[nBytes];

                Marshal.Copy(polarPtr, polarColors, 0, nBytes);

                //
                toolStripProgressBar1.Visible = true;

                nObjects = 0;

                for (int y = 0; y < height; ++y)
                {
                    toolStripProgressBar1.Value = (int) ((float) 100 * (float) y / (float) height);

                    for (int x = 0; x < xLim; x += 3)
                    {
                        int colorIndexA = x + y * xLim;
                        int colorIndexB = (x - 3) + y * xLim;
                        int colorIndexC = x + (y - 1) * xLim;
                        int colorIndexD = (x - 3) + (y - 1) * xLim;

                        int labelIndexA = x / 3 + y * width;
                        int labelIndexB = (x / 3 - 1) + y * width;
                        int labelIndexC = x / 3 + (y - 1) * width;
                        int labelIndexD = (x / 3 - 1) + (y - 1) * width;

                        int labeled = -1, oldLabel = -1;
                        bool runUpdate = false;

                        // check 1: top left (D)
                        if (x != 0 && y != 0)
                        {
                            if (polarColors[colorIndexA] == polarColors[colorIndexD])
                            {
                                idTable[labelIndexA] = idTable[labelIndexD];
                                labeled = idTable[labelIndexA];
                            }
                        }

                        // check 2: top (C)
                        if (y != 0)
                        {
                            if (polarColors[colorIndexA] == polarColors[colorIndexC])
                            {
                                idTable[labelIndexA] = idTable[labelIndexC];
                                if (labeled != -1)
                                {
                                    if (labeled != idTable[labelIndexA])
                                    {
                                        runUpdate = true;
                                        oldLabel = Math.Max(labeled, idTable[labelIndexA]);
                                        labeled = Math.Min(labeled, idTable[labelIndexA]);
                                    }
                                }

                                else
                                {
                                    labeled = idTable[labelIndexA];
                                }
                            }
                        }

                        // check 3: left (B)
                        if (x != 0)
                        {
                            if (polarColors[colorIndexA] == polarColors[colorIndexB])
                            {
                                idTable[labelIndexA] = idTable[labelIndexB];
                                if (labeled != -1)
                                {
                                    if (labeled != idTable[labelIndexA])
                                    {
                                        runUpdate = true;
                                        oldLabel = Math.Max(labeled, idTable[labelIndexA]);
                                        labeled = Math.Min(labeled, idTable[labelIndexA]);
                                    }
                                }

                                else
                                {
                                    labeled = idTable[labelIndexA];
                                }
                            }
                        }

                        // if no friends, give new label
                        if (labeled == -1)
                            idTable[labelIndexA] = nObjects++;

                        if (runUpdate)
                            updateLabels(labeled, oldLabel, x, y);
                    }
                }

                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < xLim; x += 3)
                    {
                        int colorIndex = x + y * xLim;
                        int idIndex = x / 3 + y * width;

                        objsColors[colorIndex + 2] = (byte)((977 * idTable[idIndex]) + 173 % 256);
                        objsColors[colorIndex + 1] = (byte)((644 * idTable[idIndex]) + 45 % 256);
                        objsColors[colorIndex] = (byte)((311 * idTable[idIndex]) + 247 % 256);
                    }
                }

                //
                Marshal.Copy(objsColors, 0, objsPtr, nBytes);
                polar.UnlockBits(polarData);
                objs.UnlockBits(objsData);

                pictureBox1.Image = objs;
                updateStatusBar();

                toolStripProgressBar1.Visible = false;
            }
        }
    }
}