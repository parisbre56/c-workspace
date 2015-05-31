t=[;42.09552941322326660156;41.11568460464477539062;38.98483543395996093750;39.72321705818176269531;42.58111495971679687500];
w1=[;1.9;1.9;1.9;1.9;1.9];
w2=[;1.9;1.9;1.9;1.9;1.9];
q=[;sqrt(nthreads)/4;sqrt(nthreads)/2;sqrt(nthreads);sqrt(nthreads)*2;sqrt(nthreads)*4];
sqrtN=[;1000;1000;1000;1000;1000];
p=[;16;16;16;16;16];
%Data is in the form of w1:w2:q:sqrtN:procs
d=[;'1.9:1.9:sqrt(nthreads)/4:1000:16';'1.9:1.9:sqrt(nthreads)/2:1000:16';'1.9:1.9:sqrt(nthreads):1000:16';'1.9:1.9:sqrt(nthreads)*2:1000:16';'1.9:1.9:sqrt(nthreads)*4:1000:16'];
