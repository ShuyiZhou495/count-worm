function [upperThanLine, UtoD] = InitializUTL(chong, yLine, UtoD)
if(chong == zeros(1, 4))
    upperThanLine = -1;
    return;
end
if(chong(2)>=yLine) 
    upperThanLine = 0;        
end
if(chong(2)+chong(4)>=yLine &&chong(2)<yLine)
    upperThanLine = 0.5;
    UtoD = 0.5;
end
if(chong(2)+chong(4)<yLine)
    upperThanLine = 1;
end
end