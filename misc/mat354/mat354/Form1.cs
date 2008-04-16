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
  // Consts
  const int BIG_NUMBER = 0x4FF;

  // Data
  byte[] rgbs;
  List<Point> points;
  Point[] region;
  Bitmap diagram;
  int k = 1;
  byte[] color;
  List<Region> regions;

  // Functions
  public Form1()
  {
    InitializeComponent();

    diagram  = new Bitmap( pictureBox.Width, pictureBox.Height );
    color    = new byte[3];
    points   = new List<Point>();
    color[2] = 128;
    color[1] = 0;
    color[0] = 255;
  }

  private void pictureBox_Click( object sender, EventArgs e )
  {
    MouseEventArgs args = (MouseEventArgs)e;

    if ( args.Button == MouseButtons.Left )
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
    int nBytes = stride * height;
    rgbs       = new byte[nBytes];

    if ( points.Count > k )
    {
      FillRegions( width, height );
    }
    else
    {
      for ( int i = 0; i < nBytes; ++i )
        rgbs[i] = 255;
    }

    SetPointHandles();

    Marshal.Copy( rgbs, 0, data.Scan0, nBytes );
    diagram.UnlockBits( data );

    // Update displayed image
    pictureBox.Image = diagram;

    // Update displayed image
    pictureBox.Image = diagram;
  }

  private void FillRegions( int width, int height )
  {
    Point tester = new Point();

    // For each pixel method
    for ( int y = 0; y < height; ++y )
    {
      for ( int x = 0; x < width; ++x )
      {
        Point[] nears = new Point[k];
        tester.X = x;
        tester.Y = y;
        Region region = GetRegion( tester, false );

        int found = -1;
        int index = 0;
        foreach ( Region r in regions )
        {
          if ( r.indices.Equals( (object)region ) )
          {
            found = index;
            break;
          }
          index++;
        }

        if ( found != -1 )
          region = regions[found];
        else
          regions.Add( region );

        for ( int i = 0; i < 3; ++i )
          rgbs[3 * ( y * width + x ) + i] = region.color[i];
      }
    }
  }

  private Region GetRegion( Point cur, bool exclude )
  {
    int nPoints = points.Count;

    //if ( !exclude )
    //{
      List<RelPoint> rpoints = new List<RelPoint>( nPoints );

      int index = 0;
      foreach ( Point p in points )
      {
        int x = p.X, y = p.Y;
        rpoints.Add( new RelPoint( x * x + y * y, index++ ) );
      }

      rpoints.Sort();

      rpoints.RemoveRange( k, nPoints - 1 );

      return new Region( rpoints );
    //}
    //else
    //{
    //  List<RelPoint> rpoints = new List<RelPoint>( nPoints - 1 );
    //}
  }

  private void SetPointHandles()
  {
    foreach ( Point p in points )
    {
      int x = p.X, y = p.Y, width = pictureBox.Width, step = 3;

      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y - 2 ) * width + ( x - 1 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y - 2 ) * width + ( x - 0 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y - 2 ) * width + ( x + 1 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y - 1 ) * width + ( x + 2 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y - 0 ) * width + ( x + 2 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y + 1 ) * width + ( x + 2 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y + 2 ) * width + ( x + 1 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y + 2 ) * width + ( x - 0 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y + 2 ) * width + ( x - 1 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y + 1 ) * width + ( x - 2 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y - 0 ) * width + ( x - 2 ) ) + i] = color[i];
      for ( int i = 0; i < 3; ++i )
        rgbs[step * ( ( y - 1 ) * width + ( x - 2 ) ) + i] = color[i];
    }
  }


}

class Rainbow
{
  static int count = 0;

  static public byte[] GetNext()
  {
    byte[] color = new byte[3];

    //switch ( count )
    //{
    //  case 0: // RED
    //    c.R = 255;
    //    c.G = 0;
    //    c.B = 0;
    //    break;
    //  case 1: // ORANGE
    //    c.R = 255;
    //    c.G = 128;
    //    c.B = 0;
    //    break;
    //  case 2: // YELLOW
    //    c.R = 255;
    //    c.G = 0;
    //    c.B = 255;
    //    break;
    //  case 3: // GREEN
    //    c.R = 0;
    //    c.G = 255;
    //    c.B = 0;
    //    break;
    //  case 4: // BLUE
    //    c.R = 0;
    //    c.G = 0;
    //    c.B = 255;
    //    break;
    //  case 5: // INDIGO
    //    c.R = 128;
    //    c.G = 0;
    //    c.B = 255;
    //    break;
    //  case 6: // VIOLET
    //    c.R = 255;
    //    c.G = 0;
    //    c.B = 255;
    //    break;
    //}

    count = count % 7;
    return color;
  }
}

class RelPoint
{
  public RelPoint( int cost_, int index_ )
  {
    cost  = cost_;
    index = index_;
  }

  static public bool operator <( RelPoint lhs, RelPoint rhs )
  {
    return ( lhs.cost < rhs.cost );
  }

  static public bool operator >( RelPoint lhs, RelPoint rhs )
  {
    return ( lhs.cost > rhs.cost );
  }

  public int cost;
  public int index;
}

class Region
{
  public Region( List<RelPoint> rpoints )
  {
    indices = new List<int>( rpoints.Count );
    foreach ( RelPoint p in rpoints )
      indices.Add( p.index );
    indices.Sort();

    color = Rainbow.GetNext();

    //index = count++;
  }

  public Region( int[] indices_ )
  {
    indices = new List<int>( indices_.Length );
    foreach ( int i in indices_ )
      indices.Add( i );
    indices.Sort();

    color = Rainbow.GetNext();

    //index = count++;
  }

  public List<int> indices;
  public byte[] color;
  //public int index;

  //static public int count = 0;
}

}
