
% SVD-PCA数据降维
% input: D×N output:K×N
function [Z] = SVD_PCA(X, K)
    fprintf('Running SVD-PCA dimensionality reduction...\n');
    [D, N] = size(X);    % D:feature dimension
    %% Step 1: Center the data
    mu  = mean(X, 2);
    X = X - mu * ones(1, N);

    %% Step 2: Compute the A^{T}A
    Mat = X' * X;
    %% Step 3: Do an eigendecomposition of A^{T}A
    % 利用左奇异值矩阵U进行特征维度压缩，即减少X的行数
    [V, S] = eig(Mat);  % N×N
    %% Step 4: Take first K leading eigenvector of A^{T}A then build 
    %% Left single matrix U
    S = diag(S);
    [S, si] = sort(S, 'descend');

    eigenVec = zeros(N, K);
    eigenVal = zeros(1, K);
    for i = 1 : K
        eigenVec(:, i) = V(:, si(i));
        eigenVal(i) = S(i);
    end
    
    rU = zeros(D, K);   % reconstructed matrix U
    for i = 1 : K
        rU(:, i) = X * eigenVec(:, i) / sqrt(eigenVal(i));    % 奇异值≈sqrt(特征值)
    end
%     save('svdData', 'eigenVec', 'eigenVal', 'rU');
    
    %% Step 5: Calc the final K dim. projection of data
    Z = rU' * X; % (K×N)=(K×D)*(D×N)
    
    fprintf('SVD-PCA done\n');
end

