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

namespace cs370
{
    public partial class Form1 : Form
    {
        enum MORPH_MASK { CROSS, SQUARE, SQUARE5, CIRCLE5, CIRCLE7 }
        class Mask
        {
            public Mask(int w, int h, int x, int y)
            {
                this.x = x;
                this.y = y;
                this.w = w;
                this.h = h;
                data = new bool[w * h];

                for (int j = 0; j < h; ++j)
                {
                    for (int i = 0; i < w; ++i)
                        data[i + j * w] = true;
                }

                Set(x, y, false);
            }

            public bool Get(int x, int y)
            {
                return data[x + y * w];
            }

            public void Set(int x, int y, bool on)
            {
                data[x + y * w] = on;
            }

            public int x, y;
            public int w, h;
            public bool[] data;
        };

        Bitmap cachedBmp, dispBmp;
        float threshold = 0;
        int[] idTable;
        int nObjects = 0;
        MORPH_MASK maskID = MORPH_MASK.CROSS;

        public Form1()
        {
            InitializeComponent();
        }

        private void btnLoadImg_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Bitmap files (*.bmp)|*.bmp|JPEG files (*.jpg)|*.jpg|TIFF files (*.tif)|*.tif";
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
            dispBmp = new Bitmap(ofd.FileName);

            int widthDif = pictureBox1.Width - dispBmp.Width;
            int heightDif = pictureBox1.Height - dispBmp.Height;
            pictureBox1.SetBounds(pictureBox1.Bounds.X, pictureBox1.Bounds.Y, dispBmp.Width, dispBmp.Height);
            pictureBox1.Image = dispBmp;
            labelImagePath.Text = ofd.FileName;
            this.Width -= widthDif;
            this.Height -= heightDif;

            updateStatusBar();
        }

        private void btnRestoreImg_Click(object sender, EventArgs e)
        {
            dispBmp = cachedBmp;
            pictureBox1.Image = dispBmp;
        }

        private void btnGetObjs_Click(object sender, EventArgs e)
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
                    byte grayVal = (byte)(((int)srcColors[i + 2] +
                                           (int)srcColors[i + 1] +
                                           (int)srcColors[i]) / 3);
                    if (grayVal <= threshold)
                    {
                        polarColors[i + 2] = 0;
                        polarColors[i + 1] = 0;
                        polarColors[i] = 0;
                    }

                    else
                    {
                        polarColors[i + 2] = 255;
                        polarColors[i + 1] = 255;
                        polarColors[i] = 255;
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
                toolStripProgressBar1.Value = (int)((float)100 * (float)y / (float)height);

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

        private void btnOpenImg_Click(object sender, EventArgs e)
        {
            // crosshair mask
            Mask crossMask = new Mask(3, 3, 1, 1);
            crossMask.Set(0, 0, false);
            crossMask.Set(2, 0, false);
            crossMask.Set(0, 2, false);
            crossMask.Set(2, 2, false);

            // square mask 3x3
            Mask squareMask = new Mask(3, 3, 1, 1);

            // square mask 5x5
            Mask squareMask5 = new Mask(5, 5, 2, 2);

            // circle mask 5x5
            Mask circleMask5 = new Mask(5, 5, 2, 2);
            circleMask5.Set(0, 0, false);
            circleMask5.Set(4, 0, false);
            circleMask5.Set(0, 4, false);
            circleMask5.Set(4, 4, false);

            // circle mask 7x7
            Mask circleMask7 = new Mask(7, 7, 3, 3);
            circleMask7.Set(0, 0, false);
            circleMask7.Set(6, 0, false);
            circleMask7.Set(0, 6, false);
            circleMask7.Set(6, 6, false);

            Mask mask = crossMask;
            switch (maskID)
            {
                case MORPH_MASK.CROSS:   mask = crossMask;   break;
                case MORPH_MASK.SQUARE:  mask = squareMask;  break;
                case MORPH_MASK.SQUARE5: mask = squareMask5; break;
                case MORPH_MASK.CIRCLE5: mask = circleMask5; break;
                case MORPH_MASK.CIRCLE7: mask = circleMask7; break;
            }

            try
            {
                applyMask(mask, false); // erode
                applyMask(mask, true);  // dilate
            }
            catch (System.Exception exception)
            {
                MessageBox.Show(exception.Message);
            }
        }

        private void radioCross_CheckedChanged(object sender, EventArgs e)
        {
            maskID = MORPH_MASK.CROSS;
        }

        private void radioSquare_CheckedChanged(object sender, EventArgs e)
        {
            maskID = MORPH_MASK.SQUARE;
        }

        private void radioSquare5_CheckedChanged(object sender, EventArgs e)
        {
            maskID = MORPH_MASK.SQUARE5;
        }

        private void radioCircle_CheckedChanged(object sender, EventArgs e)
        {
            maskID = MORPH_MASK.CIRCLE5;
        }

        private void radioCircle7_CheckedChanged(object sender, EventArgs e)
        {
            maskID = MORPH_MASK.CIRCLE7;
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

            do
            {
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

        private void applyMask(Mask mask, bool dilate)
        {
            int width = dispBmp.Width, height = dispBmp.Height;

            Bitmap maskBmp = new Bitmap(width, height);

            BitmapData srcData = dispBmp.LockBits(new Rectangle(0, 0, width, height),
                                                  ImageLockMode.ReadOnly,
                                                  PixelFormat.Format24bppRgb);

            BitmapData dispData = maskBmp.LockBits(new Rectangle(0, 0, width, height),
                                                   ImageLockMode.WriteOnly,
                                                   PixelFormat.Format24bppRgb);

            System.IntPtr srcPtr = srcData.Scan0;
            System.IntPtr dispPtr = dispData.Scan0;
            int width3 = width * 3;
            int nBytes = width3 * height;
            byte[] srcColors = new byte[nBytes];
            byte[] dispColors = new byte[nBytes];

            Marshal.Copy(srcPtr, srcColors, 0, nBytes);
            Marshal.Copy(srcPtr, dispColors, 0, nBytes);

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width3; x += 3)
                {
                    int xmin = x - 3 * mask.x, xmax = 3 * (mask.w - mask.x) + x;
                    int ymin = y - mask.y, ymax = mask.h - mask.y + y;
                    int index = x + y * width3;

                    // ignore where mask goes off image
                    if (xmin < 0 || xmax >= width3 || ymin < 0 || ymax >= height) continue;

                    // ignore whites if eroding, blacks if dilating
                    if ((srcColors[index] == 0 && dilate) || (srcColors[index] != 0 && !dilate)) continue;

                    for (int j = ymin, jm = 0; j < ymax; ++j, ++jm)
                    {
                        for (int i = xmin, im = 0; i < xmax; i += 3, ++im)
                        {
                            if (mask.Get(im, jm))
                            {
                                index = i + j * width3;
                                byte fill = (dilate) ? (byte)0xFF : (byte)0;
                                dispColors[index] = fill;
                                dispColors[index + 1] = fill;
                                dispColors[index + 2] = fill;
                            }
                        }
                    }
                }
            }

            Marshal.Copy(dispColors, 0, dispPtr, nBytes);
            dispBmp.UnlockBits(srcData);
            maskBmp.UnlockBits(dispData);

            dispBmp = maskBmp;
            pictureBox1.Image = dispBmp;
        }
    }
}