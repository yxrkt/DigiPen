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

                        //int iB = (factor * x) + (factor * y) * destRes * 3;
                        //int iG = iB + 1;
                        //int iR = iB + 2;

                        //destColors[iR] = srcColors[iRSrc];
                        //destColors[iG] = srcColors[iGSrc];
                        //destColors[iB] = srcColors[iBSrc];
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

            if (dest == destBmp && src == sourceBmp)
            {
                //pictureBox1.Size.Height = destRes;
                //pictureBox1.Size.Width  = destRes;
            }
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            BitmapData bmpData = sourceBmp.LockBits(new Rectangle(0, 0, sourceBmp.Width, sourceBmp.Height), 
                                                    ImageLockMode.ReadWrite, 
                                                    PixelFormat.Format24bppRgb);

            System.IntPtr ptr = bmpData.Scan0;
            int numBytes = sourceBmp.Width * sourceBmp.Height * 3;
            byte[] rgbValues = new byte[numBytes];

            destBmp = new Bitmap(sourceBmp.Width, sourceBmp.Height);
            BitmapData bmpData1 = destBmp.LockBits(new Rectangle(0, 0, destBmp.Width, destBmp.Height),
                                                   ImageLockMode.WriteOnly,
                                                   PixelFormat.Format24bppRgb);

            System.IntPtr ptr1 = bmpData.Scan0;
            byte[] rgbValues1 = new byte[numBytes];

            Marshal.Copy(ptr, rgbValues, 0, numBytes);

            for (int i = 0; i < numBytes; i += 3)
            {
                byte grayVal = (byte)(((float) rgbValues[i] + 
                                       (float) rgbValues[i + 1] + 
                                       (float) rgbValues[i + 2]) / (float) 3);
                rgbValues[i]     = grayVal;
                rgbValues[i + 1] = grayVal;
                rgbValues[i + 2] = grayVal;
            }

            Marshal.Copy(rgbValues, 0, ptr, numBytes);

            sourceBmp.UnlockBits(bmpData);

            pictureBox1.Image = sourceBmp;
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
    }
}