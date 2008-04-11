%% CS 370 - Assignment 5
%% Read image and convert to binary
train = imread( 'A_Train.png' );
train = rgb2gray( train );
train = im2bw( train, graythresh( train ) );
%% Check each block
for i = 0:9
    min = i * 50;
    max = min + 50;
    cur = imcrop( train, [min 0 max 49] );
    imshow( cur );
end
