clear
close all

%% 不同K值下模型的准确率
load('plotData.mat');
figure
plot(K, cr_ovo, 'LineWidth', 2, 'MarkerSize', 30);
xlabel('K values');
ylabel('Accuracy');
title('Accuracy under different K values');

%% 找出Test Set中误分类项
% load('misclassData.mat');
% std = misclass{7}{1}(:, 1);
% res = misclass{7}{1}(:, 2);
% misc = find(std ~= res);
% for i = 1 : length(misc)
%     path = ['orl_faces\s'];
%     id = randi([1, 10]);
%     stdFileName = [path, num2str(std(misc(i))), '\', num2str(id), '.pgm'];
%     stdImage = double(imread(stdFileName));
%     resFileName = [path, num2str(res(misc(i))), '\', num2str(id), '.pgm'];
%     resImage = double(imread(resFileName));
%     figure
%     subplot(2, 1, 1)
%     tit = strcat('Class', num2str(std(misc(i))));
%     imshow(uint8(stdImage)), title(tit);
%     subplot(2, 1, 2)
%     tit = strcat('Class', num2str(res(misc(i))));
%     imshow(uint8(resImage)), title(tit);
% end

%% (K,weight)的函数关系图
% load('eigenData.mat');
% n = length(eigenVal);
% n = 100;
% sumVal = sum(eigenVal);
% rate = zeros(n, 1);
% sum = 0;
% for i = 1 : n
%     sum = sum + eigenVal(i);
%     rate(i) = sum / sumVal;
% end
% pt = plot([1 : n], rate, 'LineWidth', 2, 'MarkerSize', 30);
% xlabel('K values');
% ylabel('Weight');
% title("Weight of first K eigenvalues"); % 前K个特征值占比




