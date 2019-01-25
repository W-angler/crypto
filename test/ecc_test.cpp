#include <cassert>
#include "ecc.h"

using namespace std;

int main() {
    {
        ECCurve curve(19, 1, 1, ECPoint(9, 6));
        cout << curve.add(ECPoint(10, 2), ECPoint(9, 6)) << endl;//(16,3)
        cout << curve.add(ECPoint(10, 2), ECPoint(10, 2)) << endl;//(15,16)
    }
    {
        ECPoint G = ECPoint(0, 1);
        ECCurve curve(23, 1, 1, ECPoint(0, 1));
        for (int i = 1;; i++) {
            ECPoint p = curve.multiply(G, bigint(i));
            if (p == ECPoint()) {
                break;
            }
            cout << i << ": " << p << endl;
        }
    }
    return 0;
}