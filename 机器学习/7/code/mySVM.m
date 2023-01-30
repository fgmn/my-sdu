
function SVMObj = mySVM(x, y, C)
    m = size(x, 1);
    % !err：length=max(size(X))，返回数组最大维度长度
    
    options = optimset;
    options.largeScale = 'off';
    options.Display = 'off';
    
    %% A. 构建目标函数
    H = zeros(m);
    for i = 1 : m
        for j = 1 : m
            H(i, j) = y(i) * y (j) * x(i, :) * x(j, :)';
        end
    end
    f = (-1) * ones(m, 1);
    
    %% B. 构建约束
    Aeq = y';
    beq = 0;
    lb = zeros(m, 1);
    ub = zeros(m, 1);
    ub(:) = C;
    a0 = zeros(m, 1);   % 迭代初始值
    
    %% C. 利用quadprog求解器求解对偶问题
    % quadprog(H,f,A,b,Aeq,beq,lb,ub)
    [alpha, fval] = quadprog(H, f, [], [], Aeq, beq, lb, ub, a0, options);
    
    %% D. 求support vector
    alpha(find(alpha < 1e-8)) = 0;
    sv = find(alpha > 0 & alpha < C);
    w = 0;  % omega
    for i = 1 : length(sv)
        w = w + alpha(sv(i)) * y(sv(i)) * x (sv(i), :)';
    end
    num = y - x * w;
    b = sum(num(sv)) / length(sv);
    
    %% 构建返回对象SVMObj
    SVMObj.alpha = alpha;   % alpha(sv)
    SVMObj.w = w;
    SVMObj.b = b;
end
