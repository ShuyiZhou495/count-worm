function transform()
videoReader = vision.VideoFileReader('day2.mp4');
videoFWriter = vision.VideoFileWriter('day_avi.avi' ,'FrameRate',...
    videoReader.info.VideoFrameRate);
while ~isDone(videoReader)
    frame = step(videoReader);
    step(videoFWriter,frame); % write the video
end
release(videoReader); % close the video file
release(videoFwriter); % close the video file
end