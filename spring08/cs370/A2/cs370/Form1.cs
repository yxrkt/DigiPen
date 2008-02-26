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
        Bitmap cachedBmp, dispBmp;
        float threshold = 0;
        int[] idTable;
        int nObjects = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
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
            dispBmp   = new Bitmap(ofd.FileName);
            pictureBox1.Image = dispBmp;
            imageLabel.Text   = ofd.FileName;
            
            updateStatusBar();
        }

        private void updateStatusBar()
        {
            if (nObjects != 0)
            {
                string objsString = "Objects total: ";
                objsString += nObjects.ToString();
                toolStripStatusLabel1.Text = objsString;
            }
            else
            {
                toolStripStatusLabel1.Text = "";
            }

            if (threshold != 0.0F)
            {
                toolStripStatusLabel2.Text = "Threshold at ";
                toolStripStatusLabel2.Text += threshold.ToString();
            }
            else
            {
                toolStripStatusLabel2.Text = "";
            }
        }

        private void updateLabels(int labelKeep, int labelReplace, int xLast, int yLast)
        {
            int dim = dispBmp.Width;
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
            int width = dispBmp.Width, height = dispBmp.Height;

            BitmapData srcData = dispBmp.LockBits(new Rectangle(0, 0, width, height),
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
            threshold = getThreshold(srcColors, 1.0F);
            
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

            Marshal.Copy(polarColors, 0, polarPtr, nBytes);
            dispBmp.UnlockBits(srcData);
            polar.UnlockBits(polarData);

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

        private void restoreImgBtn_Click(object sender, EventArgs e)
        {
            dispBmp = cachedBmp;
            pictureBox1.Image = dispBmp;
        }

        private float getThreshold(byte[] rgbs, float termVal)
        {
            int nBytes = rgbs.Length;
            int nGrays = nBytes / 3;

            // Create array of grays
            float[] grays = new float[nGrays];

            // Starting gray value: mean gray val
            float avgGray = 0;

            for (int i = 0, j = 0; i < nBytes; i += 3)
            {
                float curGray = (float)((int)rgbs[i] + (int)rgbs[i + 1] + (int)rgbs[i + 2]) / 3.0F;
                grays[j++] = curGray;
                avgGray += curGray;
            }

            avgGray /= nGrays;

            float curThresh = avgGray, prevThresh;

            do {
                // Get values for groups using cur threshold
                prevThresh = curThresh;
                float G1 = 0.0F, G2 = 0.0F;
                float nG1 = 0.0F, nG2 = 0.0F;
                for (int i = 0; i < nGrays; ++i)
                {
                    if (grays[i] < curThresh)
                    {
                        G1 += grays[i];
                        nG1 += 1.0F;
                    }
                    else
                    {
                        G2 += grays[i];
                        nG2 += 1.0F;
                    }
                }
                curThresh = ((G1 / nG1) + (G2 / nG2)) / 2.0F;
            } while (Math.Abs(prevThresh - curThresh) > termVal);

            return curThresh;
        }
    }
}