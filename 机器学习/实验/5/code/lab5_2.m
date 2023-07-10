
clearvars
close

train_data = load('training_3.txt');
x = train_data(:, [1 2]);
y = train_data(:, 3);
pos = find(y > 0); neg = find(y < 0);

% figure
% hold on
% title('Non-Linear Data');
% plot(x(pos, 1), x(pos, 2), '.r');
% plot(x(neg, 1), x(neg, 2), '.b');
% xlabel('x_1'); ylabel('x_2');
% legend('pos', 'neg');
sgt = sgtitle('Decision Boundary with Increasing \gamma');
sgt.FontSize = 20;

m = length(x);

gamma = [1, 10, 100, 1000];
for t = 1 : length(gamma)
    kmat = get_kernel_mat(x, gamma(t));

    % SVMstart
    % 构建目标函数
    H = zeros(m);
    for i = 1 : m
        for j = 1 : m
            H(i, j) = y(i) * y (j) * kmat(i, j);
        end
    end
    f = (-1) * ones(m, 1);
    % 构建约束
    Aeq = y';
    beq = 0;
    lb = zeros(m, 1);
    ub = [];
    % 利用quadprog求解器求解对偶问题
    % quadprog(H,f,A,b,Aeq,beq,lb,ub)
    [alpha, fval] = quadprog(H, f, [], [], Aeq, beq, lb, ub);

    % 求support vector 
    alpha(find(alpha < 1e-8)) = 0;
    sv = find(alpha > 0);
    sum0 = 0;
    for i = 1 : length(sv)
        sum1 = 0;
        for j = 1 : length(sv)
            sum1 = sum1 + alpha(sv(j)) * y(sv(j)) * kmat(sv(i), sv(j));
        end
        sum0 = sum0 + y(sv(i)) - sum1;
    end
    b = sum0 / length(sv);
    % omega无法直接求取

    % Make classification predictions over a grid of values
    xplot = linspace(min(x(:, 1)), max(x(:, 1)), 100)';
    yplot = linspace(min(x(:, 2)), max(x(:, 2)), 100)';
    [X, Y] = meshgrid(xplot, yplot);
    vals = zeros(size(X));
    % For each point in this grid, you need to compute its decision
    % value. Store the decision values in vals.
    for i = 1 : length(vals)
        for j = 1 : length(vals)
            sumk = 0; tx = [X(i, j), Y(i, j)];
            for k = 1 : length(sv)  % 仅考虑support vector
                sumk = sumk + alpha(sv(k)) * y(sv(k)) * exp(-gamma(t) * norm(x(sv(k), :) - tx) ^ 2);
            end
            vals(i, j) = sumk + b;
        end
    end
    subplot(2, 2, t);
    hold on
    plot(x(pos, 1), x(pos, 2), '.r');
    plot(x(neg, 1), x(neg, 2), '.b');
    xlabel('x_1'); ylabel('x_2');
    str = strcat('\gamma=', num2str(gamma(t)));
    title(str);
    % Plot the SVM boundary
    colormap bone;
    contour(X, Y, vals, [0 0], 'LineWidth', 2);
end

% 随着gamma增大，模型对数据依赖增强，decision boundary可以较好分隔数据点
% 但gamma过大会出现过拟合现象

% 获取基于核函数Radial Basis Function计算的关系矩阵kmat
function kmat = get_kernel_mat(x, gamma)
    kmat = [];
    for i = 1 : length(x)
        for j = 1 : length(x)
            kmat(i, j) = exp(-gamma * norm(x(i, :) - x(j, :)) ^ 2);
        end
    end
end
