
clearvars
close

%% Load data
trainNum = randi([5, 7]);
dimx = 112; dimy = 92;  % input image:112×92
classNum = 40;
[trainData, testData] = loadDataSet(trainNum, dimx, dimy, classNum);

%% PCA && OvO SVM
K = [1, 2, 4, 8, 16, 32, 48, 64, 80, 96];
data = [trainData; testData];
division = size(trainData, 1);
totalTest = size(testData, 1);

% store result
cr_ovo = zeros(length(K), 1);   % one vs one
misclass = cell(length(K), 1);
% cr_ova = zeros(length(K), 1);   % one vs all

for i = 1 : length(K)
% for  i = 1 : 1
    %% 对trainData和testData应用PCA，降至K维
    weight = 0.9; rK = -1;
    [datak, rK] = EVD_PCA(data', K(i), weight);
%     [datak] = SVD_PCA(data', K(i));
%     [datak, rK] = EVD_PCA(data', -1, weight);   % rK：最终使用K值
    datak = datak';
    if rK < 0
        rK = K(i);
    end
    
    trainDatak = datak(1 : division, :);
    testDatak = datak((division + 1) : size(data, 1), :);
    
    %% 基于一对一方式实现多分类器
    [cr_ovo(i), misclass{i}] = ovoMultiClassModel(trainDatak, testDatak, classNum, rK, trainNum);
    %% 基于一对多方式实现多分类器
%     [cr_ovo(i), ~] = ovaMultiClassModel(trainDatak, testDatak, classNum, rK, trainNum);
end

%% save result
save('plotData', 'K', 'cr_ovo');    
save('misclassData', 'misclass');

%% todo
% 1.找出Test Set中误分类项；
% 2.前K个特征值之和占特征值之和的所占比例为rate，绘制(K,rate)的函数关系图；
% 3.实现SVD-PCA降维.

