function newChong = InitializChong(chong, bbox, x1, x2)
newChong = chong;
temp = [];
for i = 1:size(bbox, 1)
    if(bbox(i, 1)>= x2 && bbox(i, 1)<=x1)
        if(isempty(temp))
            temp = bbox(i,:);
        else
            if(bbox(i,2)>temp(2))
                temp = bbox(i,:);
            end
        end
    end
end
if(size(temp, 2)==4)
    newChong = temp;
else
    newChong = chong;
end
end