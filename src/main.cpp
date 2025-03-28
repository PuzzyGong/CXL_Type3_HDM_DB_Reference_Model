#include "define.h"
#include "s_test.h"

int main() {
    S_Test_Type test;
    test.device_self_test(
        "{ MemRd + SnpInv  + A  <900>  001  (011) }{ XXXXXX  XXX }{ MemWr + NoOp + A  <909>  011  (033) [533] }."
        "{ XXXXX + XXXXXX  + X  <XXX>  XXX  (XXX) }{ XXXXXX  XXX }{ XXXXX + XXXX + X  <XXX>  XXX  (XXX) [XXX] }."
        "{ MemRd + SnpInv  + A  <901>  003  (011) }{ XXXXXX  XXX }{ XXXXX + XXXX + X  <XXX>  XXX  (XXX) [XXX] }."
        "{ XXXXX + XXXXXX  + X  <XXX>  XXX  (XXX) }{ BIRspI  000 }{ XXXXX + XXXX + X  <XXX>  XXX  (XXX) [XXX] }."
        "{ MemRd + SnpData + S  <902>  005  (011) }{ XXXXXX  XXX }{ XXXXX + XXXX + X  <XXX>  XXX  (XXX) [XXX] }."
        "{ XXXXX + XXXXXX  + X  <XXX>  XXX  (XXX) }{ BIRspS  001 }{ XXXXX + XXXX + X  <XXX>  XXX  (XXX) [XXX] }."
        "{ XXXXX + XXXXXX  + X  <XXX>  XXX  (XXX) }{ XXXXXX  XXX }{ XXXXX + XXXX + X  <XXX>  XXX  (XXX) [XXX] }."
    );
    return 0;
}
