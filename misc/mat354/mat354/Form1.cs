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
  private byte[]        rgbs;
  private List<Point>   points;
  private Bitmap        diagram;
  private int           k;
  private byte[]        color;
  private List<VRegion> regions;
  private bool          adding;

  // Functions
  public Form1()
  {
    InitializeComponent();

    points   = new List<Point>();
    k        = 1;
    color    = new byte[3];
    color[2] = 0;
    color[1] = 0;
    color[0] = 0;
    regions  = new List<VRegion>();
    adding   = true;
  }

  private void Form1_Load( object sender, EventArgs e )
  {
    toolStripStatusLabel1.Text = "Mode: Add Points (click to add points)";
    toolStripStatusLabel2.Text = "Regions: 0";
  }

  private void pictureBox_Click( object sender, EventArgs e )
  {
    MouseEventArgs args = (MouseEventArgs)e;

    if ( args.Button == MouseButtons.Left )
    {
      if ( adding )
      {
        points.Add( new Point( args.X, args.Y ) );
        UpdateImage();
      }
      else
      {
        if ( points.Count > k )
        {
          RemoveRegion( args.X, args.Y );
        }
        else
        {
          points.Clear();
          toolStripStatusLabel2.Text = "Regions: 1";
        }
        UpdateImage();
      }
    }
  }

  private void UpdateImage()
  {
    int width       = pictureBox.Width;
    int height      = pictureBox.Height;
    Rectangle rec   = new Rectangle( 0, 0, width, height );
    diagram         = new Bitmap( pictureBox.Width, pictureBox.Height );
    BitmapData data = diagram.LockBits( rec, ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb );

    int stride = data.Stride;
    int step   = stride / width;
    int nBytes = stride * height;
    rgbs       = new byte[nBytes];

    if ( toolStripTextBox1.Text != "" )
    {
      k = Convert.ToInt32( toolStripTextBox1.Text );
      if ( k == 0 ) k = 1;
    }
    regions.Clear();

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
    toolStripProgressBar1.Visible = true;

    // For each pixel method
    for ( int y = 0; y < height; ++y )
    {
      toolStripProgressBar1.Value = (int)( (float)y / (float)height );
      for ( int x = 0; x < width; ++x )
      {
        tester.X = x;
        tester.Y = y;
        VRegion region = GetRegion( tester );

        int found = -1;
        int index = 0;
        foreach ( VRegion r in regions )
        {
          bool regionAlreadyExists = false;
          for ( int i = 0; i < k; ++i )
          {
            if ( r.indices[i] != region.indices[i] )
            {
              regionAlreadyExists = true;
              break;
            }
          }
          if ( regionAlreadyExists == false )
          {
            found = index;
            break;
          }
          index++;
        }

        if ( found != -1 )
        {
          for ( int i = 0; i < 3; ++i )
            rgbs[3 * ( y * width + x ) + i] = regions[found].color[i];
        }
        else
        {
          region.color = Rainbow.GetNext();
          regions.Add( region );
          for ( int i = 0; i < 3; ++i )
            rgbs[3 * ( y * width + x ) + i] = region.color[i];
        }
      }
    }

    toolStripStatusLabel2.Text    = "Regions: " + regions.Count.ToString();
    toolStripProgressBar1.Visible = false;
  }

  private VRegion GetRegion( Point cur )
  {
    int nPoints = points.Count;

    List<RelPoint> rpoints = new List<RelPoint>( nPoints );

    int index = 0;
    foreach ( Point p in points )
    {
      int x = p.X - cur.X, y = p.Y - cur.Y;
      rpoints.Add( new RelPoint( x * x + y * y, index++ ) );
    }

    rpoints.Sort( delegate( RelPoint lhs, RelPoint rhs ) { return lhs.cost.CompareTo( rhs.cost ); } );

    rpoints.RemoveRange( k, nPoints - k );

    return new VRegion( rpoints );
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

  private void RemoveRegion( int x, int y )
  {
    Point clicked = new Point();
    clicked.X = x;
    clicked.Y = y;
    VRegion clickedRegion = GetRegion( clicked ); 
    VRegion found = regions.Find(
      delegate( VRegion val )
      {
        for ( int i = 0; i < k; ++i )
        {
          if ( val.indices[i] != clickedRegion.indices[i] )
            return false;
        }
        return true;
      } );

    List<Point> toRemove = new List<Point>( k );
    foreach ( int i in found.indices )
      toRemove.Add( points[i] );
    foreach ( Point p in toRemove )
      points.Remove( p );
  }

  private void toolStripButtonAdd_Click( object sender, EventArgs e )
  {
    toolStripButtonRemove.Checked = false;
    toolStripButtonAdd.Checked    = true;
    toolStripStatusLabel1.Text    = "Mode: Add Points (click to add points)";
    adding = true;
  }

  private void toolStripButtonRemove_Click( object sender, EventArgs e )
  {
    toolStripButtonAdd.Checked    = false;
    toolStripButtonRemove.Checked = true;
    toolStripStatusLabel1.Text    = "Mode: Remove Regions (click to remove regions)";
    adding = false;
  }

  private void toolStripButtonUpdate_Click( object sender, EventArgs e )
  {
    UpdateImage();
  }

  private void toolStripButtonClear_Click( object sender, EventArgs e )
  {
    points.Clear();
    toolStripStatusLabel2.Text = "Regions: 0";
    UpdateImage();
  }
}

class Rainbow
{
  static int count = 0;

  static public byte[] GetNext()
  {
    byte[] color = new byte[3];

    switch ( count )
    {
      case 0: // RED
        color[2] = 255;
        color[1] = 0;
        color[0] = 0;
        break;
      case 1: // ORANGE
        color[2] = 255;
        color[1] = 128;
        color[0] = 0;
        break;
      case 2: // YELLOW
        color[2] = 255;
        color[1] = 255;
        color[0] = 0;
        break;
      case 3: // GREEN
        color[2] = 0;
        color[1] = 255;
        color[0] = 0;
        break;
      case 4: // BLUE
        color[2] = 0;
        color[1] = 0;
        color[0] = 255;
        break;
      case 5: // INDIGO
        color[2] = 128;
        color[1] = 0;
        color[0] = 255;
        break;
      case 6: // VIOLET
        color[2] = 255;
        color[1] = 0;
        color[0] = 255;
        break;
    }

    count = ( count + 1 ) % 7;
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

class VRegion
{
  public VRegion( List<RelPoint> rpoints )
  {
    indices = new List<int>( rpoints.Count );
    foreach ( RelPoint p in rpoints )
      indices.Add( p.index );
    indices.Sort();
    //index = count++;
  }

  public VRegion( int[] indices_ )
  {
    indices = new List<int>( indices_.Length );
    foreach ( int i in indices_ )
      indices.Add( i );
    indices.Sort();
    //index = count++;
  }

  public List<int> indices;
  public byte[] color;
  //public int index;

  //static public int count = 0;
}

}
