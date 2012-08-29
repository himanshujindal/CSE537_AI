% I have defined 1 fact about each figure which tells which object is inside which object
% I have defined 4 facts which relate figures
% There is one predicate analogy which will find the analogy between each figures

% facts about figures
figure(1, middle(triangle, square)).
figure(2, middle(circle, triangle)).
figure(3, middle(square, circle)).
figure(4, middle(square, square)).
figure(5, middle(square, triangle)).
figure(6, middle(triangle, circle)).
figure(7, middle(circle, square)).
figure(8, middle(triangle, triangle)).

% facts about relations between figures
relate(middle(A,B), middle(B,A), invert).
relate(middle(A,A), middle(B,B), out_in_same).
relate(middle(A,B), middle(A,B), same).
relate(middle(A,_),middle(A,_), same_middle).
relate(middle(_,A),middle(_,A), same_outer).

% analogy predicate
analogy(A, B, C, D) :- figure(A, X), figure(B, Y), relate(X, Y, Z), figure(C, M), relate(M, N, Z), figure(D, N), write(Z).