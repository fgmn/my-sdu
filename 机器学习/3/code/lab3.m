

clearvars
close

% 3 Regularized Linear Regression
% load('ex3Linx.dat');
% load('ex3Liny.dat');
% x = ex3Linx;
% y = ex3Liny;
% m = length(y);
% 
% X = [ones(m, 1), x, x .^ 2, x .^ 3, x .^ 4, x .^ 5];
% lambda = [0 1 10];
% D = eye(6);
% D(1, 1) = 0;
% 
% figure
% hold on
% plot(x, y, 'or');
% x_plot = -1 : 0.01 : 1;
% x_plot = x_plot(:);
% X_plot = [ones(length(x_plot), 1), x_plot, x_plot .^ 2, x_plot .^ 3, x_plot .^ 4, x_plot .^ 5];
% 
% for i = 1 : length(lambda)
%     % the normal equations solution to regularized linear regression
%     theta = inv(X' * X + lambda(i) * D) * X' * y;    
%     plot(x_plot, X_plot * theta, '--', 'LineWidth', 2);
% end
% title('Regularized Linear Regression');
% legend('input data', '\lambda=0', '\lambda=1', '\lambda=10');


% 4 Regularized Logistic Regression
x = load('ex3Logx.dat');
y = load('ex3Logy.dat');


u = x(:, 1); v = x(:, 2);
X = map_feature(u, v);  % 117 * 28
[m, n] = size(X);
% theta = zeros(n, 1);
lambda = [0 1 5 10 20];
max_err = 1e-10;   % 可接受偏差
max_iteration = 5005;   % 最大迭代轮数
L = zeros(max_iteration, 1);
g = inline('1.0 ./ (1.0 + exp(-z))');   % 内联函数g(z)
color = ['r', 'g', 'b', 'y', 'p'];

figure
pos = find (y); neg = find(y == 0);
plot(x(pos, 1), x(pos, 2), '+')
hold on
plot(x(neg, 1), x(neg, 2), 'o')
u = linspace(-1, 1.5, 200);
v = linspace(-1, 1.5, 200);
z = zeros(length(u), length(v));
title('Regularized Logistic Regression')
xlabel('u')
ylabel('v')

norm_theta = zeros(length(lambda), 1);

for k = 1 : length(lambda)
% k = 2;
    theta = zeros(n, 1);
    for t = 1 : max_iteration
        z = X * theta;
        L(t) = (-y' * log(g(z)) - (1 - y)' * log(1 - g(z))) / m;
        L(t) = L(t) + sum(theta(2 : n) .^ 2) * lambda(k) / (2 * m);   
        if t > 1 && abs(L(t) - L(t - 1)) < max_err
            break
        end
        grad = X' * (g(z) - y) / m;
        H = (X' * diag(1 - g(z)) * diag(g(z)) * X) / m;
        % regularized
        tmp = theta; tmp(1) = 0;    % 所有正则化均不包括 θ0 项
        grad = grad + tmp * (lambda(k) / m);
        tmp = diag(ones(n, 1));
        tmp(1, 1) = 0;
        H = H + tmp * (lambda(k) / m);
%         theta = theta - inv(H) * grad;
        theta = theta - (H \ grad);
    end
    norm_theta(k) = norm(theta, 2);
    % contour
    for i = 1 : length(u)
        for j = 1 : length(v)
            z(i, j) = map_feature(u(i), v(j)) * theta;
        end
    end
    z = z';
    contour(u, v, z, [0 0], color(k))  
end
legend('pos', 'neg', '\lambda=0', '\lambda=1', '\lambda=5', '\lambda=10', '\lambda=20')

% plot(1 : t, L(1 : t));
% ylabel('L(\theta)')
% xlabel('iteration')
% title('Loss Function(\lambda=1)')

% figure
% plot(1 : length(lambda), norm_theta);

% lambda增大，减少对训练数据的依赖，可能表现为欠拟合
% lambda减小，增加对训练数据的依赖，可能表现为过拟合





