Matrix Crypt
============

*Matrix Crypt* is a very inefficient and weak algorithm for encrypting stuff. It is vaguely similar to the [Hill cipher](http://en.wikipedia.org/wiki/Hill_cipher), that means it uses matrix algebra to encrypt, kinda like [this](http://aix1.uottawa.ca/~jkhoury/cryptography.htm) but different. It is just a small hobby project of mine, so don't except much. 

How does it work?
-----------------

Essentially, it converts the clear text and the password into matrices and multiplies them. The result is translated back into text, which is then compressed with zlib's [DEFLATE](http://en.wikipedia.org/wiki/DEFLATE) and encoded with [Ascii85](http://en.wikipedia.org/wiki/Ascii85). This means that the encrypted version is far larger than the original cleartext, but it is in a format that can easily be worked with (Ascii85). 

How can I try it?
-----------------

You can't. I am not done coding it yet, there are a couple of components in the code that still need to be written/finished/tested:

-   *CMatrix*, a class which represents matrices
+   *Ascii86*, the implementation of Ascii85
+   *GZip*, a wrapper around zlib's DEFLATE (has nothing to do with gzip, needs to be renamed)
-   *mops*, short for matrix operatrions, which deals with matrices
-   *MCrypt*, which performs the matrix encryption

Once all of these are done coding, I can get to finalize the API. Meanwhile you are welcome to help, if my coding style doesn't scare you. 

Some sample encrypted stuff
---------------------------

I have already written this algorithm in other languages, so I can demonstrate it here. I have encrypted this text:

> Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.

with the password "unfathomable", the encrypted output looks like this:

<~Gane$!knOXl]H>1i5;uj$]/bKF1=/n/1-Wm9*b],;VH\)/A@]Z%ksFf^^DCk3Pb/mFl9M-<TbLa%t4o6jd$!!3IR-3r&2,,XH'rNG(gUX$@)7m\?K%#0'JT;bWj(\i:12ti2liTXWZU6ILDePIo6JYq2i-^<qRGhN[]k_j]=4kJQXP?!a@E4Ynd1liWI@*W[UOCYJ+"X01V=1#3^\/@ie3b9R_dXkUh:M&;i`+qN;'0oPh$4;d:tkQ:+AK"]dqZ)"i]4W&lddY8&*Ne`@kri&KlKKD7Q2)'FlWMb+(s',ETNG]gAPcQ3K(`!eL,[Xk"ubp]VJ;^X.-'hSTJJae:$6#E:-J6:#6]F;B,CEp#+aJd]M[bOnp1;]C*bT4[0@^>5nOJYoPLap-'1WN]eI.?.1gd&/';kY\2'X:88jTf6)+SXg'U/oJ\Ae1:;?"HtIY9;(PYjPNP05VJ<9H8bsP0!`"E,cpW9e#NFA-WQr>11df<1p@^;@G3$(,H>RV._W'aY,?G&5b/$eI=7V5G_4Dq2o8[]RX,..o)?/7chO'_V4fg?ugoY9/-:0r61N3j[+U!3-_2f:Ibt6#Jpk`-&pp'3DAM>5Z=0mM2j$BqLquOf1=a8]Dbp\1#3XZLe\&r!kOLn'Vl"1W@\@?7a!^o9QAl2g7Z+f=G@`K%&$=\7@fRFONe3?JlLY[o9Feqg&m]D^]@I)SmkC^0p\/EbDQOG&T_Wh49,?]!/#Ru&c~>

