
clearvars
close

% todo
% 1. 使用更小的训练集（不重复采样）
% 2. 用混淆矩阵进行模型的评价

% 数据集：前8个值为特征，第9个值为分类

tr_data = load('training_data.txt');
tr_data = tr_data + 1;  % 将取值调整为从1开始
[m n] = size(tr_data);

% 训练集大小
% tr_size = [10 : 10 : 90, 100 : 100 : 1000, 2000 : 1000 : 10000];
% acc_avg = zeros(length(tr_size), 1);

% 加载测试集
test_data = load('test_data.txt');
test_data = test_data + 1;
cnt = length(test_data);

% for t = 1 : length(tr_size)
%     for v = 1 : 10  % 10次不同采样求平均正确率
        % 采样
        rp = randperm(m);
%         samp = rp(1 : tr_size(t));
        samp = rp;
        X = tr_data(samp, 1 : n - 1);  % 特征
        Y = tr_data(samp, n);          % 分类

        % maxY = max(Y);
        % maxX = max(X);
        % 根据数据集定义指定
        maxY = 5;
        maxX = [3, 5, 4, 4, 3, 2, 3, 3];

        py = zeros(maxY, 1);  % p(y)
        pxy = zeros(maxY, n - 1, max(maxX));    % p_j(x|y)

        % 根据MLE对应的最优化参数，计算先验概率
        for i = 1 : maxY
            Yi = find(Y == i);
            % Laplace Smoothing: 防止0/0错误，并保证sum(py(i))=1
            py(i) = (length(Yi) + 1) / (m + maxY);
            Xi = X(Yi, :);  % 选择Y=yi的样本
            for j = 1 : n - 1   % 逐个特征进行统计
                for k = 1 : maxX(j)
                    pxy(i, j, k) = (length(find(Xi(:, j) == k)) + 1) / (length(Yi) + maxX(j));
                end
            end
        end

        % 预测向量，用于计算混淆矩阵
        prec_vec = zeros(cnt, 1);
        
        % 测试，验证模型效果
        cnt_corr = 0;
        for i = 1 : cnt
            p_max = 0; c_predict = 0;
            for c = 1 : maxY    % 分类为c
                p = py(c);
                for j = 1 : n - 1
                    xj = int8(test_data(i, j)); % 第j个特征的取值
                    p = p * pxy(c, j, xj);
                end
                if p > p_max
                    p_max = p;
                    c_predict = c;
                end
            end
        %     fprintf("预测分类为%d，实际分类为%d\n", c_predict, test_data(i, n));
            prec_vec(i) = c_predict;
            if c_predict == test_data(i, n)
                cnt_corr = cnt_corr + 1;
            end
        end
        acc = cnt_corr / cnt;
        fprintf("测试集分类的正确率为%f\n", acc);
%         acc_avg(t) = acc_avg(t) + acc;
        % 0.903041
%     end
% end
% acc_avg = acc_avg / v;

% figure
% plot(tr_size, acc_avg);
% title('Naive Bayes model')
% xlabel('the size of training set')
% ylabel('10 times average correct rate(%)')

% 在训练集小于1000时，准确率随着训练集增大快速增加，同时增长放缓；
% 在训练集大于1000后，准确率几乎稳定，不会随训练集增大而变化

confusion_matrix(test_data(:, n), prec_vec);
