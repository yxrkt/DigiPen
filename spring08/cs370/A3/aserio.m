% load woman image into f, pattern into g
f = imread('woman.tif')
g = imread('test_pattern.tif')

% spectrum, magnitude, angle of f
sF = fft2(f)
mF = abs(sF)
aF = angle(sF)

% spectrum, magnitude, angle of g
sG = fft2(g)
mG = abs(sG)
aG = angle(sG)

% convert images to grayscale
colormap(gray)

% create plots for dif parts of assignments
partAi  = subplot(2, 2, 1)
partAii = subplot(2, 2, 2)
partB   = subplot(2, 2, 3)
partC   = subplot(2, 2, 4)

% Part A.i: show phase image
imagesc(aF, 'parent', partAi)

% Part A.ii: inverse using phase
imagesc(abs(ifft2(exp(j * aF))), 'parent', partAii, [0,.01])

% Part B: inverse using spectrum
imagesc(abs(ifft2(mF)), 'parent', partB, [0,80])

% Part C: inverse using mag of g and angle of f
imagesc(abs(ifft2(complex(mG .* cos(aF), mG .* sin(aF)))))