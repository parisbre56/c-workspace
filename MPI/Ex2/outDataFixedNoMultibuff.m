t=[;62.57119225263595581054;50.73709160089492797851;47.04979040622711181640;47.50232619047164916992;62.69035089015960693359];
w1=[;1.9;1.9;1.9;1.9;1.9];
w2=[;1.9;1.9;1.9;1.9;1.9];
q=[;sqrt(nthreads)/4;sqrt(nthreads)/2;sqrt(nthreads);sqrt(nthreads)*2;sqrt(nthreads)*4];
sqrtN=[;1000;1000;1000;1000;1000];
p=[;16;16;16;16;16];
%Data is in the form of w1:w2:q:sqrtN:procs
d=[;'1.9:1.9:sqrt(nthreads)/4:1000:16';'1.9:1.9:sqrt(nthreads)/2:1000:16';'1.9:1.9:sqrt(nthreads):1000:16';'1.9:1.9:sqrt(nthreads)*2:1000:16';'1.9:1.9:sqrt(nthreads)*4:1000:16'];
