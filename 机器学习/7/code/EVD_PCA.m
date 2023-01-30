
% EVD-PCA数据降维
% input: D×N output:K×N
function [Z, K] = EVD_PCA(X, K, weight)
    fprintf('Running EVD-PCA dimensionality reduction...\n');
    if exist('eigenData.mat', 'file') == 0
        [~, N] = size(X);
        %% Step 1: Center the data
%         mu  = mean(X);
%         X = X - mu;   !err
        mu  = mean(X, 2);
        X = X - mu * ones(1, N);
        %% Step 2: Compute the covariance matrix
        S = X * X' / N; % D×D
%         size(S)
        %% Step 3: Do an eigendecomposition of S
        [V, D] = eig(S);    % !time-consuming
        % S*V=V*D，其中D为特征值的对角矩阵，V对应列为特征向量
        % (D×D)*(D×E)=(D×E)*(E×E)，其中E为特征值个数，D为原数据维度（区分对角矩阵D）
        
        %% Step 4: Take first K leading eigenvector
        eigenVal = diag(D); % 特征值序列
        [~, sortedIndex] = sort(eigenVal, 'descend');
        eigenVec = V(:, sortedIndex);   % 对应特征向量构筑矩阵
        eigenVal = eigenVal(sortedIndex);
        save('eigenData.mat', 'eigenVec', 'eigenVal');
    else 
        load('eigenData.mat');
    end
    %% 检查是否传入有效K，否则基于weight动态定义K
    % 前K个特征值之和占特征值之和的比例达到weight
    if K < 0
       sumVal = sum(eigenVal);
       for i = 1 : length(eigenVal)
           newRate = sum(eigenVal(1 : i), 1) / sumVal;
           if newRate >= weight
               K = i; break;
           end
       end
       fprintf('Dynamically define K to %d\n', K);
    end
    U = eigenVec(:, 1 : K); % (D×K)
    
    %% Step 5: Calc the final K dim. projection of data
    Z = U' * X; % (K×N)=(K×D)*(D×N)
    
    fprintf('EVD-PCA done\n');
end

