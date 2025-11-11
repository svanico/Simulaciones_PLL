clear;
%% === COMPENSADOR DIGITAL ===
s = tf('s');
wp = 100*2*pi/30;% 20.95
wc = wp/10;
Ts = 1e-3;

% Filtros y compensadores continuos
LPF = 1/(1 + s/wp);
LPF2 = 2/(1 + s/wp);
K = (s^2 * (1 + s/wp)) / (1 + s/wc);
K_comp = abs(evalfr(K, 1j*wp));

Gc = K_comp * (1 + s/wc) / s;
int = 1/s;

fprintf('\nK_comp = %.12f\n', K_comp);
 
GH = Gc * int * LPF;

% Discretización
GH_z    = c2d(GH, Ts);
Gc_z    = c2d(Gc, Ts);
Gc_z_b  = c2d(Gc, Ts, 'tustin');
LPF_z   = c2d(LPF, Ts);
LPF_z_b = c2d(LPF, Ts, 'tustin');
LPF2_z  = c2d(LPF2, Ts);
LPF2_z_b= c2d(LPF2, Ts, 'tustin');

fprintf('\n=== DISCRETIZACIÓN ===\n');
fprintf('\nGH_z:\n');    GH_z
fprintf('\nGc_z:\n');    Gc_z
fprintf('\nGc_z_b (Tustin):\n');  Gc_z_b
fprintf('\nLPF_z:\n');   LPF_z
fprintf('\nLPF_z_b (Tustin):\n'); LPF_z_b
fprintf('\nLPF2_z:\n');  LPF2_z
fprintf('\nLPF2_z_b (Tustin):\n'); LPF2_z_b

systems = {GH_z, Gc_z, Gc_z_b, LPF_z, LPF_z_b, LPF2_z, LPF2_z_b};
names   = {'GH_z', 'Gc_z', 'Gc_z_b', 'LPF_z', 'LPF_z_b', 'LPF2_z', 'LPF2_z_b'};

for k = 1:length(systems)
    [num, den] = tfdata(systems{k}, 'v'); % obtiene vectores num y den
    fprintf('\n=== %s ===\n', names{k});
    fprintf('Num = '); disp(num);
    fprintf('Den = '); disp(den);

    % Genera ecuación en diferencias
    fprintf('Ecuación en diferencias:\n');
    fprintf('y[n] =');

    % Términos de y[n-k]
    for i = 2:length(den)
        fprintf(' - (%.6f)*y[n-%d]', den(i), i-1);
    end

    % Términos de x[n-k]
    for i = 1:length(num)
        if i == 1
            fprintf(' + (%.6f)*x[n]', num(i));
        else
            fprintf(' + (%.6f)*x[n-%d]', num(i), i-1);
        end
    end
    fprintf('\n');
end



