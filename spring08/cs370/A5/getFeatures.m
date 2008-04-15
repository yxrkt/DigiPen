function fVec = getFeatures( image )

%% Read image, keep backup
curImg = image;

%% Convert image to black & white
curImg = rgb2gray( curImg );
curImg = im2bw( curImg, graythresh( curImg ) );

%% Find edges and fill
curImg = edge( uint8( curImg ) );
curImg = imdilate( curImg, strel( 'square', 2 ) );
curImg = imfill( curImg, 'holes' );

%% Find the objects
label = bwlabel( curImg );
props = regionprops( label, 'area' );
area  = [props.Area];
props = regionprops( label, 'perimeter' );
perim = [props.Perimeter];
props = regionprops( label, 'extent' );
ext   = [props.Extent];
fVec  = vertcat( area, perim, ext );

end