using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections;
using System.Drawing.Imaging;

namespace mat354
{
public partial class Form1 : Form
{
  // Data
  byte[] rgbs;
  List<Point> points;
  Bitmap diagram;
  int k = 1;

  // Functions
  public Form1()
  {
    InitializeComponent();

    diagram = new Bitmap( pictureBox.Width, pictureBox.Height );
  }

  private void pictureBox_Click( object sender, EventArgs e )
  {
    MouseEventArgs args = (MouseEventArgs)e;

    if ( args.Button = MouseButtons.Left )
      points.Add( new Point( args.X, args.Y ) );

    UpdateImage();
  }

  private void UpdateImage()
  {
    int width       = pictureBox.Width, height = pictureBox.Height;
    Rectangle rec   = new Rectangle( 0, 0, width, height );
    BitmapData data = diagram.LockBits( rec, ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb );

    int stride = data.Stride;
    int step   = stride / width;

    for ( int y = 0; y < height; ++y )
    {
      for ( int xs = 0; xs < stride; xs += step )
      {
        int index = y * width + xs / step;
        int strideIndex = y * stride + xs;
      }
    }

    // Update displayed image
    pictureBox.Image = diagram;

    // Update displayed image
    pictureBox.Image = diagram;
  }
}

class Rainbow
{
  static int count = 0;

  static public Color GetNext()
  {
    Color c = new Color();

    switch ( count )
    {
      case 0: // RED
        c.R = 255;
        c.G = 0;
        c.B = 0;
        break;
      case 1: // ORANGE
        c.R = 255;
        c.G = 128;
        c.B = 0;
        break;
      case 2: // YELLOW
        c.R = 255;
        c.G = 0;
        c.B = 255;
        break;
      case 3: // GREEN
        c.R = 0;
        c.G = 255;
        c.B = 0;
        break;
      case 4: // BLUE
        c.R = 0;
        c.G = 0;
        c.B = 255;
        break;
      case 5: // INDIGO
        c.R = 128;
        c.G = 0;
        c.B = 255;
        break;
      case 6: // VIOLET
        c.R = 255;
        c.G = 0;
        c.B = 255;
        break;
    }

    count = count % 7;
    return c;
  }
}

}