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
    enum NEIGHBOR { NONE = -1, B = -2, C = -3, D = -4 };
    
    public partial class Form1 : Form
    {
        Bitmap cachedBmp, sourceBmp, destBmp;
        int threshold = 0;
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
            sourceBmp = cachedBmp;
            destBmp   = sourceBmp;
            pictureBox1.Image = destBmp;
            //string resInfo = new string("source: " + sourceBmp.Width + "x" + sourceBmp.Height
            //                            + " destination: " + destBmp.Width + "x" + destBmp.Height);
            imageLabel.Text = ofd.FileName;// +resInfo;
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
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            threshold = trackBar1.Value;
            thresholdText.Text = "Threshold (" + trackBar1.Value.ToString() + ")";
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
                int curId = 0;

                //for (int y = 0; y < height; ++y)
                //{
                //    for (int x = 0; x < xLim; x += 3)
                //    {
                //        int i = x + y * xLim;

                //        // check 1: left
                //        if (x != 0)
                //        {
                //            if (polarColors[i] == polarColors[(x - 3) + y * xLim])
                //            {
                //                idTable[x / 3 + y * width] = idTable[x / 3 - 1 + y * width];
                //                continue;
                //            }
                //        }

                //        // check 2: top left
                //        if (x != 0 && y != 0)
                //        {
                //            if (polarColors[i] == polarColors[(x - 3) + (y - 1) * xLim])
                //            {
                //                idTable[x / 3 + y * width] = idTable[x / 3 - 1 + (y - 1) * width];
                //                continue;
                //            }
                //        }

                //        // check 3: top
                //        if (y != 0)
                //        {
                //            if (polarColors[i] == polarColors[x + (y - 1) * xLim])
                //            {
                //                idTable[x / 3 + y * width] = idTable[x / 3 + (y - 1) * width];
                //                continue;
                //            }
                //        }

                //        //// check 4: right
                //        //if (x < xLim)
                //        //{
                //        //    if (polarColors[i] == polarColors[(x + 3) + y * xLim])
                //        //    {
                //        //        idTable[x / 3 + y * width] = curId;
                //        //        continue;
                //        //    }
                //        //}

                //        //// check 5: bottom right
                //        //if (x < xLim && y < height)
                //        //{
                //        //    if (polarColors[i] == polarColors[(x + 3) + (y + 1) * xLim])
                //        //    {
                //        //        idTable[x / 3 + y * width] = curId;
                //        //        continue;
                //        //    }
                //        //}

                //        //// check 6: bottom
                //        //if (y < height)
                //        //{
                //        //    if (polarColors[i] == polarColors[x + (y + 1) * xLim])
                //        //    {
                //        //        idTable[x / 3 + y * width] = curId;
                //        //        continue;
                //        //    }
                //        //}

                //        idTable[x / 3 + y * width] = ++curId;
                //    }
                //}

                //// begin test alg
                int count = 0;

                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < xLim; x += 3)
                    {
                        if (x == 0 && y == 0) continue;

                        int colorIndexA = x + y * xLim;
                        int colorIndexB = x - 1 + y * xLim;
                        int colorIndexC = x + (y - 1) * xLim;
                        int colorIndexD = x - 1 + (y - 1) * xLim;

                        int tableIndexA = x / 3 + y * width;
                        int tableIndexB = x / 3 - 1 + y * width;
                        int tableIndexD = x / 3 - 1 + (y - 1) * width;
                        int tableIndexC = x / 3 + (y - 1) * width;
                        
                        // case 1: check top left (D)
                        if (x != 0 && y != 0)
                        {
                            if (objsColors[colorIndexA] == objsColors[colorIndexD])
                            {
                                idTable[tableIndexA] = (int) NEIGHBOR.D;
                                continue;
                            }
                        }
                        
                        // case 2: check left (B)
                        if (x != 0)
                        {
                            if (objsColors[colorIndexA] == objsColors[colorIndexB])
                            {
                                // case 3a: check top (C)
                                if (y != 0)
                                {
                                    if (objsColors[colorIndexA] == objsColors[colorIndexC])
                                    {
                                        if (idTable[tableIndexB] == (int) NEIGHBOR.C)
                                        {
                                            idTable[tableIndexA] = (int) NEIGHBOR.B;
                                            continue;
                                        }

                                        else
                                        {
                                            // call UPDATE here
                                            // ...

                                            idTable[tableIndexA] = (int) NEIGHBOR.B;
                                            continue;
                                        }
                                    }
                                }

                                else
                                {
                                    idTable[tableIndexA] = (int) NEIGHBOR.B;
                                    continue;
                                }
                            }
                        }

                        else
                        {
                            if (y != 0)
                            {
                                if (objsColors[colorIndexA] == objsColors[colorIndexC])
                                {
                                    idTable[tableIndexA] = (int) NEIGHBOR.C;
                                    continue;
                                }
                            }

                            else
                            {
                                idTable[tableIndexA] = count;
                            }
                        }
                    }
                }
                //// end test alg

                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < xLim; x += 3)
                    {
                        int colorIndex = x + y * xLim;
                        int idIndex = x / 3 + y * width;

                        objsColors[colorIndex + 2] = (byte)((977 * idTable[idIndex]) % 256);
                        objsColors[colorIndex + 1] = (byte)((644 * idTable[idIndex]) % 256);
                        objsColors[colorIndex] = (byte)((311 * idTable[idIndex]) % 256);
                    }
                }

                //
                Marshal.Copy(objsColors, 0, objsPtr, nBytes);
                polar.UnlockBits(polarData);
                objs.UnlockBits(objsData);

                nObjects = curId + 1;
                pictureBox1.Image = objs;
            }
        }
    }
}