% I have created facts for the monkey to follow and store the path using traverse predicate
% I have defined states of the universe as state(monkey_position, banana_position, chair_position, monkey_state)
% where monkey_state can be the following
% start- represents starting position
% with_chair - represents monkey standing with  chair
% with_chair_below_banana - represents monkey standing with chair below banana
% on_chair_below_banana - represents monkey standing on th chair kept below banana
% on_chair_with_banana - represents final state with monkey standing on chair with banana

% represents final move for the monkey
path( state(A, A, A, on_chair_below_banana), grasp(A), state(A, A, A, on_chair_with_banana) ).

% when chair is below banana, the monkey mus get on chair
path( state(A, A, A, with_chair_below_banana),climb_chair(A), state(A, A, A, on_chair_below_banana) ).

% when monkey has chair, it must get the chair to banana's position
path( state(A, B, A, with_chair), push_chair(A, B), state(B, B, B, with_chair_below_banana) ).

% when monkey has started, he must get to the chair, since he needs to stand on it to get the banana
path(state(A, B, C, start), walk(A, C), state(C, B, C, with_chair) ).

% traverse is used to find the path to the banana, and store it in list
traverse( state(_, _, _, on_chair_with_banana), [] ).
traverse(State1, [M|List]) :- path(State1, M, State2), traverse(State2, List).

% predicate which will be called by the user
reach(X) :- traverse(state(a,b,c,start), X).