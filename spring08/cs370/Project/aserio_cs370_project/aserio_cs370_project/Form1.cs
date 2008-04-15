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
  private Bitmap disp;

  public Form1()
  {
    InitializeComponent();

    clock.Start();
    lastTick = clock.ElapsedMilliseconds;

    // Set player attributes
    player.pos.x    = 10.0F;
    player.pos.y    = 100.0F;
    player.speed    = 3;
    player.color[2] = 255;
    player.color[1] = 163;
    player.color[0] = 177;
    player.rad      = 8;
  }

  private void Form1_Load( object sender, EventArgs e )
  {
    if ( !DsUtils.IsCorrectDirectXVersion() )
    {
      MessageBox.Show( this, "DirectX 8.1 NOT installed!", "DirectShow.NET", MessageBoxButtons.OK, MessageBoxIcon.Stop );
      this.Close(); return;
    }

    if ( !DsDev.GetDevicesOfCat( FilterCategory.VideoInputDevice, out capDevices ) )
    {
      MessageBox.Show( this, "No video capture devices found!", "DirectShow.NET", MessageBoxButtons.OK, MessageBoxIcon.Stop );
      this.Close(); return;
    }

    DsDevice dev = null;
    if ( capDevices.Count == 1 )
      dev = capDevices[0] as DsDevice;
    else
    {
      DeviceSelector selector = new DeviceSelector( capDevices );
      selector.ShowDialog( this );
      dev = selector.SelectedDevice;
    }

    if ( dev == null )
    {
      this.Close(); return;
    }

    if ( !StartupVideo( dev.Mon ) )
      this.Close();

    CaptureImage();
  }

  private void pictureBox_Click( object sender, EventArgs e )
  {
    MouseEventArgs args = (MouseEventArgs)e;

    MouseButtons key = args.Button;
    byte minThresh = 255;

    switch ( key )
    {
      case MouseButtons.Left:
        player.pos = new Vector2D( (float)args.X, (float)args.Y );
        player.touching = false;
        break;

      case MouseButtons.Right:
        for ( int y = args.Y - 1; y < args.Y + 1; ++y )
        {
          if ( y < 0 || y >= disp.Height ) continue;
          for ( int x = args.X - 1; x < args.X + 1; ++x )
          {
            if ( x < 0 || x >= disp.Width ) continue;
            Color c = disp.GetPixel( x, y );
            int gray = ( (int)c.R + (int)c.G + (int)c.B ) / 3;
            if ( (byte)gray < minThresh )
            {
              autoThresh = false;
              minThresh = (byte)gray;
              objThresh = minThresh;
            }
          }
        }
        break;
    }
  }
}

}