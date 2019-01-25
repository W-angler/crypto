#include <iostream>
#include <cassert>
#include <ctime>
#include "bigint.h"

using namespace std;

int main(int argc, char **argv) {
    {//转换
        bigint a = 65537;
        bigint b = bigint("65537");
        assert(a == b);
    }
    {//转换
        bigint a = -1234567887654321;
        bigint b = bigint("-1234567887654321");
        assert(a == b);
    }
    {//转换
        bigint a = 0x1234567887654321;
        bigint b = bigint::fromHex("1234567887654321");
        assert(a == b);
    }
    {//基本运算
        bigint a = 65537;
        bigint b = 2;
        assert(a.bitLength() == 17);
        assert(b.bitLength() == 2);
        assert(a.testBit(0));
        assert(a.testBit(16));
        assert(!a.testBit(8));
        assert(b.testBit(1));

        bigint c = 1234567887654321LL;
        assert((c + b) == bigint(1234567887654323));
        assert((c - b) == bigint(1234567887654319));
        assert((-c) == bigint(-1234567887654321));
        assert((-b) == bigint(-2));

        assert((-c).abs() == bigint(1234567887654321LL));
        assert((-b).abs() == bigint(2));

        assert((a >> 1) == bigint(32768));
        assert((a << 1) == bigint(131074));
        assert((b >> 1) == bigint(1));
        assert((b << 1) == bigint(4));
        assert(!(a < b));
        assert(-a < -b);
        assert(!(a <= (-a)));
        assert((-a) <= (-a));

        assert((a * b) == bigint(131074));
        assert((a * -b) == bigint(-131074));
        assert((-a * b) == bigint(-131074));
        assert((-a * -b) == bigint(131074));

        assert((a / b) == bigint(32768));
        assert((-a / b) == bigint(-32768));
        assert((a / -b) == bigint(-32768));
        assert((-a / -b) == bigint(32768));

        assert((a % b) == bigint(1));
        assert((-a % b) == bigint(1));
        assert((a % -b) == bigint(-1));
        assert((-a % -b) == bigint(-1));
    }
    {//GCD、互质
        bigint p1("3347807169895689878604416984821269081770479498371376856891"
                  "2431388982883793878002287614711652531743087737814467999489");
        bigint p2("3674604366679959042824463379962795263227915816434308764267"
                  "6032283815739666511279233373417143396810270092798736308917");
        assert(p1.gcd(p2) == bigint::ONE);
        assert(p1.coprime(p2));
    }
    {//扩展欧几里得
        bigint a = 47;
        bigint b = 30;
        bigint gcd = bigint::ONE;
        bigint x = -7;
        bigint y = 11;
        auto result = a.xgcd(b);
        assert(result[0] == gcd);
        assert(result[1] == x);
        assert(result[2] == y);
    }
    {//幂
        bigint a = 2;
        unsigned long long b = 2333;
        bigint std(
                "20089997563073892894610983580353126456347072215343367141256554853768525956181600656105955062958697012"
                "11680753538484413396037243754314876180954595354096587978436986423090374036253880043145036243115531091"
                "17987430151716767882613843014592316410121901377715366831858992404632261011813434394298601266972443962"
                "76475940195732128102930573531726754637537218440837622767703269612623569770564253639451448645348169948"
                "84533074138027541987159480450643594180650556576544267327962666376615321641372238481361710024365486287"
                "06046538735590665784161574502552261305498115470021957178438275981014902941038979190866503215545770037"
                "2923786347743262701766765721681939535916544748387326210333531915969732358554013679547784041070592"
        );
        assert(a.pow(b) == std);
    }
    {//模幂
        bigint a = 2;
        bigint b = 665778;
        bigint n = 555536655;
        bigint std = 267898564;
        assert(a.modPow(b, n) == std);
    }
    {//模逆
        bigint a = 2333;
        bigint m = 65537;
        bigint result = 45845;
        assert(a.modInverse(m) == result);
    }
    {
        bigint a(
                "1336062739864631492057320992911969434636306180484327060611945781842750471146354491097766933894506464941"
                "8673088939015009775702826562508545339349803687833632334415249051716551419040421193567034052639307596235"
                "5785385326689093018201540230180280182592052468519891501418639510811114181203620022259882324361657147620");
        bigint b(
                "5597892703054265953753039697394057843989344597182786319945928880020869805483032374319290123353529868621"
                "6909848223925325485602478067194848498442912274261498401334225969012608438119090175710775225784044338404"
                "756839600582703735982202191094364420938381422574798162362364812695636961333492709937679764120951950480");
        bigint n(
                "1667217898245701827649411206841234025208802451854650293997744953791993715648950615408057106341297935912"
                "2616726593768592077967061810718630742414046515856570724259974726618044556268791524394267722868038055763"
                "4484901202922176654700160498452269014666559059922771944342443761217939179131042466671679654930298834137");

        bigint a_add_b(
                "1895852010170058087432624962651375219035240640202605692606538669844837451694657728529695946229859451804"
                "0364073761407542324263074369228030189194094915259782174548671648617812262852330211138111575217712030076"
                "0542224927271796754183742421274644603530433891094689663781004323506751142537112732197562088482609098100");
        bigint a_sub_b(
                "7762734695592048966820170231725636502373717207660484286173528938406634905980512536658379215591534780796"
                "9821041166224772271425787557890604895055124604074824942818264548152905752285121759959565300609031623951"
                "028545726106389282219338039085915761653671045945093339056274698115477219870127312322202560240705197140");
        bigint a_mul_b(
                "7479135862310910556024333244188068796317944251150877644805806110477349309328350553611699319234828826265"
                "5871505236251462131963522692713806908747402564429436986688541528908166078567749762599779773518463797287"
                "5362089445930764109176676331670991564704100974252783723439662374276638984115783841908458809751437369579"
                "9376221568373166767435950989348873610601657870032417967933120583910142601287764060633176081830822003331"
                "4116047214849263107476054542190395484146664807372114199714640708870741940765044179826145339864552230154"
                "30469678683743825242338129034034486263988530103730781086414403778121013330611996053214609672289857600");
        bigint a_div_b = 2;
        bigint a_mod_b(
                "2164841992537783013067130534331578658384372610477697966227600058385765100497480162339089092238004912175"
                "2911192942299446785823309490695756396612212329813326541484038579140297314166031584248790074824987285546"
                "271706125523685546237135847991551340715289623370295176693909885419840258536634602384522796119753246660");
        bigint a_modInverse_n(
                "2514723138189900247833523177723745769306085980492425208070770631817764761791961646953637403228066016323"
                "4912300916408937081921926270858836620223410354504251806527375820666354425858753861618824473912787002265"
                "942710824067806970634680296077909951715607129976461813649365511282874893503947674172503481444556688527");
        bigint a_pow_b_mod_n(
                "3798221690101390767162415442687109735207716796929731443930369114209031927236780180886223169659834222988"
                "1264895927332672205801032981377769522067678937052010938220232556056434778823495477311643258054495417049"
                "591513314430345880159537348013769939870425523626083219620298080399627055144241124310946864785528749419");

        assert(a + b == a_add_b);
        assert(a - b == a_sub_b);
        assert(a * b == a_mul_b);
        assert(a / b == a_div_b);
        assert(a % b == a_mod_b);
        assert(a.modInverse(n) == a_modInverse_n);
        assert(a.modPow(b, n) == a_pow_b_mod_n);
        {
            cout << "Cost of 1024 bits big number arithmetic:" << endl;
            clock_t start, end;
            size_t times_quick = 1000;
            size_t times_slow = 10;
            double DOUBLE = 1.0;

            bigint temp(b);
            bool result = false;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                temp = a + temp;
            }
            end = clock();
            cout << "a + b cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                temp = temp - b;
            }
            end = clock();
            cout << "a - b cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_slow; i++) {
                temp = a * temp;
            }
            end = clock();
            cout << "a * b cost " << ((end - start) / (times_slow * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_slow; i++) {
                temp = temp / a;
            }
            end = clock();
            cout << "a / b cost " << ((end - start) / (times_slow * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_slow; i++) {
                temp = temp % a;
            }
            end = clock();
            cout << "a % b cost " << ((end - start) / (times_slow * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                temp = temp << 2;
            }
            end = clock();
            cout << "a << 1 cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                temp = temp >> 1;
            }
            end = clock();
            cout << "a >> 1 cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                result = result == a == b;
            }
            end = clock();
            cout << "a == b cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                result = result == a != b;
            }
            end = clock();
            cout << "a != b cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                result = result == a < b;
            }
            end = clock();
            cout << "a < b cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                result = result == a <= b;
            }
            end = clock();
            cout << "a <= b cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                result = result == a > b;
            }
            end = clock();
            cout << "a > b cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_quick; i++) {
                result = result == a >= b;
            }
            end = clock();
            cout << "a >= b cost " << ((end - start) / (times_quick * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_slow; i++) {
                a.pow(128);
            }
            end = clock();
            cout << "a ^128 cost " << ((end - start) / (times_slow * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_slow; i++) {
                a.modInverse(n);
            }
            end = clock();
            cout << "a.modInverse(n) cost " << ((end - start) / (times_slow * DOUBLE)) << "ms" << endl;

            start = clock();
            for (size_t i = 0; i < times_slow; i++) {
                a.modPow(b, n);
            }
            end = clock();
            cout << "a.modPow(b, n) cost " << ((end - start) / (times_slow * DOUBLE)) << "ms" << endl;

            cout << endl;
        }
    }
    cout << "All Tests Passed!" << endl;
    return 0;
}
