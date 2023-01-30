

% Attribute values:
%    parents        usual:0, pretentious:1, great_pret:2
%    has_nurs       proper:0, less_proper:1, improper:2, critical:3, very_crit:4
%    form           complete:0, completed:1, incomplete:2, foster:3
%    children       1:0, 2:1, 3:2, more:3
%    housing        convenient:0, less_conv:1, critical:2
%    finance        convenient:0, inconv:1
%    social         non-prob:0, slightly_prob:1, problematic:2
%    health         recommended:0, priority:1, not_recom:2


% Class:
% not_recom:0
% recommend:1
% very_recom:2
% priority: 3
% spec_prior: 4


fid = fopen('nursery.data.txt','rt');
data_num = 12960;
att_num = 9;
data = zeros(data_num, att_num);
it = 0;
%label = zeros(ROW, 1);


while feof(fid) ~=1
    str = fgetl(fid);
    c = strsplit(str,',');
    it = it+1;
    
    %parents    usual:0, pretentious:1, great_pret:2
    parents = char(c(1));
    switch(parents)
        case 'usual'
            data(it,1) = 0;
        case 'pretentious'
            data(it,1) = 1;
        case 'great_pret'
            data(it,1) = 2;
        otherwise
            disp('Errors in PARENTS\n');
    end
    
    
     %has_nurs	proper:0, less_proper:1, improper:2, critical:3, very_crit:4
     has_nurs = char(c(2));
     switch(has_nurs)
        case 'proper'
            data(it,2) = 0;
        case 'less_proper'
            data(it,2) = 1;
        case 'improper'
            data(it,2) = 2;
        case 'critical'
            data(it,2) = 3;
        case 'very_crit'
            data(it,2) = 4;
        otherwise
            disp('Errors in HAS_NURS\n');
     end
    
    
     
     %form	complete:0, completed:1, incomplete:2, foster:3
     form = char(c(3));
     switch(form)
        case 'complete'
            data(it,3) = 0;
        case 'completed'
            data(it,3) = 1;
        case 'incomplete'
            data(it,3) = 2;
        case 'foster'
            data(it,3) = 3;
        otherwise
            disp('Errors in FORM\n');
     end
      
     
     %children 1:0, 2:1, 3:2, more:3
     children = char(c(4));
     switch(children)
        case '1'
            data(it,4) = 0;
        case '2'
            data(it,4) = 1;
        case '3'
            data(it,4) = 2;
        case 'more'
            data(it,4) = 3;
        otherwise
            disp('Errors in CHILDREN\n');
     end
    
     
     %housing convenient:0, less_conv:1, critical:2
     housing = char(c(5));
     switch(housing)
        case 'convenient'
            data(it,5) = 0;
        case 'less_conv'
            data(it,5) = 1;
        case 'critical'
            data(it,5) = 2;
        otherwise
            disp('Errors in HOUSING\n');
     end
    
     
     %finance convenient:0, inconv:1
     finance = char(c(6));
     switch(finance)
        case 'convenient'
            data(it,6) = 0;
        case 'inconv'
            data(it,6) = 1;
        otherwise
            disp('Errors in FINANCE\n');
     end
    
     %social nonprob:0, slightly_prob:1, problematic:2
     social = char(c(7));
     switch(social)
        case 'nonprob'
            data(it,7) = 0;
        case 'slightly_prob'
            data(it,7) = 1;
        case 'problematic'
            data(it,7) = 2;
        otherwise
            disp('Errors in SOCIAL\n');
     end
    
     
     %health recommended:0, priority:1, not_recom:2
     health = char(c(8));
     switch(health)
        case 'recommended'
            data(it,8) = 0;
        case 'priority'
            data(it,8) = 1;
        case 'not_recom'
            data(it,8) = 2;
        otherwise
            disp('Errors in HEALTH\n');
     end
     
     %Class	not_recom:0; recommend:1; very_recom:2; priority: 3; spec_prior: 4
     class = char(c(9));
     switch(class)
        case 'not_recom'
            data(it,9) = 0;
        case 'recommend'
            data(it,9) = 1;
        case 'very_recom'
            data(it,9) = 2;
        case 'priority'
            data(it,9) = 3;
        case 'spec_prior'
            data(it,9) = 4;
        otherwise
            disp('Errors in CLASS\n');
     end
     
end

fclose(fid);

% The number of training data
training_num = 10000;

% The number of test data
test_num = data_num - training_num;

test_data_index = randperm(data_num, test_num);

test_data = zeros(test_num, 9);

training_data = zeros(training_num, 9);


test_it = 0;
training_it = 0;


for i=1:1:data_num
    flag = 0;
    
    for j=1:1:test_num
        if(test_data_index(j)==i)
            flag = 1;
        end
    end
    
    if(flag==1)
        test_it = test_it+1;
        test_data(test_it,:) = data(i,:);
    else
        training_it = training_it+1;
        training_data(training_it,:) = data(i,:);
    end
end


num0=0;
num1=0;
num2=0;
num3=0;
num4=0;

for i=1:1:2960
    tmp = training_data(i,9);
    switch(tmp)
        case 0
            num0 = num0+1;
        case 1
            num1 = num1+1;
        case 2
            num2 = num2+1;
        case 3
            num3 = num3+1;
        case 4
            num4 = num4+1;
        otherwise
            disp('Error!\n');
    end
end





fid = fopen('training_data.txt','wt');
for i=1:1:training_num
    fprintf(fid,'%d %d %d %d %d %d %d %d %d\n',training_data(i,1), training_data(i,2), training_data(i,3), training_data(i,4), training_data(i,5), training_data(i,6), training_data(i,7), training_data(i,8), training_data(i,9));
end
fclose(fid);

fid = fopen('test_data.txt','wt');
for i=1:1:test_num
    fprintf(fid,'%d %d %d %d %d %d %d %d %d\n',test_data(i,1), test_data(i,2), test_data(i,3), test_data(i,4), test_data(i,5), test_data(i,6), test_data(i,7), test_data(i,8), test_data(i,9));
end
fclose(fid);



