# mathParser
Simple parser library for calculating mathematical expressions written in C++ and Qt5.
Project conatins dll library sources in mathParser directory, and simple program simParser(simpleParser) for evaluating functionality of the library. Project build in Qt Creator 4.4.1, and use Qt 5.9.2. Compiler minigw32.

# Syntax description
Parser can recognize the following operators:
* "( )" brackets are most priority operators
* "sin(), cos(), tan(), sinh(), cosh(), tanh(), asin(), acos(), atan(), exp(), log(), l10g(), ln()" are second priority operators
* "^ and sqrt()" are third priority operators
* "\* /" are fourth priority operators
* "\+ \-" are last priority operators

Also, it can recognize numbers (int or double) and constants (e and pi).

Before parsing this lib prepare string:
* remove all space(" ") symbols
* remove all [A-Z] symbols
* remove all [А-Я] symbols
* remove all [а-я] symbols
* replace x by \*
* replace : by /
* replace \\ by /
* replace , by .
* insert \* symbol between bracets 
<pre><code>   ")(" == ")*("</code></pre>
* insert \* symbol between bracket and number(or math operator from second priority operators list or sqrt())
<pre><code>   ")66" == ")*66"
   "sqrt(4)6" == "sqrt(4)*6"
</code></pre>
* insert \* symbol between two math operator from second priority operators list or sqrt()
<pre><code>   "sqrt(4)sqrt(4)" == "sqrt(4)*sqrt(4)"</code></pre>

# Simple use (simParser)
simParser is simple calculator, which return answer for input math string.

Typical syntax srting to parse is:
<pre><code>2+2*(2^2-2)*5</code></pre>
But you can write this, and it will be correct:
<pre><code>2 +   2x(   2^2   - 2)5</code></pre>
These two strings are indentical for parser.

# How to use mathParser
Just compile it and include in your project as dynamic linked library.

MathParser has one dependence - qt5, and it's base module - QtBase5.

Example of use:
```cpp
//don'forget about include
#include "mathParser.h"

MathParser *parser = new MathParser;
QStringList res;
res = parser->parse(ui->lineEdit->text());

if (res.at(1).toInt() == -1) {
        //if res[1] is -1, res[0] contains string with error description
        debugOut(res.at(0));
    }
    else
    {
        //else res[0] contains answer (in QString, to convert use res[0].toDouble())
        //res[2] contains number of iterative calculations (in QString)
        resultOut(res.at(0));
        debugOut(res.at(2) + " total iterations");
    }
```

Return type for parser->parse(QString) is QStringList smth(for example).

If smth[1] is "-1", you has an error. smth[0] contains description of error.

If smth[1] is "0", you has no errors. smth[0] contains result of calculating(in QString), smth[2] contains number of iterative calculations.
