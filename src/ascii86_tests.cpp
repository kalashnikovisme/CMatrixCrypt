#include <iostream>
#include <string>
#include <cassert>
#include "ascii86.hpp"
using namespace std;

int main(void) {
  cme::Encode86 enc;
  string tmp;

  // all of these tests use data from the reference material from http://www.tools4noobs.com/online_tools/ascii85_encode/

  // test for hello, world
  enc << "Hello, World";
  enc.close();
  enc >> tmp;
  assert(tmp == "<~87cURD_*#4DfTZ)~>");
  enc.reset(); tmp.clear();
  
  // test for somethin' longer
  enc << "The advantage";
  enc.close();
  enc >> tmp;
  assert(tmp == "<~<+ohc@:XOiDKK<\"AH~>");
  enc.reset(); tmp.clear();

  // test for one more sentence
  enc << "This should ensure that the algorithm is working allright. ";
  enc.close();
  enc >> tmp;
  assert(tmp == "<~<+oue+EM47F_kS2ASuR3Eb-A8BOQ!*FD,5.@;KRpEbTW/D'3P6+EqaECM@[!+CT)&EbT0\"F=n[~>");
  enc.reset(); tmp.clear();
  
  // let's try this, shall we?
  cme::Decode86 dec;
  dec.debug();
  dec << "<~<+oue+EM47F_kS2ASuR3Eb-A8BOQ!*FD,5.@;KRpEbTW/D'3P6+EqaECM@[!+CT)&EbT0\"F=n[~>";
  dec.debug();
  dec >> tmp;
  dec.debug();
  assert(tmp == "This should ensure that the algorithm is working allright. ");
  dec.reset(); tmp.clear();
}
