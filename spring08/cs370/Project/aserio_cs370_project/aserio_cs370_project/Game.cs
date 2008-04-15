using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

using DShowNET;
using DShowNET.Device;


namespace aserio_cs370_project
{

public partial class Form1 : Form
{
  float gravity   = 9.81F / 2.0F;
  Stopwatch clock = new Stopwatch();
  long lastTick;

  // Player and game data
  GameObject player = new GameObject();
  float objThresh = 40.0F;
  bool autoThresh = true;

  byte[] grays;

  private void UpdateGame()
  {
    if ( fromCam != null )
    {
      UpdateGraphics();
      UpdatePhysics();
    }
  }

  private void UpdateGraphics()
  {
    // Image from webcam
    int width    = fromCam.Width;
    int height   = fromCam.Height;

    // Lock bitmap
    BitmapData data = fromCam.LockBits( new Rectangle( 0, 0, width, height ),
                                        ImageLockMode.ReadOnly,
                                        PixelFormat.Format32bppRgb );

    int stride  = data.Stride;
    int step    = stride / width;
    int nBytes  = stride * height;

    IntPtr ptr  = data.Scan0;
    byte[] rgbs = new byte[nBytes];
    Marshal.Copy( ptr, rgbs, 0, nBytes );

    grays = new byte[width * height];

    // Histogram stretch
    HistStretch( ref rgbs, width, height, step, stride );

    // Specify region checked by physics
    GetRegionBounds( ref player.coords.min, ref player.coords.max );
    Vector2D min    = new Vector2D( player.coords.min );
    Vector2D max    = new Vector2D( player.coords.max );
    player.window   = new byte[(int)player.coords.Width() * (int)player.coords.Height()];
    int regionIndex = 0;
    int radSqrd     = player.rad * player.rad;

    for ( int y = 0; y < height; ++y )
    {
      for ( int xs = 0; xs < stride; xs += step )
      {
        int index = y * stride + xs;
        int x = xs / step;

        int gray = ( (int)rgbs[index + 2] + (int)rgbs[index + 1] + (int)rgbs[index] ) / 3;
        grays[y * width + x] = (byte)gray;

        // Fill physics update region
        if ( x >= min.x && x <= max.x && y >= min.y && y <= max.y )
          player.window[regionIndex++] = (byte)( gray );

        // Draw player
        int xTrans = x - player.pos.XInt();
        int yTrans = y - player.pos.YInt();
        if ( xTrans * xTrans + yTrans * yTrans <= radSqrd )
        {
          rgbs[index + 2] = player.color[2];
          rgbs[index + 1] = player.color[1];
          rgbs[index + 0] = player.color[0];
        }
      }
    }

    disp = new Bitmap( width, height );
    BitmapData dispData = disp.LockBits( new Rectangle( 0, 0, width, height ),
                                         ImageLockMode.WriteOnly,
                                         PixelFormat.Format32bppRgb );
    ptr = dispData.Scan0;

    Marshal.Copy( rgbs, 0, ptr, nBytes );

    // Unlock bitmap
    fromCam.UnlockBits( data );
    disp.UnlockBits( dispData );
  }

  private void UpdatePhysics()
  {
    // Update time passed
    long timeStamp  = clock.ElapsedMilliseconds;
    long timePassed = timeStamp - lastTick;
    lastTick        = timeStamp;

    // Check if player will collide
    float minDist = -1.0F;
    int width = (int)player.coords.Width();
    int height = (int)player.coords.Height();
    Vector2D disp = new Vector2D();
    Vector2D n = new Vector2D( player.vel.y, -player.vel.x ).Normalized();
    Vector2D relPos = new Vector2D( player.pos - player.coords.min );
    if ( autoThresh )
      objThresh = AutoThresh( ref grays );

    for ( int y = 0; y < height; ++y )
    {
      for ( int x = 0; x < width; ++x )
      {
        if ( player.window[y * width + x] < objThresh )
        {
          // Get distance from direction line
          float distP = Math.Abs( n * new Vector2D( (float)x - relPos.x, (float)y - relPos.y ) );
          if ( distP <= player.rad )
          {
            float dist = new Vector2D( relPos.x - x, relPos.y - y ).Mag();
            if ( minDist == -1.0F || dist < minDist )
              minDist = dist;
          }
        }
      }
    }

    // if no collision
    if ( minDist == -1.0F )
    {
      player.pos += player.vel;
      if ( player.touching )
      {
        player.vel = new Vector2D();
      }
    }
    // if collision
    else
    {
      player.touching = true;
      player.pos += ( minDist * ( 20F / 1000F ) * player.vel );
      player.vel = new Vector2D();
    }

    // Update velocity if falling
    if ( !player.touching )
      player.vel.y += ( gravity * (float)timePassed / 1000.0F );
  }

  private void GetRegionBounds( ref Vector2D min, ref Vector2D max )
  {
    if ( player.vel.x >= 0 )
    {
      min.x = player.pos.x - (float)player.rad;
      max.x = player.pos.x + player.vel.x + (float)player.rad;
    }
    else
    {
      min.x = player.pos.x + player.vel.x - (float)player.rad;
      max.x = player.pos.x + (float)player.rad;
    }

    if ( player.vel.y >= 0 )
    {
      min.y = player.pos.y - (float)player.rad;
      max.y = player.pos.y + player.vel.y + (float)player.rad;
    }
    else
    {
      min.y = player.pos.y + player.vel.y - (float)player.rad;
      max.y = player.pos.y + (float)player.rad;
    }

    min.x = (float)Math.Floor( min.x );
    min.y = (float)Math.Floor( min.y );
    max.x = (float)Math.Ceiling( max.x );
    max.y = (float)Math.Ceiling( max.y );
  }

  private void HistStretch( ref byte[] rgbs, int width, int height, int step, int stride )
  {
    byte[] mins = new byte[3];
    byte[] maxs = new byte[3];

    bool first = true;

    for ( int y = 0; y < height; ++y )
    {
      for ( int xs = 0; xs < stride; xs += step )
      {
        int index = y * stride + xs;
        if ( !first )
        {
          for ( int i = 0; i < 3; ++i )
          {
            int curIndex = index + i;
            if ( rgbs[curIndex] < mins[i] )
              mins[i] = rgbs[curIndex];
            if ( rgbs[curIndex] > maxs[i] )
              maxs[i] = rgbs[curIndex];
          }
        }
        else
        {
          first = false;
          for ( int i = 0; i < 3; ++i )
          {
            mins[i] = rgbs[index + i];
            maxs[i] = mins[i];
          }
        }
      }
    }

    // choose band with greatest breadth for histogram stretch
    int c = 0;
    for ( int i = 1; i <= 2; ++i )
    {
      if ( maxs[i] - mins[i] > maxs[c] - mins[c] )
        c = i;
    }

    float factor = 255.0F / (float)maxs[c];
    int nBytes = stride * height;
    for ( int i = 0; i < nBytes; ++i )
    {
      int val = (int)( (float)( rgbs[i] - mins[c] ) * factor );
      if ( val < 0 ) val = 0;
      else if ( val > 255 ) val = 255;
      rgbs[i] = (byte)val;
    }
  }

  private float AutoThresh( ref byte[] grays )
  {
    float termVal = 1.0F;

    // Starting gray value: mean gray val
    int nGrays = grays.Length;
    float avgGray = 0.0F;

    for ( int i = 0; i < nGrays; ++i )
        avgGray += grays[i];
    avgGray /= nGrays;

    float curThresh = avgGray, prevThresh;

    do
    {
      // Get values for groups using cur threshold
      prevThresh = curThresh;
      float G1 = 0.0F, G2 = 0.0F;
      float nG1 = 0.0F, nG2 = 0.0F;
      for ( int i = 0; i < nGrays; ++i )
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
    } while ( Math.Abs( prevThresh - curThresh ) > termVal );

    return curThresh;
  }

  protected override bool ProcessCmdKey( ref Message msg, Keys keyData )
  {
    switch ( keyData )
    {
      // Move left
      case Keys.Left:
      case Keys.A:
        if ( !player.touching )
          player.vel.x = -(float)player.speed;
        break;

      // Move right
      case Keys.Right:
      case Keys.D:
        if ( !player.touching )
          player.vel.x = (float)player.speed;
        break;

      // Jump
      case Keys.Up:
      case Keys.W:
      case Keys.Space:
        if ( player.touching )
        {
          player.touching = false;
          player.vel.y = -player.hops;
        }
        break;

      // Fall
      case Keys.S:
        player.touching = false;
        break;

      default:
        return false;
    }

    return true;
  }
}

class Vector2D
{
  public Vector2D()
  {
    x = 0.0F;
    y = 0.0F;
  }

  public Vector2D( Vector2D rhs )
  {
    this.x = rhs.x;
    this.y = rhs.y;
  }

  public Vector2D( float _x, float _y )
  {
    x = _x;
    y = _y;
  }

  public float Mag()
  {
    return (float)Math.Sqrt( x * x + y * y );
  }

  public void Normalize()
  {
    float mag = Mag();
    x /= mag;
    y /= mag;
  }

  public Vector2D Normalized()
  {
    float mag = Mag();
    return new Vector2D( x / mag, y / mag );
  }

  static public Vector2D operator +( Vector2D lhs, Vector2D rhs )
  {
    return new Vector2D( lhs.x + rhs.x, lhs.y + rhs.y );
  }

  static public Vector2D operator -( Vector2D lhs, Vector2D rhs )
  {
    return new Vector2D( lhs.x - rhs.x, lhs.y - rhs.y );
  }

  static public float operator *( Vector2D lhs, Vector2D rhs )
  {
    return ( lhs.x * rhs.x + lhs.y * rhs.y );
  }

  static public Vector2D operator *( float lhs, Vector2D rhs )
  {
    return new Vector2D( rhs.x * lhs, rhs.y * lhs );
  }

  public int XInt()
  {
    return (int)( x + .5f );
  }

  public int YInt()
  {
    return (int)( y + .5f );
  }

  public float x;
  public float y;
}

class Box2D
{
  public Box2D()
  {
    min = new Vector2D();
    max = new Vector2D();
  }

  public float Width()
  {
    return ( max.x - min.x + 1.0F );
  }

  public float Height()
  {
    return ( max.y - min.y + 1.0F );
  }

  public Vector2D min;
  public Vector2D max;
}

class GameObject
{
  public Vector2D pos;
  public Vector2D vel;
  public int rad;
  public int speed;
  public int hops;
  public bool touching;
  public byte[] color;
  public byte[] window;
  public Box2D coords;

  public GameObject()
  {
    pos = new Vector2D( 0.0F, 0.0F );
    vel = new Vector2D( 0.0F, 0.0F );
    rad = 3;
    speed = 1;
    hops = 3;
    touching = false;
    color = new byte[3];
    coords = new Box2D();
  }
}

}
