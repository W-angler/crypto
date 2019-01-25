#include <cassert>
#include "RSA.h"

using namespace std;

int main() {
    {
        bigint p = 61;
        bigint q = 53;
        bigint n = p * q;
        bigint e = 17;
        bigint d = 2753;
        bigint m = 65;
        bigint c = 2790;

        RSA::PrivateKey privateKey(n, p, q, d, 0, 0, 0);
        RSA::PublicKey publicKey(n, e);

        auto cipher = RSA::encrypt(m, publicKey);
        auto plaintext = RSA::decrypt(cipher, privateKey);

        assert(cipher == c);
        assert(plaintext == m);
    }
    {
        cout << "generating RSA key pair..." << endl;
        auto keyPair = RSA::generate(1024);
        cout << "found one RSA key pair!" << endl;

        auto pub = keyPair.publicKey;
        auto pri = keyPair.privateKey;

        cout << "Public Key :" << endl;
        cout << "    n:" << pub.n << endl;
        cout << "    e:" << pub.e << endl;
        cout << "Private Key:" << endl;
        cout << "    n:" << pri.n << endl;
        cout << "    p:" << pri.p << endl;
        cout << "    q:" << pri.q << endl;
        cout << "    d:" << pri.d << endl;
        cout << "   dP:" << pri.dP << endl;
        cout << "   dQ:" << pri.dQ << endl;
        cout << " qInv:" << pri.qInv << endl;

        cout << endl;

        cout << "---encrypt&decrypt---" << endl;
        bigint message("12345678901234567890123456789012345678901234567890");
        cout << "message  : " << message << endl;
        auto cipher = RSA::encrypt(message, pub);
        cout << "cipher   : " << cipher << endl;
        auto plaintext = RSA::decrypt(cipher, pri);
        cout << "plaintext: " << plaintext << endl;
        assert(plaintext == message);
        cout << "correct? : " << ((plaintext == message) ? "true" : "false") << endl;
    }
    return 0;
}