

clearvars

X = [2005 : 1 : 2009]';
Y = [12, 19, 29, 37, 45]';
m = length(Y);
X = [ones(m, 1), X];
theta = inv(X' * X) * X' * Y;
% [-1.683039999614656e+04;8.399999998082421]
% Y_predict = X * theta;

X = [X; 1, 2012];
Y_predict = X * theta;

% err? %

% x = X;
% y = Y;
% m = length(y);
% x = [ones(m, 1), x];
% 
% theta = [1000; 0];
% theta_new = [0; 0];
% alpha = 1e-5;
% for t = 1 : 500
% %     theta_new(1) = theta(1) - alpha * sum((x * theta - y) .* x(:, 1)) / m;
% %     theta_new(2) = theta(2) - alpha * sum((x * theta - y) .* x(:, 2)) / m;
%     for i = 1 : 2
%         tmp = 0;
%         for j = 1 : m
%             tmp = tmp + (x(j, :) * theta - y(j)) * x(j, i);
%         end
%         theta_new(i) = theta(i) - alpha * tmp / m;
%     end
%     theta = theta_new;
% end
% y_predict = x * theta;


