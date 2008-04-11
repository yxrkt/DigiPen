%% CS 370 - Assignment 5

%% Read image, keep backup
train  = imread( 'A_Train.png' );
cached = train;

%% Convert image to black & white
train = rgb2gray( train );
train = im2bw( train, graythresh( train ) );

%% Find edges and fill
train = edge( uint8( train ) );
train = imdilate( train, strel( 'square', 2 ) );
train = imfill( train, 'holes' );
imshow( train );

%% Find the objects
[Ilabel num] = bwlabel( train );
Iprops = regionprops( Ilabel );

%% Check each block
for i = 0:9
    min = i * 50;
    cur = imcrop( train, [min 0 49 49] );
end
