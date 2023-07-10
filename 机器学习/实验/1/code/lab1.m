
clearvars
close all

% % 3. 2D Linear Regression
% x = load('ex1_1x.dat');
% y = load('ex1_1y.dat');
% 
% figure
% plot(x, y, 'o');
% ylabel('Height in meters')
% xlabel('Age in years')
% 
% m = length(y);
% x = [ones(m, 1), x];
% 
% theta = [0; 0];
% theta_new = [0; 0];
% alpha = 0.07;
% % first iteration
% theta_new(1) = theta(1) - alpha * sum((x * theta - y) .* 1) / m;
% theta_new(2) = theta(2) - alpha * sum((x * theta - y) .* x(:, 2)) / m;
% theta = theta_new;
% % [0.0745280238220000,0.380021672507806]
% % 1500 iterations
% for t = 1 : 1500
%     theta_new(1) = theta(1) - alpha * sum((x * theta - y) .* 1) / m;
%     theta_new(2) = theta(2) - alpha * sum((x * theta - y) .* x(:, 2)) / m;
%     theta = theta_new;
% end
% % [0.750150480371403;0.063883359379671]
% hold on
% plot(x(:, 2), x * theta, '-')
% legend('Training data', 'Linear regression')
% % predict
% h_predict = [1, 3.5; 1, 7] * theta;
% % [0.973742238200250;1.197333996029097]
% 
% % 4. understanding J(theta)
% J_vals = zeros(100, 100);
% theta0_vals = linspace(-3, 3, 100);
% theta1_vals = linspace(-1, 1, 100);
% % theta0_vals = logspace(-3, 3, 100);
% % theta1_vals = logspace(-1, 1, 100);
% for i = 1 : length(theta0_vals)
%     for j = 1 : length(theta1_vals)
%         t = [theta0_vals(i); theta0_vals(j)];
%         J_vals(i, j) = sum((x * [theta0_vals(i); theta1_vals(j)] - y) .^ 2) / (2 * m);
%     end
% end
% figure
% surf(theta0_vals, theta1_vals, J_vals);
% xlabel('\theta_0'); ylabel('\theta_1'); zlabel('J(\theta)');

% 5. Multivariate Linear Regression
x = load('ex1_2x.dat');
y = load('ex1_2y.dat');
m = length(y);
x = [ones(m, 1), x];
sigma = std(x); % 标准差
mu = mean(x);   % 平均值
% 数据标准化，数据数值范围相差较大，通过标准化提高梯度下降算法效率
x(:, 2) = (x(:, 2) - mu(2)) ./ sigma(2);
x(:, 3) = (x(:, 3) - mu(3)) ./ sigma(3);

theta = zeros(size(x(1, :)));
theta_new = zeros(size(x(1, :)));
theta = theta(:); theta_new = theta_new(:);
alpha = [0.001 0.01 0.1 1 10];
J = zeros(5, 50, 1);

% for t = 1 : 5
    for num_iterations = 1 : 50
        J(num_iterations) = (x * theta - y)' * (x * theta - y) / (2 * m);
        for i = 1 : size(x(1, :))
            theta_new(i) = theta(i) - alpha(3) * sum((x * theta - y) .* x(:, i)) / m;
        end
        theta = theta_new;
    end
% end

% figure
% for t = 1 : 5
%     hold on
%     plot(0 : 49, J(t, 1 : 50), '-')
% end
% xlabel('Number of iterations'); ylabel('Cost J');
% legend('\alpha=0.001', '\alpha=0.01', '\alpha=0.1', '\alpha=1', '\alpha=10')
% axis([-1 50 0 1e11]);

% Answer the following questions:
% 1. 学习率过大无法求得结果，过小收敛速度很慢
% 2. （a）[338658.249249295;0;0]
% （b）predict
x_test = [1, 1650, 3];
y_predict = x_test * theta;
% 3.386582492492952e+05




