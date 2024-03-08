function DetectChong()
num = 10;
count(num,1) = 0;
yLine = 790;
x1 = [490, 680, 780, 880, 1000, 1300, 1400, 1500, 1600, 1700];
x2 = [380, 580, 690, 800, 890, 1190, 1300, 1400, 1520, 1610];
foregroundDetector = vision.ForegroundDetector('NumGaussians', 7, ...
    'NumTrainingFrames', 100);

videoReader = vision.VideoFileReader('day2.mp4');

blobAnalysis = vision.BlobAnalysis('BoundingBoxOutputPort', true, ...
    'AreaOutputPort', false, 'CentroidOutputPort', false, ...
    'MinimumBlobArea', 400);

chong(num, 4) = 0;
UtoD(num,1) = 0;
upperThanLine(num,1) = -1;
for i = 1:num
    upperThanLine(i) = InitializUTL(chong(i), yLine);
end

videoPlayer = vision.VideoPlayer('Name', 'Chong');
videoPlayer.Position(3:4) = [1120,630];  % window size: [width, height]
videoFWriter = vision.VideoFileWriter('count_chong.mp4','FileFormat', 'MPEG4' ,'FrameRate',...
    videoReader.info.VideoFrameRate);

se = strel('square', 5); % morphological filter for noise removal
frame_num = 0;
while ~isDone(videoReader)
    frame_num = frame_num + 1;
    frame = step(videoReader); % read the next video frame
    
    % Detect the foreground in the current video frame
    foreground = step(foregroundDetector, frame);
    
    % Use morphological opening to remove noise in the foreground
    filteredForeground = imopen(foreground, se);
    
    % Detect the connected components with the specified minimum area, and
    % compute their bounding boxes
    bbox = step(blobAnalysis, filteredForeground);
    
    for i = 1:num
        [count(i), upperThanLine(i), UtoD(i)] = countChong(upperThanLine(i), yLine, count(i), chong(i, :), UtoD(i));
        chong(i, :) = InitializChong(chong(i, :), bbox, x1(i), x2(i));
    end
    
    % Draw bounding boxes around the detected cars
    result = insertShape(frame, 'Rectangle', bbox, 'Color', 'green');
    
    % Draw a line
    yEnd(1:num) = yLine;
    result = insertShape(result, 'Line', [x1', yEnd', x2', yEnd']); 

    % Display the number of cars found in the video frame
    %numChong = size(bbox, 1);
    result = insertText(result, [x1',yEnd'], count, 'BoxOpacity', 1, ...
        'FontSize', 14);

    step(videoPlayer, result);  % display the results
    step(videoFWriter,result); % write the video
    
   
end

release(videoReader); % close the video file
release(videoFwriter); % close the video file
end
