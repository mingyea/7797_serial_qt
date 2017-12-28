#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <qstring.h>
#include <QDebug>


class arithmetic
{
	public:
		arithmetic();
		~arithmetic();
		

		bool string_to_char_array(bool is_debug, QString str,qint64 *size,char *buf,qint64 max_size);
		bool string_hex_to_char_array(bool is_debug, QString str,qint64 *size,char *buf,qint64 max_size);
		bool char_array_to_string_hex(QString *str,qint64 size,char *buf);
		bool char_array_to_string(QString *str,qint64 size,char *buf);


    private:
        void init(void);
        void test(void);

};





#endif // ARITHMETIC_H
