Matrix Crypt
============

*Matrix Crypt* is a pet project of mine. It is an encryption algorithm vaguely similar to the [Hill cipher](http://en.wikipedia.org/wiki/Hill_cipher), that means it uses matrix algebra to encrypt, kinda like [this](http://aix1.uottawa.ca/~jkhoury/cryptography.htm) but different. It provides more diffusion than a classic substitution cipher et the expense of space (it inflates data by 212.5%). 

How does it work?
-----------------

It's difficult to explain, you can find a better explanation in `doc/`. Essentially, it converts the clear text and the password into 2x2 matrices and multiplies them. This is how the algorithm manages to encrypt data, decryption is done in the same way except the inverses of the 2x2 password matrices are used.  
Encrypted data will be compressed with zlib's [DEFLATE](http://en.wikipedia.org/wiki/DEFLATE) and encoded with [Ascii85](http://en.wikipedia.org/wiki/Ascii85). The deflate algorithm compresses the encrypted data and the ascii85 algorithm encodes it in a format that can be easily used (like in mails or in the web). 

How can I try it?
-----------------

Since I'm still coding it, I don't really have anything that you can 'try' apart from the source code. CMatrixCrypt is made up of these essential files/classes:

-   *CMatrix*, a class which represents a matrix
-   *Ascii86*, my implementation of Ascii85
-   *Deflate*, a wrapper around zlib's DEFLATE algorithm
-   *SHA512*, a class that wraps around PolarSSL's implementation of SHA512
-   *util*, various utilities (that is, functions)
-   *mops*, short for matrix operations, which deals with matrices

Once all of these are done coding, I can get to finalize the API so that other people can actually use this code. In the meantime, you can have a look through the code, I think it it pretty much self-explanatory. 

If you want to code something that uses CMatrixCrypt, you should run `make`, which will give you the file `cmatrixcrypt.a` (in `obj`). When writing code, you can `#include "src/mops.hpp"` or any other header file, and then compile your code with `gcc -lz -o yourfile yourfile.cpp obj/cmatrixcrypt.a`. 

How is it being written?
------------------------

I am writing all of the code in C++. Basically, I am using some libraries (like PolarSSL and zlib) for which I write C++ wrapper classes (because I like Object Oritented Programming) and also I'm writing some classes myself (like Ascii86, CMatrix) which are useful for the Encryption. Then I just piece all the things together to create Matrix Crypt. This is how I'm planning to finish the project. 

All of the source code is in /src/. There are 2 libraries that I am using, one of which is PolarSSL, the part of it that I'm using is in /src/ in the folder /polarssl/. The other library is zlib, which needs to be linked via the linker argument `-lz` when compiling anything that uses the *Deflate* or *Inflate* classes. All of this is done automatically by the Makefile. The Makefile is set up so that it compiles all .cpp files that are in /src/ into object files which are stored in /obj/. These are then linked and places as binaries in /bin/. 

Some sample encrypted stuff
---------------------------

I have already written this algorithm in other languages, so I can demonstrate it here. I have encrypted this text:

> Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.

with the password "unfathomable", the encrypted output looks like this:

    <~Gane$!knOXl]H>1i5;uj$]/bKF1=/n/1-Wm9*b],;VH\)/A@]Z%ksFf^^DCk3Pb/mFl9M-<TbLa%t4
    o6jd$!!3IR-3r&2,,XH'rNG(gUX$@)7m\?K%#0'JT;bWj(\i:12ti2liTXWZU6ILDePIo6JYq2i-^<qR
    GhN[]k_j]=4kJQXP?!a@E4Ynd1liWI@*W[UOCYJ+"X01V=1#3^\/@ie3b9R_dXkUh:M&;i`+qN;'0oPh
    $4;d:tkQ:+AK"]dqZ)"i]4W&lddY8&*Ne`@kri&KlKKD7Q2)'FlWMb+(s',ETNG]gAPcQ3K(`!eL,[Xk
    "ubp]VJ;^X.-'hSTJJae:$6#E:-J6:#6]F;B,CEp#+aJd]M[bOnp1;]C*bT4[0@^>5nOJYoPLap-'1WN
    ]eI.?.1gd&/';kY\2'X:88jTf6)+SXg'U/oJ\Ae1:;?"HtIY9;(PYjPNP05VJ<9H8bsP0!`"E,cpW9e#
    NFA-WQr>11df<1p@^;@G3$(,H>RV._W'aY,?G&5b/$eI=7V5G_4Dq2o8[]RX,..o)?/7chO'_V4fg?ug
    oY9/-:0r61N3j[+U!3-_2f:Ibt6#Jpk`-&pp'3DAM>5Z=0mM2j$BqLquOf1=a8]Dbp\1#3XZLe\&r!kO
    Ln'Vl"1W@\@?7a!^o9QAl2g7Z+f=G@`K%&$=\7@fRFONe3?JlLY[o9Feqg&m]D^]@I)SmkC^0p\/EbDQ
    OG&T_Wh49,?]!/#Ru&c~>

You can see that the encrypted output is far longer than the raw text, this is an unfortunate side-effect of the matrix multiplication algorithm I am using.
