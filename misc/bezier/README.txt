//====================================================
//  MAT 300A
//  Project 1 - Bezier Curves
//  By Alex Serio
//====================================================

How to use:
  ~ Left click to create a control point
  ~ Toggle control polygon on or off with the toolbar
  ~ Toggle between de Casteljau and Bernstein with
    toolbar (Bernstein is on by default)
  ~ Press 'c' (no caps) to clear all control points
  ~ Use middle mouse button to drag control points

Implementation:
  ~ Points found recursively using both de Casteljau's 
    algorithm and Bernstein polynomials
  ~ Space between points covered with lines. Any holes
    in curve are due to bad line algorithm.
  ~ Using MFC

Other notes:
  ~ Notice that Bernstein breaks after 14 control
    points. This is because the degree of the poly-
    nomial is 13, and 13! > 6,000,000, which is more
    than a signed or an unsigned integer can hold.
  ~ de Casteljau's algorithm, while less efficient, is
    numerically stable, and will not crash given any
    number of control points.