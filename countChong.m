function [count, upperThanLine_return, UtoD] = countChong(upperThanLine, yLine, count, chong, UtoD)
if(upperThanLine == -1 && chong(1)>0)
    [upperThanLine_return, UtoD] =  InitializUTL(chong,yLine, UtoD);
else
    if(upperThanLine == 1 && chong(2)+chong(4)>yLine)
        upperThanLine_return = 0.5;
        UtoD = 1;
    else 
        if(upperThanLine == 0.5 && chong(2)>yLine && (UtoD==1 || UtoD==0.5))
            count = count + 1;
            upperThanLine_return = 0;
            UtoD = 0;
        else
            if(upperThanLine == 0 && chong(2)<yLine)
                upperThanLine_return = 0.5;
                UtoD = -1;
            else                
                if(upperThanLine ==0.5 && chong(2)+chong(4)<yLine && (UtoD==-1||UtoD==0.5))
                    count = count + 1;
                    upperThanLine_return = 1;
                    UtoD = 0;
                else
                    upperThanLine_return = upperThanLine;
                end
            end
        end
    end
end
end