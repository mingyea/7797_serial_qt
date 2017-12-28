#include "language.h"
#include <QTranslator>

language::language(QWidget *parent) : QWidget(parent)
{
    //init();
}


/*
*	語言的切換；
*/
bool language::switch_to_language(language_type_e sel_lan)
{

   QTranslator *translator = new QTranslator(qApp);
   bool b = false;

   switch(sel_lan)
   {

       case language_cht:
           //b = translator->load(QString(":/res/%1.qm").arg("cn"));
           b = translator->load(":/res/lan/ct.qm");
           break;

       case language_chs:
           //b = translator->load(QString(":/res/%1.qm").arg("cn"));
           //b = translator->load(QString(":/%1.qm").arg("cn"));
           b = translator->load(":/res/lan/cn.qm");
           break;

       case language_english:
           //b = translator->load(QString(":/res/%1.qm").arg("en"));
           b = translator->load(":/res/lan/en.qm");
           break;

       default:
           //b = translator->load(QString(":/res/%1.qm").arg("cn"));
           b = translator->load(":/res/lan/ct.qm");
           break;

   }

   if(b)
   {
        qDebug() << "language switch load qm successful";
   }

   qApp->installTranslator(translator);

   delete translator;

    return TRUE;
    //return 1;
}


/*
void language::func_init()
{

    switch_to_language(language_cht);


}
*/
