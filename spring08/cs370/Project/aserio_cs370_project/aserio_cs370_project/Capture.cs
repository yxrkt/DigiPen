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

public partial class Form1 : Form, ISampleGrabberCB
{
  private void videoPanel_Resize( object sender, System.EventArgs e )
  {
    ResizeVideoWindow();
  }

  private void CaptureImage()
  {
    if ( savedArray == null )
    {
      int size = videoInfoHeader.BmiHeader.ImageSize;
      if ( ( size < 1000 ) || ( size > 16000000 ) )
        return;
      savedArray = new byte[size];
    }

    captured = false;
    sampGrabber.SetCallback( this, 1 );
  }

  /// <summary> capture event, triggered by buffer callback. </summary>
  void OnCaptureDone()
  {
    Trace.WriteLine( "!!DLG: OnCaptureDone" );

    try
    {
      int hr;
      if ( sampGrabber == null )
        return;
      hr = sampGrabber.SetCallback( null, 0 );

      int w = videoInfoHeader.BmiHeader.Width;
      int h = videoInfoHeader.BmiHeader.Height;
      if ( ( ( w & 0x03 ) != 0 ) || ( w < 32 ) || ( w > 4096 ) || ( h < 32 ) || ( h > 4096 ) )
        return;
      int stride = w * 3;

      GCHandle handle = GCHandle.Alloc( savedArray, GCHandleType.Pinned );
      int scan0 = (int)handle.AddrOfPinnedObject();
      scan0 += ( h - 1 ) * stride;
      fromCam = new Bitmap( w, h, -stride, PixelFormat.Format24bppRgb, (IntPtr)scan0 );
      handle.Free();
      savedArray = null;
      UpdateGame();
      pictureBox.Image = disp;
      CaptureImage();
    }
    catch ( Exception ee )
    {
      MessageBox.Show( this, "Could not grab picture\r\n" + ee.Message, "DirectShow.NET", MessageBoxButtons.OK, MessageBoxIcon.Stop );
    }
  }

  /// <summary> start all the interfaces, graphs and preview window. </summary>
  bool StartupVideo( UCOMIMoniker mon )
  {
    int hr;
    try
    {
      if ( !CreateCaptureDevice( mon ) )
        return false;

      if ( !GetInterfaces() )
        return false;

      if ( !SetupGraph() )
        return false;

      if ( !SetupVideoWindow() )
        return false;

#if DEBUG
      DsROT.AddGraphToRot( graphBuilder, out rotCookie );		// graphBuilder capGraph
#endif

      hr = mediaCtrl.Run();
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      CaptureImage(); // Begins updating...

      return true;
    }
    catch ( Exception ee )
    {
      MessageBox.Show( this, "Could not start video stream\r\n" + ee.Message, "DirectShow.NET", MessageBoxButtons.OK, MessageBoxIcon.Stop );
      return false;
    }
  }

  /// <summary> make the video preview window to show in videoPanel. </summary>
  bool SetupVideoWindow()
  {
    int hr;
    try
    {
      // Set the video window to be a child of the main window
      hr = videoWin.put_Owner( videoPanel.Handle );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      // Set video window style
      hr = videoWin.put_WindowStyle( WS_CHILD | WS_CLIPCHILDREN );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      // Use helper function to position video window in client rect of owner window
      ResizeVideoWindow();

      // Make the video window visible, now that it is properly positioned
      hr = videoWin.put_Visible( DsHlp.OATRUE );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      hr = mediaEvt.SetNotifyWindow( this.Handle, WM_GRAPHNOTIFY, IntPtr.Zero );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );
      return true;
    }
    catch ( Exception ee )
    {
      MessageBox.Show( this, "Could not setup video window\r\n" + ee.Message, "DirectShow.NET", MessageBoxButtons.OK, MessageBoxIcon.Stop );
      return false;
    }
  }

  /// <summary> build the capture graph for grabber. </summary>
  bool SetupGraph()
  {
    int hr;
    try
    {
      hr = capGraph.SetFiltergraph( graphBuilder );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      hr = graphBuilder.AddFilter( capFilter, "Ds.NET Video Capture Device" );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      //DsUtils.ShowCapPinDialog( capGraph, capFilter, this.Handle );

      AMMediaType media = new AMMediaType();
      media.majorType = MediaType.Video;
      media.subType = MediaSubType.RGB24;
      media.formatType = FormatType.VideoInfo;		// ???
      hr = sampGrabber.SetMediaType( media );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      hr = graphBuilder.AddFilter( baseGrabFlt, "Ds.NET Grabber" );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      Guid cat = PinCategory.Preview;
      Guid med = MediaType.Video;
      hr = capGraph.RenderStream( ref cat, ref med, capFilter, null, null ); // baseGrabFlt 
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      cat = PinCategory.Capture;
      med = MediaType.Video;
      hr = capGraph.RenderStream( ref cat, ref med, capFilter, null, baseGrabFlt ); // baseGrabFlt 
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      media = new AMMediaType();
      hr = sampGrabber.GetConnectedMediaType( media );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );
      if ( ( media.formatType != FormatType.VideoInfo ) || ( media.formatPtr == IntPtr.Zero ) )
        throw new NotSupportedException( "Unknown Grabber Media Format" );

      videoInfoHeader = (VideoInfoHeader)Marshal.PtrToStructure( media.formatPtr, typeof( VideoInfoHeader ) );
      Marshal.FreeCoTaskMem( media.formatPtr ); media.formatPtr = IntPtr.Zero;

      hr = sampGrabber.SetBufferSamples( false );
      if ( hr == 0 )
        hr = sampGrabber.SetOneShot( false );
      if ( hr == 0 )
        hr = sampGrabber.SetCallback( null, 0 );
      if ( hr < 0 )
        Marshal.ThrowExceptionForHR( hr );

      return true;
    }
    catch ( Exception ee )
    {
      MessageBox.Show( this, "Could not setup graph\r\n" + ee.Message, "DirectShow.NET", MessageBoxButtons.OK, MessageBoxIcon.Stop );
      return false;
    }
  }

  /// <summary> create the used COM components and get the interfaces. </summary>
  bool GetInterfaces()
  {
    Type comType = null;
    object comObj = null;
    try
    {
      comType = Type.GetTypeFromCLSID( Clsid.FilterGraph );
      if ( comType == null )
        throw new NotImplementedException( @"DirectShow FilterGraph not installed/registered!" );
      comObj = Activator.CreateInstance( comType );
      graphBuilder = (IGraphBuilder)comObj; comObj = null;

      Guid clsid = Clsid.CaptureGraphBuilder2;
      Guid riid = typeof( ICaptureGraphBuilder2 ).GUID;
      comObj = DsBugWO.CreateDsInstance( ref clsid, ref riid );
      capGraph = (ICaptureGraphBuilder2)comObj; comObj = null;

      comType = Type.GetTypeFromCLSID( Clsid.SampleGrabber );
      if ( comType == null )
        throw new NotImplementedException( @"DirectShow SampleGrabber not installed/registered!" );
      comObj = Activator.CreateInstance( comType );
      sampGrabber = (ISampleGrabber)comObj; comObj = null;

      mediaCtrl = (IMediaControl)graphBuilder;
      videoWin = (IVideoWindow)graphBuilder;
      mediaEvt = (IMediaEventEx)graphBuilder;
      baseGrabFlt = (IBaseFilter)sampGrabber;
      return true;
    }
    catch ( Exception ee )
    {
      MessageBox.Show( this, "Could not get interfaces\r\n" + ee.Message, "DirectShow.NET", MessageBoxButtons.OK, MessageBoxIcon.Stop );
      return false;
    }
    finally
    {
      if ( comObj != null )
        Marshal.ReleaseComObject( comObj ); comObj = null;
    }
  }

  /// <summary> create the user selected capture device. </summary>
  bool CreateCaptureDevice( UCOMIMoniker mon )
  {
    object capObj = null;
    try
    {
      Guid gbf = typeof( IBaseFilter ).GUID;
      mon.BindToObject( null, null, ref gbf, out capObj );
      capFilter = (IBaseFilter)capObj; capObj = null;
      return true;
    }
    catch ( Exception ee )
    {
      MessageBox.Show( this, "Could not create capture device\r\n" + ee.Message, "DirectShow.NET", MessageBoxButtons.OK, MessageBoxIcon.Stop );
      return false;
    }
    finally
    {
      if ( capObj != null )
        Marshal.ReleaseComObject( capObj ); capObj = null;
    }

  }

  /// <summary> do cleanup and release DirectShow. </summary>
  void CloseInterfaces()
  {
    int hr;
    try
    {
#if DEBUG
      if ( rotCookie != 0 )
        DsROT.RemoveGraphFromRot( ref rotCookie );
#endif

      if ( mediaCtrl != null )
      {
        hr = mediaCtrl.Stop();
        mediaCtrl = null;
      }

      if ( mediaEvt != null )
      {
        hr = mediaEvt.SetNotifyWindow( IntPtr.Zero, WM_GRAPHNOTIFY, IntPtr.Zero );
        mediaEvt = null;
      }

      if ( videoWin != null )
      {
        hr = videoWin.put_Visible( DsHlp.OAFALSE );
        hr = videoWin.put_Owner( IntPtr.Zero );
        videoWin = null;
      }

      baseGrabFlt = null;
      if ( sampGrabber != null )
        Marshal.ReleaseComObject( sampGrabber ); sampGrabber = null;

      if ( capGraph != null )
        Marshal.ReleaseComObject( capGraph ); capGraph = null;

      if ( graphBuilder != null )
        Marshal.ReleaseComObject( graphBuilder ); graphBuilder = null;

      if ( capFilter != null )
        Marshal.ReleaseComObject( capFilter ); capFilter = null;

      if ( capDevices != null )
      {
        foreach ( DsDevice d in capDevices )
          d.Dispose();
        capDevices = null;
      }
    }
    catch ( Exception )
    { }
  }

  /// <summary> resize preview video window to fill client area. </summary>
  void ResizeVideoWindow()
  {
    if ( videoWin != null )
    {
      Rectangle rc = videoPanel.ClientRectangle;
      videoWin.SetWindowPosition( 0, 0, rc.Right, rc.Bottom );
    }
  }

  void ResizeCaptureWindow()
  {
    //videoInfoHeader.BmiHeader.Width  = videoPanel.Width;
    //videoInfoHeader.BmiHeader.Height = videoPanel.Height;
  }

  /// <summary> override window fn to handle graph events. </summary>
  protected override void WndProc( ref Message m )
  {
    if ( m.Msg == WM_GRAPHNOTIFY )
    {
      if ( mediaEvt != null )
        OnGraphNotify();
      return;
    }
    base.WndProc( ref m );
  }

  protected override void OnSizeChanged( EventArgs e )
  {
    base.OnSizeChanged( e );
    ResizeCaptureWindow();
  }

  /// <summary> graph event (WM_GRAPHNOTIFY) handler. </summary>
  void OnGraphNotify()
  {
    DsEvCode code;
    int p1, p2, hr = 0;
    do
    {
      hr = mediaEvt.GetEvent( out code, out p1, out p2, 0 );
      if ( hr < 0 )
        break;
      hr = mediaEvt.FreeEventParams( code, p1, p2 );
    }
    while ( hr == 0 );
  }

  /// <summary> sample callback, NOT USED. </summary>
  int ISampleGrabberCB.SampleCB( double SampleTime, IMediaSample pSample )
  {
    Trace.WriteLine( "!!CB: ISampleGrabberCB.SampleCB" );
    return 0;
  }

  /// <summary> buffer callback, COULD BE FROM FOREIGN THREAD. </summary>
  int ISampleGrabberCB.BufferCB( double SampleTime, IntPtr pBuffer, int BufferLen )
  {
    if ( captured || ( savedArray == null ) )
    {
      Trace.WriteLine( "!!CB: ISampleGrabberCB.BufferCB" );
      return 0;
    }

    captured = true;
    bufferedSize = BufferLen;
    Trace.WriteLine( "!!CB: ISampleGrabberCB.BufferCB  !GRAB! size = " + BufferLen.ToString() );
    if ( ( pBuffer != IntPtr.Zero ) && ( BufferLen > 1000 ) && ( BufferLen <= savedArray.Length ) )
      Marshal.Copy( pBuffer, savedArray, 0, BufferLen );
    else
      Trace.WriteLine( "    !!!GRAB! failed " );
    this.BeginInvoke( new CaptureDone( this.OnCaptureDone ) );
    return 0;
  }

  /// <summary> flag to detect first Form appearance </summary>
  private bool firstActive;

  /// <summary> base filter of the actually used video devices. </summary>
  private IBaseFilter capFilter;

  /// <summary> graph builder interface. </summary>
  private IGraphBuilder graphBuilder;

  /// <summary> capture graph builder interface. </summary>
  private ICaptureGraphBuilder2 capGraph;
  private ISampleGrabber sampGrabber;

  /// <summary> control interface. </summary>
  private IMediaControl mediaCtrl;

  /// <summary> event interface. </summary>
  private IMediaEventEx mediaEvt;

  /// <summary> video window interface. </summary>
  private IVideoWindow videoWin;

  /// <summary> grabber filter interface. </summary>
  private IBaseFilter baseGrabFlt;

  /// <summary> structure describing the bitmap to grab. </summary>
  private VideoInfoHeader videoInfoHeader;
  private bool captured = true;
  private int bufferedSize;

  /// <summary> buffer for bitmap data. </summary>
  private byte[] savedArray;

  /// <summary> list of installed video devices. </summary>
  private ArrayList capDevices;

  /// <summary> last image from cam. </summary>
  private Bitmap fromCam;

  private const int WM_GRAPHNOTIFY = 0x00008001;	// message from graph
  private const int WM_SIZE = 0x00000005;

  private const int WS_CHILD = 0x40000000;	// attributes for video window
  private const int WS_CLIPCHILDREN = 0x02000000;
  private const int WS_CLIPSIBLINGS = 0x04000000;

  /// <summary> event when callback has finished (ISampleGrabberCB.BufferCB). </summary>
  private delegate void CaptureDone();

  #if DEBUG
  private int rotCookie = 0;
  #endif
}

internal enum PlayState
{
  Init, Stopped, Paused, Running
}

}
