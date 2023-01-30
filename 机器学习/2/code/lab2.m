
clearvars
close

% 4 Logistic Regression

load('ex2x.dat')
load('ex2y.dat')
x = ex2x;
y = ex2y;
m = length(y);
x = [ones(m, 1), x];
x_ = x;
% x = zscore(x);  % *标准化*
for i = 2 : size(x, 2)
    x_mean(i) = mean(x(:, i));
    x_std(i) = std(x(:, i));
    x(:, i) = (x(:, i) - x_mean(i)) ./ x_std(i);    % /0 err!
end
pos = find(y == 1);
neg = find(y == 0);

% figure
% hold on
% plot(x(pos, 2), x(pos, 3), '+')
% plot(x(neg, 2), x(neg, 3), 'o')
% ylabel('Exam 2 score')
% xlabel('Exam 1 score')

g = inline('1.0 ./ (1.0 + exp(-z))');   % 内联函数g(z)

theta = zeros(size(x, 2), 1);
max_err = 1e-6;   % 可接受偏差
max_iteration = 5005;   % 最大迭代轮数
alpha = [0.1 0.05 0.01 0.005 0.001 0.0005 0.0001 0.00001];    % 学习率
L = zeros(max_iteration, 1);

for t = 1 : max_iteration
    z = x * theta;
    L(t) = (-y' * log(g(z)) - (1 - y)' * log(1 - g(z))) / m;
    if t > 1 && abs(L(t) - L(t - 1)) < max_err
        break
    end
    
    % method 1: GD
%     grad = x' * (g(z) - y) / m;
%     theta = theta - alpha(1) * grad;

    % method 2: Newton’s Method
    grad = x' * (g(z) - y) / m;
    % 将向量(1-g(z)),g(z)对角化以构建矩阵表达
    % x：(m*n)
    % diag(1-g(z))：m*m
    H = (x' * diag(1 - g(z)) * diag(g(z)) * x) / m;
    theta = theta - inv(H) * grad;
%     theta = theta - (H \ grad)';
end

plot(1 : t, L(1 : t));
ylabel('L(\theta)')
xlabel('iteration')
% title('Loss Function(\alpha=0.1)')
title('Loss Function(Newton’s Method)')

% figure
% hold on
% plot(x_(pos, 2), x_(pos, 3), '+')
% plot(x_(neg, 2), x_(neg, 3), 'o')
% % theta(1)+theta(2)*x(2)+theta(3)*x(3)=0
% x_plot = linspace(-2.5, 2.5, 500);
% y_plot = (-(theta(2) * x_plot + theta(1)) / theta(3));
% plot(x_plot .* x_std(2) + x_mean(2), y_plot .* x_std(3) + x_mean(3), '.');
% ylabel('Exam 2 score')
% xlabel('Exam 1 score')
% legend('Admitted', 'Not Admitted', 'Dicision Boundary')


% 1. 选择学习率为0.1时，507轮迭代后收敛到与期望值偏差1e-6内；
% 2. [-0.051212370653521;1.434821090552851;1.531312854024689]
% 3. 如图
% 4. 如图
% 5. 0.663088858956603
% x_predict = [1 20 80];
% for i = 2 : size(x, 2)
%     x_predict(:, i) = (x_predict(:, i) - x_mean(i)) ./ x_std(i);    % /0 err!
% end
% z = x_predict * theta
% not_admitted_prop = 1 - g(z);

% 1. [-0.056594943597035;1.472022204512080;1.570583537977476]
% 2. 如图
% 3. dicision boundary和GD几乎相同，两种方法求解的函数极值相同，最终theta值十分接近
% 4. 0.668021803681154
% 5. 牛顿法在该问题上是GD的优化版本，用海森矩阵代替学习率常数，
% 将曲面曲率纳入考虑，具有更强的收敛能力，但代价是海森矩阵的计算








