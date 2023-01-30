
% to fix
function [alpha, bias] = SMO(Features, Labels, C, maxItertimes, tolerance)

% init
[sampleNum, featureNum] = size(Features);
alpha = zeros(sampleNum, 1);
bias = 0;
iteratorTimes = 0;

K = Features * Features';   % 计算K
while iteratorTimes < maxItertimes
    alphaPairsChanged = 0;  % 记录变化
    % find alpha1
    for i = 1 : sampleNum
        g1 = (alpha .* Labels)' * (Features * Features(i, :)') + bias;
        Error1 = g1 - Labels(i);
       % choose i: voilate KKT conditions
       if(((Error1 * Labels(i) < -tolerance) && alpha(i) < C) ||...
           ((Error1 * Labels(i) > tolerance) && alpha(i) > 0))
           % choose j: different from i 
           j = i;
           while j == i
                j = randi(sampleNum);   % 随机另外一个alpha2
           end

            % 尝试更新alpha1和alpha2
            alpha1New = alpha(i, 1);
            alpha2New = alpha(j, 1);
            y1 = Labels(i); y2 = Labels(j);

            g2 = (alpha .* Labels)' * (Features * Features(j, :)') + bias;
            Error2 = g2 - Labels(j);
            
            % 计算Lower bound和Higher bound
            if y1 ~= y2
                L = max(0, alpha2New - alpha1New);
                H = min(C, C + alpha2New - alpha1New);
            else
                L = max(0, alpha1New + alpha2New - C);
                H = min(C, alpha1New + alpha2New);
            end

            if L == H
                fprintf('L=H\n');
                continue;
            end

            para = K(i, i) + K(j, j) - 2 * K(i, j);

            if para <= 0
                fprintf('parameter\leq 0\n');
                continue;
            end
            % 得到新的alpha
            alpha2New = alpha_upd + y2 * (Error1 - Error2) / para;

            alpha2New = max(alpha2New, L);
            alpha2New = min(alpha2New, H);

            if abs(alpha2New-alpha_upd)<=0.0001
                fprintf('change small\n');
                continue;
            end

            alpha1New=alpha_upd+y1*y2*(alpha_upd-alpha2New);

            % updata bias
            bias1=-Error1-y1*K(i,i)*(alpha1New-alpha_upd)-y2*K(j,i)*(alpha2New-alpha_upd)+bias;
            bias2=-Error2-y1*K(i,j)*(alpha1New-alpha_upd)-y2*K(j,j)*(alpha2New-alpha_upd)+bias;

            if alpha1New>0&&alpha1New<C
                bias=bias1;
            elseif alpha2New>0&&alpha2New<C
                bias=bias2;
            else
                bias=(bias2+bias1)/2;
            end

            alpha(i,1)=alpha1New;
            alpha(j,1)=alpha2New;
            alphaPairsChanged=alphaPairsChanged+1;
       end  
    end

    if alphaPairsChanged == 0
        iteratorTimes = iteratorTimes + 1;
    else
        iteratorTimes = 0;
    end
    fprintf('iteratorTimes=%d\n', iteratorTimes);
end

