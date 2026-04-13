#include <stdio.h>
#include <math.h>

double f(double x) {
    return 4.0 / (1.0 + x * x); // Función
}

double simpson(double a, double b) { // Regla de Simpson
    double c = (a + b) / 2.0;
    return (b - a) / 6.0 * (f(a) + 4.0 * f(c) + f(b));
}

int main() {
    double a = 0.0;  // Límites
    double b = 1e9; // 1e9
    double pi_real = acos(-1.0);  // Valor real de π
        
    double resultado_simple = simpson(a, b);
    printf("Resultado: %.15f\n", resultado_simple);
    printf("Error: %.2e\n\n", fabs(resultado_simple - pi_real));

    return 0;
}
