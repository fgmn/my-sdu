
clearvars
close all

A = double(imread('bird_small.tiff'));
figure
subplot(1, 2, 1);
imshow(uint8(round(A)))
title('original')

[m, n, color] = size(A);
k = 16;     % 簇数
center = zeros(k, 3);

% 随机采样图像中16个pixel作为初始中心点
for i = 1 : k
    rp = randperm(m, 2);
    center(i, :) = A(rp(1), rp(2), :);
end

% 迭代
maxiter = 60;
label = zeros(m, n);
% K-means loss function
Loss = zeros(1, maxiter);
for i = 1 : maxiter
    for x = 1 : m
        for y = 1 : n   % 遍历所有pixel
            min_dis = 1e10;
            for j = 1 : k   % calculate its nearest mean
                dis = [A(x, y, 1) - center(j, 1), A(x, y, 2) - center(j, 2), A(x, y, 3) - center(j, 3)];
                % A(x, y, :): 1 * 1 * 3
                % center(k, :): 1 * 3
                dis = dis * dis';
                if min_dis > dis
                    min_dis = dis;
                    label(x, y) = j;
                end
            end
            Loss(i) = Loss(i) + min_dis;
        end
    end
    
    % Update the values of the means
    cluster_size = zeros(k, 1);
    new_center = zeros(k, 3);
    for x = 1 : m
        for y = 1 : n
            lb = label(x, y);
            cluster_size(lb) = cluster_size(lb) + 1;
            new_center(lb, :) = new_center(lb, :) + [A(x, y, 1), A(x, y, 2), A(x, y, 3)];
        end
    end
    for j = 1 : k
        if cluster_size(k) ~= 0
            center(k, :) = new_center(k, :) / cluster_size(k);
        end
    end
end

% Reassigning Colors to The Small Image
for x = 1 : m
    for y = 1 : n
        lb = label(x, y);
        A(x, y, :) = [center(lb, 1), center(lb, 2), center(lb, 3)];
    end
end

subplot(1, 2, 2);
imshow(uint8(round(A)))
title('cluster into 16 RGB means')

% plot loss function
figure
plot([1 : maxiter], Loss, 'LineWidth', 2)
xlabel('iteration')
ylabel('loss')
title('K-means loss function')

% Reassigning Colors to The Large Image
B = double(imread('bird_large.tiff'));
% Display
% figure
% subplot(1, 2, 1)
% imshow (uint8(round(B)))
% title('original')
[M, N, ~] = size(B);
for x = 1 : M
    for y = 1 : N
        min_dis = 1e18; lb = -1;
        for j = 1 : k
            dis = [B(x, y, 1) - center(j, 1), B(x, y, 2) - center(j, 2), B(x, y, 3) - center(j, 3)];
            dis = dis * dis';
            if min_dis > dis
                min_dis = dis;
                lb = j;
            end
        end
        B(x, y, :) = [center(lb, 1), center(lb, 2), center(lb, 3)];
    end
end
% subplot(1, 2, 2)
% imshow(uint8(round(B)))
% title('cluster into 16 RGB means')
% imwrite(uint8(round(B)), 'bird_kmeans.tiff');

% ColorBar = zeros(20 * k, 20, 3);
% for i = 1 : k
%     for x = i * 20 : (i + 1) * 20
%         for y = 1 : 20
%             ColorBar(x, y, :) = [center(i, 1), center(i, 2), center(i, 3)];
%         end
%     end
% end
% 
% figure
% imshow(uint8(round(ColorBar)))
% imwrite(uint8(round(A)), ['bird_', num2str(k), 'means.tiff']);




    