#include <QCoreApplication>

#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QDateTime>

QString GetRandomString(uint len);

// Example output:
/*
1|c1de75f7356d6b4e5fd9f91cd8fea8da8e496ad15d3daa5adffcb05afb5396cbfb04ec1118fe105da68a441a16e46ac0def28e45d3c4b5f0e50bc2d797629d6c|
2|2918b536dc3e9b76b08c3069bec49bd45fc22091729e3efe53646eb716cc151ce3f5ecdcab9ba28b09ae84bc05999adef8ec972f0b1428f3444da297a9715c49|
3|2ec84d37f15f8a563953035b8cf5df2cc235413acac83031e5bcea0a0c462f1e1172280d5437527437534bece55eb2089715246595a476a43a4cebe7aecb7cc0|
4|ff290439cb50e6656cd88e1a3589bba2698e894facd5d4c6ed81702eb8659c74769fa369d017ce1a0ab1443956da6a7451ddee5c7f4d694eb69e3779750c5bb3|
5|62d36b3b2655bc95bc14e4884c3650a3ab04ca1f8fb8757a3f4cf2ede232ee47d07df6c6a6b87529f2475bb14c343ade9399039f8e7692d0250edb7a662739ae|
6|550cf1fc48034feacfcd76f40eb677ed80c758fe95c1cf1f15cbaa0650127c5bc373e79446d4f57cc5cd9a143bc475adafda527e690efe61f35a853d9359a84d|
7|2b44357af3341f1d672f8f35cb3e266355d2b026cae5de483af9323f7d894b6b521e0e8734f121e2bd6ea54b274d2c83a4bed8877ac2029a189617ba83f9b3b7
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCryptographicHash qSHA3(QCryptographicHash::Sha3_512);
    QFile fStore("data.txt");

    unsigned const int accounts = 7;
    bool printPasswords = true;
    bool printHash      = true;

    bool generateSalt   = true;

    uint saltLen = 6;
    uint passLen = 12;

    if (!fStore.open(QIODevice::WriteOnly | QIODevice::Text)) {
             return 1;
    }
    else {
        QTextStream fStream(&fStore);
        if(printPasswords || printHash)
            qDebug() << "Randomly generated password for: \n";

        // Pass_ID | Acc_ID | PwHash | PwSalt
        for(unsigned int i=1; i<= accounts; i++)
        {
            QString randomPassword = GetRandomString(passLen);
            QString randomSalt="";

            if(generateSalt)
                randomSalt = GetRandomString(saltLen);

            qSHA3.addData(QString(randomPassword + randomSalt).toUtf8());

            if(printPasswords)
                qDebug() << "User: " << i << '\n'
                         << "Passowrd: " << randomPassword << '\n';

            if(printHash)
                qDebug() << "Hash: " << qSHA3.result().toHex() << '\n';

            if(i<accounts)
                fStream << i << '|' << i << '|' << qSHA3.result().toHex() << '|' << randomSalt << "|\n";
                //fStream << i << '|' << i << '|' << GetRandomString(8) << "|\n";
            else
                fStream << i << '|' << i << '|' << qSHA3.result().toHex() << '|' << randomSalt;
                //fStream << i << '|' << i << '|' << GetRandomString(8);
            qSHA3.reset();
        }

        fStore.close();
    }

    return a.exec();
}

QString GetRandomString(uint length)
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   uint randomStringLength = length;
   QTime time = QTime::currentTime();
   qsrand(time.msec());
   QString randomString;
   for(uint i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}
