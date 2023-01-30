
clearvars
close all

% [x, y] = strimage();    % 12665
% save('train_data', 'x', 'y');
load('train_data');
load('test_data');

pos = find(y > 0); neg = find(y < 0);
m = length(x);
% 使用全部训练集，H矩阵大小为12665*12665，运算巨大，耗时较久
% 因此采样部分训练集，大小为tr_size
rp = randperm(m);
tr_size = 1000;
samp = rp(1 : tr_size);
x = x(samp, :); y = y(samp);
m = length(x);

C = 1;
% SVM
% 构建目标函数
H = zeros(m);
for i = 1 : m
    for j = 1 : m
        H(i, j) = y(i) * y (j) * x(i, :) * x(j, :)';
    end
end
f = (-1) * ones(m, 1);
% 构建约束
Aeq = y';
beq = 0;
lb = zeros(m, 1);
ub = zeros(m, 1);
ub(:) = C;
% 利用quadprog求解器求解对偶问题
% quadprog(H,f,A,b,Aeq,beq,lb,ub)
[alpha, fval] = quadprog(H, f, [], [], Aeq, beq, lb, ub);

% 求support vector
alpha(find(alpha < 1e-8)) = 0;
sv = find(alpha > 0 & alpha < C);
w = 0;  % omega
for i = 1 : length(sv)
    w = w + alpha(sv(i)) * y(sv(i)) * x (sv(i), :)';
end

num = y - x * w;
b = sum(num(sv)) / length(sv);

% 在训练集上的分类情况
err_rate = classify(x, y, w, b, samp);
fprintf('Miss rate on Train Set: %f\n', err_rate);
% 在测试集上的分类情况
err_rate = classify(test_x, test_y, w, b, [1 : length(test_x)]);
fprintf('Miss rate on Test Set: %f\n', err_rate);
% fprintf('Miss rate of SVM on Handwritten Digit Recognition: %f\n', err_rate);


function err_rate = classify(x, y, w, b, samp)
    err_cnt = 0;
    for i = 1 : length(x)
        tmp = b + x(i, :) * w;
        if tmp * y(i) < 0
            err_cnt = err_cnt + 1;
%             show_image(samp(i));
        end
    end
    err_rate = err_cnt / length(x);
end





