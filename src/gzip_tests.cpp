#include "gzip.hpp"
using namespace std;

// try compressing multiple string with gzip
int main(void) {
  string tst1, tst2;
  tst1 = "Harrow, Harrow, said the Korean, ";
  tst2 = "who likes to eat pie. ";
  int compressionlevel = Z_BEST_COMPRESSION;

  // MEMORY STRUCTURES
  z_stream zs;                        // z_stream is zlib's control structure
  std::string outstring;
  int ret;
  char outbuffer[32768];
  memset(&zs, 0, sizeof(zs));
 
  // INITIALIZE
  if (deflateInit(&zs, compressionlevel) != Z_OK)
    throw(std::runtime_error("deflateInit failed while compressing."));

  // SET UP INPUT
  zs.next_in = (Bytef*)tst1.data();
  zs.avail_in = tst1.size();           // set the z_stream's input

  // COMPRESSION

  // retrieve the compressed bytes blockwise
  do {
    zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = deflate(&zs, Z_NO_FLUSH);

    if (outstring.size() < zs.total_out) {
      // append the block to the output string
      outstring.append(outbuffer,
          zs.total_out - outstring.size());
    }
  } while (ret == Z_OK);
  // END OF COMPRESSION

  // SET UP INPUT2
  zs.next_in = (Bytef*)tst2.data();
  zs.avail_in = tst2.size();           // set the z_stream's input

  // COMPRESSION

  // retrieve the compressed bytes blockwise
  do {
    zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = deflate(&zs, Z_NO_FLUSH);

    if (outstring.size() < zs.total_out) {
      // append the block to the output string
      outstring.append(outbuffer,
          zs.total_out - outstring.size());
    }
  } while (ret == Z_OK);
  // END OF COMPRESSION
  zs.next_in = NULL;
  zs.avail_in = 0;           // set the z_stream's input

  do {
    zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = deflate(&zs, Z_FINISH);

    if (outstring.size() < zs.total_out) {
      // append the block to the output string
      outstring.append(outbuffer,
          zs.total_out - outstring.size());
    }
  } while (ret == Z_OK);
  
  // FINALIZE
  deflateEnd(&zs);

  if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
    std::ostringstream oss;
    oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
    throw(std::runtime_error(oss.str()));
  }

  cout << decompress_string(outstring) << endl;
}
