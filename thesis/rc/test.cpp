#include "/usr/local/dislin/Lorenz.h"

int main () {
    int N_max = 102;
    int steps = 2000;
    Vector esn_start(N_max);
    esn_start.RandomReals();
    Vector a (3);
    a[0] = a[1] = a[2] = 1;
    DiscreteTimeSeries* data = new LorenzApprox(a, steps);
    EchoStateNetwork esn(esn_start, data, steps);
    esn.Tune();
    return 0;
}    


