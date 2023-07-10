
function [corrRate, misclass] = ovoMultiClassModel(trainData, testData, classNum, K, trainNum)
    fprintf('Running one vs one multiclass model...\n');
    %% Step 1: Assign label
    testNum = 10 - trainNum;
    N = classNum * (classNum - 1) / 2;
    trainPart = cell(N, 1); 
%     testPart = cell(N, 1);
    cnt = 1;
    map = zeros(N, 2);  % 第cnt个二分类器的对抗分类类别i,j
    
    for i = 1 : (classNum - 1)
        for j = (i + 1) : classNum
            trainPart{cnt, 1} = [[trainData((i - 1) * trainNum + 1 : i * trainNum, :), ones(trainNum, 1)]; ...
                [trainData((j - 1) * trainNum + 1 : j * trainNum, :), -ones(trainNum, 1)]];
%             testPart{cnt, 1} = [[testData((i - 1) * testNum + 1 : i * testNum, :), ones(testNum, 1)]; ...
%                 [testData((j - 1) * testNum + 1 : j * testNum, :), -ones(testNum, 1)]];
            map(cnt, 1) = i; map(cnt, 2) = j;
            cnt = cnt + 1;
        end
    end
%     save('trainPart.mat', 'trainPart');
    
    %% Step 2: Train SVM
    A = zeros(N, 2 * trainNum); % ? num(alpha)=2*trainNum
    W = zeros(N, size(trainData, 2));   % N×K
    B = zeros(N, 1);
    for i = 1 : N
        SVMObj = mySVM(trainPart{i, 1}(:, (1 : K)), trainPart{i, 1}(:, K + 1), 1);
        A(i, :) = SVMObj.alpha';
        W(i, :) = SVMObj.w';
        B(i) = SVMObj.b;
    end
    
    %% Step 3: Test classifying
    % SVM函数输出值
    totalTest = size(testData, 1);
    val = zeros(totalTest, N);
    for i = 1 : N
        val(:, i) = (testData * W(i, :)')' + B(i);
        % (?×1)=((?×K)*(1×K)')'+(?×1)，其中?为测试数据个数(totalTest)
    end
    % 第i类别参与(40×39/2)个二分类SVM中的39个
    part = zeros(classNum, classNum - 1);
    for i = 1 : classNum
        part(i, :) = find(map(:, 1) == i | map(:, 2) == i);
    end
    % f(x)=arg max_{s}∑_{t}f_{s,t}(x)
    %% A. 一对一SVM分类器结果
    res = zeros(totalTest, 1);
    for i = 1 : totalTest    % 对所有测试数据
        voteCnt = zeros(classNum, 1);
        for j = 1 : classNum    % 对所有可能分类
            for k = 1 : classNum - 1
                if val(i, part(j, k)) > 0 && map(part(j, k), 1) == j
                    voteCnt(j) = voteCnt(j) + 1;
                elseif val(i, part(j, k)) < 0 && map(part(j, k), 2) == j
                    voteCnt(j) = voteCnt(j) + 1;
                end
            end
        end
        [~, maxOne] = max(voteCnt);
        res(i) = maxOne;
    end
    %% B. 标准结果
    std = zeros(totalTest, 1);
    for i = 1 : classNum
        std((i - 1) * testNum + 1 : i * testNum) = i;
    end
    %% C. 对比统计
    corrSet = find(res == std);
    corrRate = length(corrSet) / totalTest;
    misclass = cell(1, 1);
    misclass{1} = [std, res];
    fprintf('one vs one multiclass done\n');
end

