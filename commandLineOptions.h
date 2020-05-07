//-*-c++-*-

#pragma once
#ifndef COMMANDLINEOPTIONS_H
#define COMMANDLINEOPTIONS_H


#include <QCommandLineParser>
#include <QList>
#include <QVariant>

#include <iostream>

///////////////////////////////////////////////////////////////////////////
//
// This is a Qt command line processor
//
// To use:
//     CommandLineOptions options;
//     options.add(<name of option, no dashes>, <QVariant variable>)
//     options.parse()
//
// 1. Dashes are thrown out, to you can use either -d or --d
// 2. You can also use an equals assignment, e.g -d=100
// 3. A param with no argument should only be used with boolean
//    variants. A boolean variant passed with a -<param> will be
//    inverted (e.g. just "-d"). You can also assign it using
//    the "=" option, e.g. "-d=false".
// 4. Variables other then boolean should always have an argument
//    else it's an error, e.g. -f 3.14159
//
//  See example at bottom.
//
///////////////////////////////////////////////////////////////////////////
class CommandLineOptions {
    struct Options {
        QString   _name;
        QVariant  *_result;
    };
    QList<Options> _options;

  public:
    CommandLineOptions() {}
    ~CommandLineOptions() { _options.clear();}

//
// add an option
// 
    void add(const char *name, QVariant *result) {
        Options s;
        s._name = name;
        s._result = result;
        _options.push_back(s);
    }

    void parse(void) {
        processCommandLine();
    }

  private:

//
// check for argument of the form "x=100"
// call with command line arg, the name of the option,
// and the result if match found
// e.g. checkEqualsArg(arg,"x",xarg)
//
    bool checkEqualsArg(const QString &arg
                        ,const QString &option_in
                        ,QVariant *result
                        ) {
        QString option = QString(option_in) + "=";
        if (arg.startsWith(option, Qt::CaseInsensitive)) {
            QStringList parts = arg.split("=");
            if (parts.count() > 1) {
                QString value = parts[1];
            // this will work with a boolean, no need to check!
                *result = value;
                return true;
            }
        }
        return false;
    }

    void processCommandLine(void) {
        QStringList args = QCoreApplication::arguments();
        QVariant *variantPointer = nullptr;
        foreach(QString arg, args) {
        //
        // waiting for an argument to a "-<value>". variantPointer
        // should be pointing to the variable found below
        //
            if (variantPointer != nullptr) {
                *variantPointer = arg;  // assign the argument
                variantPointer = nullptr;
            }        

            QString tmp = arg;
        //
        // remove all "-"'s and leave the name only
        //
            while (tmp.startsWith("-")) {
                tmp.remove(0,1);            // remove the '-'
            }
        //
        // process all options
        //
            for(int i=0; i<_options.size(); ++i) {
                Options s = _options.at(i);
            //
            // process all "-<letter>" options
            // If the variant variable is a boolean, invert it,
            // else just assign the value.
            //
                if (s._name == tmp) {
                    if (s._result->type() == QVariant::Bool ) {
                        bool b =(*s._result).toBool();
                        *s._result = !b;
                    } else {
                        variantPointer = s._result;
                    }
                }

            //
            // process all <letter>=<value> args
            // 
                if (checkEqualsArg(tmp
                                   ,s._name
                                   ,s._result)) {
                    variantPointer = s._result;
                }
            }
        }
    }
};

#ifdef COMMANDLINEOPTIONS_EXAMPLE

#include <QCoreApplication>
#include <QVariant>
#include <iostream>

#include "commandLineOptions.h"

int main(int argc, char * argv[])
{
    using namespace std;
    
    QVariant stringVar = "hello";
    QVariant boolVar = false;
    QVariant intVar  = 30;
    QVariant floatVar = 3.14159;
    QCoreApplication app(argc,argv);
    CommandLineOptions options;
    options.add("s",&stringVar);
    options.add("b",&boolVar);
    options.add("i",&intVar);
    options.add("f",&floatVar);
    options.parse();
    cout << qPrintable(stringVar.toString()) << endl;
    cout << boolVar.toBool() << endl; // displays 0 or 1
    cout << intVar.toInt() << endl;
    cout << floatVar.toFloat() << endl; // or toDouble

    return 0;
    
}

#endif


#endif //COMMANDLINEOPTIONS_H

