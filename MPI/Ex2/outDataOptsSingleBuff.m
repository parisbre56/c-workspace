t=[;72.21519727706909179687;58.95505918264389038085;57.17850034236907958984;58.06821953058242797851;74.71384426355361938476];
w1=[;1.9;1.9;1.9;1.9;1.9];
w2=[;1.9;1.9;1.9;1.9;1.9];
q=[;sqrt(nthreads)/4;sqrt(nthreads)/2;sqrt(nthreads);sqrt(nthreads)*2;sqrt(nthreads)*4];
sqrtN=[;1000;1000;1000;1000;1000];
p=[;16;16;16;16;16];
%Data is in the form of w1:w2:q:sqrtN:procs
d=[;'1.9:1.9:sqrt(nthreads)/4:1000:16';'1.9:1.9:sqrt(nthreads)/2:1000:16';'1.9:1.9:sqrt(nthreads):1000:16';'1.9:1.9:sqrt(nthreads)*2:1000:16';'1.9:1.9:sqrt(nthreads)*4:1000:16'];
