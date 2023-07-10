

clearvars
close

tr_data = load('training_2.txt');
test_data = load('test_2.txt');

pos = find(tr_data(:, 3) > 0);
neg = find(tr_data(:, 3) < 0);

x = [tr_data(:, 1), tr_data(:, 2)];
y = tr_data(:, 3);
m = length(x);

C = 1e-6;  % 正则项参数->松弛变量
% C = 1;

for t = 1 : 6
    C = C * 10;

    % SVM
    % 构建目标函数
    H = zeros(m);
    for i = 1 : m
        for j = 1 : m
            H(i, j) = y(i) * y (j) * x(i, :) * x(j, :)';
        end
    end
    % H = (y .* x) * (y .* x)';
    % H = (H + H') / 2;
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

    % plot
%     figure
    subplot(2, 3, t)
    hold on
    plot(tr_data(pos, 1), tr_data(pos, 2), '.r');
    plot(tr_data(neg, 1), tr_data(neg, 2), '.b');
    % 1. decision boundary(y=kx+c)
    k = -w(1) / w(2);
    c = -b / w(2);
    x_plot = 30 : 0.1 : 170;
    plot(x_plot, k * x_plot + c, '-', 'LineWidth', 2)
    plot(x_plot, k * x_plot + c - 1 / w(2), '--', 'LineWidth', 2)
    plot(x_plot, k * x_plot + c + 1 / w(2), '--', 'LineWidth', 2)
    % legend('pos', 'neg', 'w^T+b=0', 'w^T+b=-1', 'w^T+b=1');
    xlabel('x_1');
    ylabel('x_2');
%     legend('pos', 'neg', 'decision boundary');

    %  2. clac the fraction of test examples which were misclassified
    miss_cnt = 0;
    for i = 1 : length(test_data)
       tmp = b + test_data(i, 1 : 2) * w;
       if tmp * test_data(i, 3) < 0
           miss_cnt = miss_cnt + 1;
       end
    end
    miss_rate = miss_cnt / length(test_data);
%     fprintf("error rate: %f\n", miss_rate);
    str = strcat('linear SVM(C=', num2str(C), ', err rate:', num2str(miss_rate), ')');
    title(str);
end

% 3.Try different values of the regularization term C
% C增大，间隔减小；C减小，间隔增大

% 2*3plot展示，(正则项参数)松弛变量C对支持向量以及错误率的影响



