%% CS 370 - Assignment 5

%% Read image, keep backup
curImg = imread( uigetfile( { '*.png'; '*.bmp'; '*.jpg'; '*.tif'; '*.gif'; } ) );
cached = curImg;

%% Convert image to black & white
curImg = rgb2gray( curImg );
curImg = im2bw( curImg, graythresh( curImg ) );
bwImg  = curImg;

%% Find edges and fill
curImg = edge( uint8( curImg ) );
curImg = imdilate( curImg, strel( 'square', 2 ) );
curImg = imfill( curImg, 'holes' );

%% Find the objects
label = bwlabel( curImg );
props = regionprops( label );
boxes = reshape( [props.BoundingBox], [4 10] );

%% Draw bounding boxes
%imshow( cached );
%hold on;
%for i = 1:10
%    rectangle( 'position', boxes( :, i ), 'edgecolor', 'r' );
%end

%% Create vectors of the images
obj_vecs = zeros( 35, 10 );
for i = 1:10
    % normalize each object
    cur_obj = imcrop( bwImg, boxes( :, i ) );
    %imshow( cur_obj );
    obj_n   = imresize( cur_obj, [70, 50] );
    %imshow( obj_n );
    
    % convert normalized image to vector
    objvec = zeros( 1, 35 );
    for r = 1:7
        for c = 1:5
            temp = sum( obj_n( (r * 10 - 9):(r * 10), (c * 10 - 9):(c * 10) ) );
            objvec( (r - 1) * 5 + c ) = sum( temp );
        end
    end
    
    % normalize colors to be from 0 to 1 and invert so black is foreground
    objvec = ( ( 100 - objvec ) / 100 );
    obj_vecs( :, i ) = ( objvec' );
    
end

%% Create and train neural net
i10 = eye(10);

[S2, N] = size( i10 );
S1 = 10;

% create a feed-forward backpropagation network
net = newff( minmax( obj_vecs ), [S1 S2],{'logsig' 'logsig'}, 'traingdx' );
net.LW{2, 1}          = net.LW{2, 1} * .01;
net.b{2}              = net.b{2} * .01;
net.performFcn        = 'sse';
net.trainParam.goal   = .1;
net.trainParam.show   = 20;
net.trainParam.epochs = 5000;
net.trainParam.mc     = .95;
%[net, tr]             = 
train( net, obj_vecs, i10 );
