
% 加载数据集
function [trainData, testData] = loadDataSet(trainNum, dimx, dimy, classNum)
    fprintf('Loading face data set...\n');
    path = ['orl_faces\'];
    testNum = 10 - trainNum;    % 取5-7张训练，其余做测试
    for i = 1 : classNum
        id = ['s', num2str(i)];
        rp = randperm(10);
        % train data
        for j = 1 : trainNum
            fileName = [path, id, '\', num2str(rp(j)), '.pgm'];
            img = double(imread(fileName));
            trainData(trainNum * (i - 1) + j, :) = reshape(img, 1, dimx * dimy);
        end
        % test data
        for j = 1 : testNum
            fileName = [path, id, '\', num2str(rp(j + trainNum)), '.pgm'];
            img = double(imread(fileName));
            testData(testNum * (i - 1) + j, :) = reshape(img, 1, dimx * dimy);
        end
    end
    fprintf('Load done\n');
end