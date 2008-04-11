%% CS 370 - Assignment 5

%% Read image, keep backup
curImg = imread( 'A_Train.png' );
cached = curImg;

%% Convert image to black & white
curImg = rgb2gray( curImg );
curImg = im2bw( curImg, graythresh( curImg ) );

%% Find edges and fill
curImg = edge( uint8( curImg ) );
curImg = imdilate( curImg, strel( 'square', 2 ) );
curImg = imfill( curImg, 'holes' );

%% Find the objects
label = bwlabel( curImg );
props = regionprops( label );
box   = reshape( [props.BoundingBox], [4 10] );

%% Draw bounding boxes
imshow( cached );
hold on;
for i = 1:10
    rectangle( 'position', box( :, i ), 'edgecolor', 'r' );
end

%% Create and train neural net
i10 = eye(10);

%% Check each block
for i = 1:10
    %min = i * 50;
    cur = imcrop( cached, box( :, i ) );
end

