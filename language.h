#ifndef LANGUAGE_H
#define LANGUAGE_H

/*
#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif
*/
#include <QtWidgets>

#define TRUE 1
#define FALSE 0

typedef enum
{
    language_null,
    language_english,
    language_chs,
    language_cht,
}language_type_e;


class language : public QWidget
{
        Q_OBJECT
    public:
        explicit language(QWidget *parent = 0);

    signals:

    public slots:

    private:
        void func_init(void);

    public:
        bool switch_to_language(language_type_e sel_lan);
};

#endif // LANGUAGE_H
