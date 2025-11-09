%% === COMPENSADOR DIGITAL ===
s = tf('s');
wp = 20.95;
wc = wp/10;
Ts = 1e-3;

% Filtros y compensadores continuos
LPF = 1/(1 + s/wp);
LPF2 = 2/(1 + s/wp);
K = (s^2 * (1 + s/wp)) / (1 + s/wc);
K_comp = abs(evalfr(K, 1j*wp));
Gc = K_comp * (1 + s/wc) / s;
int = 1/s;

GH = Gc * int * LPF;

% Discretización
GH_z    = c2d(GH, Ts);
Gc_z    = c2d(Gc, Ts);
Gc_z_b  = c2d(Gc, Ts, 'tustin');
LPF_z   = c2d(LPF, Ts);
LPF_z_b = c2d(LPF, Ts, 'tustin');
LPF2_z  = c2d(LPF2, Ts);
LPF2_z_b= c2d(LPF2, Ts, 'tustin');

%% === CÁLCULO DE COEFICIENTES Y ECUACIONES EN DIFERENCIAS ===
tf_list = {'GH_z','Gc_z','Gc_z_b','LPF_z','LPF_z_b','LPF2_z','LPF2_z_b'};

for k = 1:length(tf_list)
    name = tf_list{k};
    H = eval(name);
    [b, a] = tfdata(H, 'v');
    
    % Normalización (a(1) = 1)
    b = b ./ a(length(a));
    a = a ./ a(length(a));

    fprintf('\n=== %s ===\n', name);
    fprintf('b = ['); fprintf(' %.6g', b); fprintf(' ]\n');
    fprintf('a = ['); fprintf(' %.6g', a); fprintf(' ]\n');

    % Construcción de la ecuación en diferencias
    fprintf('Ecuacion en diferencias:\n');
    fprintf('y[n] =');

    idx_a=1;
    idx_b=1;
    for i = 1:1:length(a)-1
        fprintf(' - (%.6g)*y[n-%d]', a(i), i);
        idx_a = idx_a+1;
    end
    for j = length(b):-1:1
        fprintf(' + (%.6g)*x[n-%d]', b(j), idx_b-1);
        idx_b = idx_b+1;
    end
    fprintf('\n');
end



