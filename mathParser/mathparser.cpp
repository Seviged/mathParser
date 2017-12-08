#include "mathparser.h"
#include <QtMath>


MathParser::MathParser()
{
    numOfIteration = 0;
    priority = 0;// mathOpers = 0, powOpers = 1, divOpers = 2, sumOpers = 3;

    allOpers << "^" << "*" << "/"  << "+" << "-" << "A" << "B" << "C" << "D" << "E"
             << "F" << "G" << "H" << "I" << "K" << "L" << "M" << "N" << "O";
    wrongSymbols << "=" << "!" << "@" << "#" << "$" << "%" << "&" << "?" << "№"
                 << "'" << "\"" << ";" << "`" << "~" << "|" << "_" << "[" << "]" << "<" << ">" << "{" << "}";

    simpleOpers << "+" << "-" << "^" << "/" << "*";
    sumOpers << "+" << "-";
    divOpers << "*" << "/";
    powOpers << "^" << "N";                                        //выполняется с меньшим приоритетом, но нужна здесь для парсинга строки<< "N";
    mathOpers << "D"    << "E"    << "F"    << "G"    << "H"    << "I"    << "A"   << "B"   << "C"   << "K"   << "L"   << "M"    << "#"    << "O" ;
    mathNames << "sinh" << "cosh" << "tanh" << "asin" << "acos" << "atan" << "sin" << "cos" << "tan" << "exp" << "log" << "l10g" << "sqrt" << "ln";

    constNumbers<< "2.7182818284590452354" << "3.14159265358979323846";
    constNames  << "e" << "pi";

    /* sin  - A
     * cos  - B
     * tan  - C
     * sinh - D
     * cosh - E
     * tanh - F
     * asin - G
     * acos - H
     * atan - I
     * exp  - K
     * log  - L
     * log10- M
     * sqrt - N
     * ln   - O
     */

}

QStringList MathParser::parse(QString str)
{
    //result[0] = answer|error message
    //result[1] = 0 is answer|-1 is error
    //result[2] = number of ietrations
    QStringList result;
    numOfIteration = 0;
    priority = 0;

    QString string;
    string = str;
    string = prepareString(string);

    //если первый символ b - то это непременно ошибка ввода, удалим первый символ и выведем
    if (string.at(0) == 'b') {
        string.remove(0,1);
        result.append("ERROR: " + string);
        result.append(QString::number(-1));
    }
    else {
    //либо начнем парсить строку
        double var = parseBracets(string);
        result.append(QString::number(var));
        result.append(QString::number(0));
        result.append(QString::number(numOfIteration) + " total iterations");
    }

    return result;
}

QString MathParser::calculate(double a, double b, QString operation)
{
    //тут мы калькулирем всякое разное
    if (operation == "+") return QString::number(a + b);
    if (operation == "-") return QString::number(a - b);
    if (operation == "*") return QString::number(a * b);
    if (operation == "^") return QString::number(qPow(a, b));
    if (operation == "A") return QString::number(qSin(b));
    if (operation == "B") return QString::number(qCos(b));
    if (operation == "C") return QString::number(qTan(b));
    if (operation == "D") return QString::number(sinh(b));
    if (operation == "E") return QString::number(cosh(b));
    if (operation == "F") return QString::number(tanh(b));
    if (operation == "G") return QString::number(qAsin(b));
    if (operation == "H") return QString::number(qAcos(b));
    if (operation == "I") return QString::number(qAtan(b));
    if (operation == "K") return QString::number(qExp(b));
    if (operation == "L") return QString::number(log(b));
    if (operation == "M") return QString::number(log10(b));
    if (operation == "N") return QString::number(qSqrt(b));
    if (operation == "O") return QString::number(qLn(b));
    return QString::number(a / b);
}

QString MathParser::parseString(QStringList strList, int currentPriority)
{
    numOfIteration++;

    //выбираем список операций с нужным приоритетом
    QStringList opers;
    switch (currentPriority) {
    case 0 :
        opers = mathOpers;
        break;
    case 1 :
        opers = powOpers;
        break;
    case 2 :
        opers = divOpers;
        break;
    case 3 :
    default:
        opers = sumOpers;
        break;
    }

    //вычисление по одному оператору за одну итерацию,
    //если переменная depth не обнулена - переходим на следующий список операторов в новой итерации
    int depth = 1;
    for(int i = 0; i < strList.size(); i++) {
        if (opers.contains(strList.at(i))) {
            strList.replace(i, calculate(strList.at(i - 1).toDouble(), strList.at(i + 1).toDouble(), strList.at(i)));
            strList.removeAt(i + 1);
            strList.removeAt(i - 1);
            depth = 0;
            break;
        }
    }

    if (depth == 1) currentPriority++;

    if (strList.size() > 1) {
        return parseString(strList, currentPriority);
    }
    return strList.at(0);
}

double MathParser::parseBracets(QString mystr)
{
    numOfIteration++;
    int openBracket = 0;
    int closeBracket = 0;
    QString strbuf;
    QString str = mystr;

    //тут мы находим первую закрывающую скобку в строке, помня про последнюю открывающую
    //выражение в скобках имеет высший приоритет, выкусим в отдельную строку
    for(int i = 0; i < str.size(); i++)
    {
        if (str.at(i) == QChar('(')) {
            openBracket = i;
            strbuf.clear();
        }
        else if (str.at(i) == QChar(')')) {
            closeBracket = i;
            break;
        }
        else {
            strbuf.append(str.at(i));
        }
    }
    if (openBracket == closeBracket) str.clear();                   //если нет скобок вообще - удаляем всё из строки
    else str.remove(openBracket, closeBracket - openBracket + 1);   //или выкусываем скобки

    //парсим выкусаную строку, конвертим в список и отправляем в парсер
    QString buffer;
    QStringList strList;
    for (int i = 0; i < strbuf.size(); i++) {
        //первый случай для корректной обработки минуса перед значением в качестве знака, а не оператора.
        //второй случай для всего остального, кроме минуса
        if ((strbuf.at(i) == '-' && (i > 0 && !(allOpers.contains(strbuf.at(i - 1))))) || (allOpers.contains(strbuf.at(i)) && strbuf.at(i) != '-')) {
                strList.append(buffer);
                strList.append(strbuf.at(i));
                buffer.clear();
        }
        else buffer.append(strbuf.at(i));
    }
    strList.append(buffer);
    strbuf = parseString(strList, priority);

    //возвращаем результат из парсера в строку
    str.insert(openBracket, strbuf);

    //если наша строка - похожа на единственное число - возвращаем его
    //если конвертация в число не произошла, итеративно вызываем эту же функцию
    bool ok;
    double var = str.toDouble(&ok);
    if (!ok) return parseBracets(str);
    return var;
}

QString MathParser::prepareString(QString str)
{
    //тут мы заменяем некоторые символы на нужные нам, чистим от пробелов и от неизвестных нам сиволов
    str.replace(QString(" "), QString("")); //remove &nbsp
    str.replace("\\", QString("/"));
    str.replace(QString(":"), QString("/"));
    str.replace(QString(","), QString("."));
    str.remove(QRegExp("([A-Zа-яА-Я])"));

    //модифицируем строку, подставляя операции умножения между скобками\скобкой и числом
    for(int i = 0; i < str.size(); i++) {
        if (str.at(i) == QString("(") && i > 0) {
            if (str.at(i - 1).isNumber() || str.at(i - 1) == ")" || mathOpers.contains(str.at(i - 1))) {
                str.insert(i, QString("*"));
                i = 0;
            }
        }
        if (i > 0 && str.at(i - 1) == QString(")")) {
            if (str.at(i).isNumber() || mathOpers.contains(str.at(i))  || str.at(i) == ".") {
                str.insert(i, QString("*"));
                i = 0;
            }
        }
    }

    //тут мы заменяем строки с операторами на символы в верхнем регистре (для парсера операция = один символ)
    int pos = 0;
    for (int i = 0; i < mathNames.size(); i++) {
        pos = 0;
        while (pos != -1) {
            pos = str.indexOf(mathNames.at(i));
            if (pos != -1) {
                str.remove(pos,mathNames.at(i).size());
                if (mathNames.at(i) == "sqrt") str.insert(pos,"N"); //у sqrt приоритет ниже остальных в mathOpers(там нет N, он в powOpers),
                                                                    //но отпарсить в N её нужно
                else str.insert(pos,mathOpers.at(i));
            }
        }
    }

    //тут нужно проверить, а не хочет ли пользователь написать две константы подряд, до валидатора не дотянуть
    for (int i = 0; i < constNames.size();i++) {
        if (str.indexOf(constNames.at(i) + constNames.at(i)) != -1)
            return QString("b constant '" + constNames.at(i) + "' duplicate");
    }

    //f тут мы заменяем константы на числа (Е и Пи в данный момент)
    pos = 0;
    for (int i = 0; i < constNames.size(); i++) {
        pos = 0;
        while (pos != -1) {
            pos = str.indexOf(constNames.at(i));
            if (pos != -1) {
                str.remove(pos,constNames.at(i).size());
                str.insert(pos,constNumbers.at(i));
            }
        }
    }

    //подставляем знак умножения между числами и сложными операторами (синус и тд)
    for (int i = 0; i < str.size(); i++) {
        if (i > 0 && str.at(i - 1).isNumber()) {
            if (mathOpers.contains(str.at(i))) {
                str.insert(i, QString("*"));
                i = 0;
            }
        }
        if (i > 0 && str.at(i - 1) == QString(")")) {
            if (mathOpers.contains(str.at(i))) {
                str.insert(i, QString("*"));
                i = 0;
            }
        }
    }

    //удаляем и заменяем символы нижнено регистра. при правильной строке на этом этапе их уже нет
    //(кроме "х", если замену сделать вверху - мы потеряем экспоненту)
    str.replace(QString("x"), QString("*"));
    //str.remove(QRegExp("([a-z])"));
    if (str.indexOf(QRegExp("([a-z])")) != -1) return QString("b unknown symbol, check the input string for syntax errors ");
    str = validate(str);

    return str;
}

QString MathParser::validate(QString str)
{
    //а не пустую ли строку нам подсунули?)
    if (str.isEmpty()) return QString("b string is empty");
    if (str.size() > 4096) return QString("b string size > 4096 symbols, abort");

    //проверяем наличие неизвестных нам символов
     for (int i = 0; i < wrongSymbols.size(); i++) {
         if (str.contains(wrongSymbols.at(i))) {
             return QString("b unknown symbol: " + wrongSymbols.at(i));
         }
     }

    //проверяем количество открывающих и закрывающих скобок
    int op = 0;
    int cl = 0;
    for(int i = 0; i < str.size(); i++) {
        if(str.at(i) == QString("(")) {
            if   (op >= cl) {
                op++;
            }
            else return QString("b wrong brackets, stop testing me, buddy");
        }
        if(str.at(i) == QString(")")) {
            cl++;
        }
        //проверка на наличие скобки после сложного оператора
        if(mathOpers.contains(str.at(i))) {
            if (str.at(i + 1) != "(") return QString("b miss '(' after math operator");
        }
    }
    if (op != cl) return QString("b unequal number of brackets: " + QString::number(op) + " '(' and " + QString::number(cl) + " ')'");

    for (int i = 0; i < str.size(); i++) {
        if(str.at(i) == QString("(")) {
            // если первый символ - оператор с двумя аргументами
            if (simpleOpers.contains(str.at(i + 1))) {
                if (str.at(i + 1) != "-") return QString("b error in operators, check first after open bracket");
                else {
                    if (str.at(i + 2) == "-") return QString("b error in operators, check first '-' after open bracket");
                }
            }
        }
        if(str.at(i) == QString(")")) {
            //если перед скобкой оператор - это точно ошибка
            if (!(str.at(i - 1).isNumber()) && str.at(i - 1) != "(" && str.at(i - 1) != ")") return QString("b error in operators near close bracket");
        }
        //проверка запрещенного применения двух операторов, разрешен вариант применения "-" вторым оператором
        if (i > 0 && allOpers.contains(str.at(i))) {
            if (allOpers.contains(str.at(i - 1))) {
                if (simpleOpers.contains(str.at(i)) && str.at(i) != "-") return QString("b error in operators");
                else {
                    if ((i + 1) < (str.size() - 1) && str.at(i + 1) == "-") return QString("b error in operators, too much '-'");
                }
            }
        }
    }

    QString pstr = str;
    // если последний символ - оператор
    if (allOpers.contains(pstr.at(pstr.size()-1))) return QString("b error in operators, check last");
    // если первый символ - оператор с двумя аргументами
    if (simpleOpers.contains(pstr.at(0)) && (pstr.at(0) != "-" || pstr.at(1) == "-")) return QString("b error in operators, check first");

    //проверка на отсутствие чисел, удаляем все операторы
    for (int i = 0; i < allOpers.size(); i++) {
        pstr.remove(allOpers.at(i));
    }
    //находим первую закрывающую скобку, вспоминаем последнюю открывающую, если между ними ничего нет - ошибка. удаляем эти скобки, повторяем заново
    while (pstr.indexOf(")") != -1) {
        int openBracket = 0;
        int closeBracket = 0;
        for(int i = 0; i < pstr.size(); i++)
        {
            if (pstr.at(i) == QChar('(')) {
                openBracket = i;
            }
            else if (pstr.at(i) == QChar(')')) {
                closeBracket = i;
                break;
            }
        }
        if ((closeBracket - openBracket) <= 1) return QString("b empty brackets");
        else {
            pstr.remove(closeBracket, 1);
            pstr.remove(openBracket, 1);
        }
    }

    //если в строке были только скобки и операторы
    if (pstr.isEmpty()) return QString("b numbers are missing");

    return str;
}
