syms a b g m1 m2 m3 k;

% This script can be used to calculate IK equations for the gimbal.
% If you are using the provided arms or different arms with the same
% angles, you do NOT need to use this script as the equations are already in
% the firmware code. This script is provided if you need to change your
% gimbal geometry for fit your use case


% Gimbal arm angles
p1 = str2sym('60*pi/180');  % Lower arm angle
p2 = str2sym('75*pi/180');  % Upper arm angle
precision = 5;  % Rounding precision


pi_2 = str2sym('pi/2');

% Gimbal kinematics
% Yaw motor
R01 = [[cos(m1) -sin(m1) 0];
       [sin(m1)  cos(m1) 0];
       [   0        0    1]];

% Lower arm
R12 = [[1      0           0];
       [0 cos(-p1) -sin(-p1)];
       [0 sin(-p1)  cos(-p1)]];

% Pitch motor
R23 = [[cos(m2) -sin(m2) 0];
       [sin(m2)  cos(m2) 0];
       [  0        0     1]];

% Upper arm
R34 = [[cos(-p2)  0  sin(-p2)];
       [0         1         0];
       [-sin(-p2) 0  cos(-p2)]];

% Roll motor
R45 = [[cos(m3) -sin(m3) 0];
       [sin(m3)  cos(m3) 0];
       [    0       0    1]];

% Align handle and head orientations
R56 = [[cos(pi_2)  0 sin(pi_2)];
       [    0      1         0];
       [-sin(pi_2) 0 cos(pi_2)]];


% Rotation matrix
Rx = [1   0       0;
      0 cos(g) -sin(g);
      0 sin(g)  cos(g)];
Ry = [cos(b) 0 sin(b);
         0   1    0  ;
     -sin(b) 0 cos(b)];
Rz = [cos(a) -sin(a) 0;
      sin(a)  cos(a) 0
        0       0    1];

% Calculate and equate matrices
fprintf("Calculating angles...\n");
Rk = R01 * R12 * R23 * R34 * R45 * R56;
Rt = Rz * Ry * Rx;
eq = Rk == Rt;

% Solve the resulting equations
fprintf("Solving equations...\n");
sol2 = solve(eq(3, 1), m2, 'ReturnConditions', true);
sol2sub = subs(sol2.m2, k, 0);
fprintf("\nSolutions for motor 2:\n");
disp(sol2sub);
vsol2 = vpa(sol2sub, precision);
fprintf("Calculated:\n");
disp(vsol2);

sol1 = solve([eq(1, 1); eq(2, 1)], m1, 'Real', true, 'ReturnConditions', true, 'IgnoreAnalyticConstraints', true);
sol1sub = subs(subs(sol1.m1, k, 0), m2, sol2sub(1, 1));
fprintf("\nSolutions for motor 1:\n");
disp(sol1sub);
vsol1 = vpa(sol1sub, precision);
fprintf("Calculated:\n");
disp(vsol1);

sol3 = solve([eq(3,2); eq(3, 3)], m3, 'Real', true, 'ReturnConditions', true, 'IgnoreAnalyticConstraints', true);
sol3sub = subs(subs(sol3.m3, k, 0), m2, sol2sub(1, 1));
fprintf("\nSolutions for motor 3:\n");
disp(sol3sub);
vsol3 = vpa(sol3sub, precision);
fprintf("Calculated:\n");
disp(vsol3);

% Visualize the equation results
fprintf("Preparing visualization...\n");
figure
subplot(1, 3, 1)
draw(piecewise(a <= 0, vsol1(1, 1), vsol1(2, 1)), {a, b, g})
fprintf("Motor 1 visualization done\n");
subplot(1, 3, 2)
draw(vsol2(1, 1), {a, b, g})
fprintf("Motor 2 visualization done\n");
subplot(1, 3, 3)
draw(piecewise(abs(g) >= pi_2, vsol3(1, 1), vsol3(2, 1)), {a, b, g})
fprintf("Motor 3 visualization done\n");


function draw(fn, params)
    [A, B, G] = meshgrid(linspace(-pi, pi, 20), linspace(-pi/4 + 1e-5, pi/4 - 1e-5, 20), linspace(-pi, pi, 20)); % Generating points

    F = real(double(subs(fn, params, {A, B, G}))); % Calculating function value at each point
    % Normalizing function to [0, 1]
    Fn = F - min(F(:)); % Subtracting the minimum value
    Fn = Fn ./ max(Fn(:)); % Dividing by the new maximum
    colors = interp1(linspace(0, 1, size(colormap('jet'), 2)), 0:size(colormap('jet'), 2) - 1, Fn, 'linear'); % Assigning colors from color scheme

    scatter3(reshape(A, [], 1), reshape(B, [], 1), reshape(G, [], 1), 50, reshape(colors, [], 1), 'filled'); % Plotting points (scatter3 only accepts 1D arrays, thus resizing)
end
