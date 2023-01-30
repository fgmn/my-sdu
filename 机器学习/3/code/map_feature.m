function out = map_feature(feat1, feat2)
% MAP_FEATURE    Feature mapping function for Exercise 4
%
%   map_feature(feat1, feat2) maps the two input features
%   to higher-order features as defined in Exercise 4.
%
%   Returns a new feature array with more features
%
%   Inputs feat1, feat2 must be the same size
%
% Note: this function is only valid for Ex 4, since the degree is
% hard-coded in.
    degree = 6;
    out = ones(size(feat1(:,1)));
    for i = 1:degree
        for j = 0:i
            out(:, end+1) = (feat1.^(i-j)).*(feat2.^j);
        end
    end
    