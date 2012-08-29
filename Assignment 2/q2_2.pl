% I have defined 1 fact about each figure which tells which object is inside which object
% I have defined 4 facts which relate figures
% There is one predicate analogy which will find the analogy between each figures

% facts about figures
figure(1, middle(circle, center)).
figure(2, middle(circle, top_left)).
figure(3, middle(circle, bottom_right)).
figure(4, middle(square, center)).
figure(5, middle(square, top_left)).
figure(6, middle(square, top_right)).
figure(7, middle(square, bottom_right)).
figure(8, middle(square, bottom_left)).

% facts about relations between figures
relate(middle(A,B), middle(A,C), same_building_block).
relate(middle(B,A), middle(C,A), same_orientation).

% analogy predicate
analogy(A, B, C, D) :- figure(A, X), figure(B, Y), relate(X, Y, Z), figure(C, M), relate(M, N, Z), figure(D, N), write(Z).