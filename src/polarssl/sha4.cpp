/* This is merely a C++ wrapper around sha4.c
 */

// I like this think to be in the namespace polarssl
namespace polarssl {
// after all, it's C code that we're dealing with here
extern "C" {
#include "sha4.c"
}
}
