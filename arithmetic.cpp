#include "arithmetic.h"
#include "qtextcodec.h"


static bool hex_to_string(bool is_debug, QString hex_str,QString *des);
static bool string_to_hex(bool is_debug, QString str,QString *des) ;



arithmetic::arithmetic()
{
	init();
    test();
}


arithmetic::~arithmetic()
{


}



/*
	init
*/
void arithmetic::init(void)
{



}



/*
	測試用，用完就刪
*/
void arithmetic::test(void)
{

#if 1
	char l_buf[50];
	int i=0;
	qint64 l_len_str;

	bool l_result;
	QString l_str,l_str_hex;

	/*當發送緩冲區，以十六進制顯示要發送的數據，且把數據都轉成數組*/
	l_str_hex = "E5 90 8D E6 8F 9A 31 32 33 34 35 36 ";
	l_result = string_hex_to_char_array(false,l_str_hex,&l_len_str,&l_buf[0],50);

	/*清空以方便下個測試*/
	l_len_str = 0;
	for(i=0;i<50;i++)
	{
		l_buf[i] = 0;
	}
	
	/*當發送緩冲區，以字符顯示要發送的數據，且把數據都轉成數組*/
	//l_str = "mingyea123456";
	//l_str = "56名揚1234";
	l_str = "名揚123456";
	l_result = string_to_char_array(false,l_str,&l_len_str,&l_buf[0],50);

	/*把收到數據，數組轉換成字符型式顯示，且顯示的格式是十六進制形式*/
	l_str_hex = "";
	char_array_to_string_hex(&l_str_hex,l_len_str,l_buf);	

	/*把收到數據，數組轉換成字符型式顯示，且顯示的格式是字符形式*/
	l_str = "";
	char_array_to_string(&l_str,l_len_str,l_buf);

	if(l_result)
	{
        qDebug() << "" ;
	}
	

#endif

}


/**
	局部函數，將十六進制顯示的字符串，轉成字符串形式
    将hex(61 62 63 64 65 66 67) to String(abcdefg) ；	
    支持了4長度的ASCII漢字
    hex_str 輸入字符串
    des 轉換後 輸出的字符串 
 */
static bool hex_to_string(bool is_debug, QString hex_str,QString *des)
{
   		 
	bool l_result= true;
	if(is_debug) 
	{
		qDebug() << __func__ << ": " << hex_str;
	}
	QString ret="";
	QStringList list = hex_str.split(" ", QString::SkipEmptyParts);
	for(QString qs:list)
	{
		bool bStatus = false;
		int a = qs.toInt(&bStatus, 16);
		/*四個字節連一起--一般用於字ASCII漢字*/
		if(bStatus && (qs.length() == 4))
		{
			QString sA = QString(QChar(a));
			if(is_debug)
			{
                qDebug() << "a:" << a << "sA:" << sA << "qs:" + qs;
			}
			ret.append(sA);
			l_result = true;
		} 
		/*二個字節連一起--最普通*/
		else if(bStatus && (qs.length() == 2))
		{
			QString sA = QString(QChar(a));
			if(is_debug)
			{
                qDebug() << "a:" << a << "sA:" << sA << "qs:" + qs;
			}
			ret.append(sA);
			l_result = true;
		} 
		else
		{
            /* 转换失败，使用默认字体串 */
			if(is_debug)
			{
				qDebug() << "error!!!";
			}
			//            ret = "abcdefg";
			l_result = false;
			break;
		}
	}
	/*輸出結果*/
	if(l_result == true)
	{
		*des = ret;
	}
	return l_result;
}



/** 局部函數，字符函數，將字符串形式轉成十六進制形式
	将String(abcdefg) to Hex(61 62 63 64 65 66 67) 
	str 輸入字符串
    des 轉換後 輸出的字符串 
*/
static bool string_to_hex(bool is_debug, QString str,QString *des) 
{
	bool l_result= true;
	int i;
	
	if(is_debug)
	{
		qDebug() << __func__ << ": " << str;
	}
	QString ret;
	/* 将String(abcdefg) to Hex */
	for(i=0; i<str.length(); i++) 
	{
		/* 将字符串中字符转换成QChar */
        /*toLatin 改成了 unicode */
        QChar random = str.at(i).unicode();
		
		QString hex;
		QString str1;

		/*如果發現內容有錯，就退出返回*/
		if(random == 0x0000)
		{
			l_result = false;
			break;
		}
		
		/* 将QChar转换成unicode */
        hex.setNum(random.unicode(), 16);
        
		/**
		* 由于unicode位数随机，所以需要根据情况进行格式化
		* 只保留末两位，如果不够两位补0
		*/
		/*按長度轉換，整理字符串*/
		if(hex.length() >= 4) 
		{
			if(is_debug) 
			{
				qDebug() << "hex.length() >= 4 hex:" << hex;
			}
			str1 = hex.mid(hex.length() - 4, hex.length());
		} 
		else if(hex.length() >= 2) 
		{
			if(is_debug) 
			{
				qDebug() << "hex.length() >= 2 hex:" << hex;
			}
			str1 = hex.mid(hex.length() - 2, hex.length());
		} 
		else if(hex.length() == 1)
		{
			if(is_debug) 
			{
				qDebug() << "hex.length() == 1 hex:" << hex;
			}
			str1 = hex.prepend("0");
		}
		else
		{
			if(is_debug) 
			{
				qDebug() << "else";
			}
			str1 = "";
		}
		ret.append(str1.toUpper() + " ");
	}


	if(l_result == true)
	{
		if(str.length()==0)
		{
			l_result = false;
			/*輸入的字符串是空的*/
			qDebug() << "str is null";
		}
		else
		{
			*des = ret;
			/*轉換成功，且無錯誤*/
			qDebug() << "str successful";
		}
	}
	else
	{		
			l_result = false;
			/*轉換失敗，出錯*/
			qDebug() << "str is error or is chinese";
	}

	return l_result;
	
}



/** 接口函數
 	将string類型（且是字符串形式） 轉換成 數組，且標示長度；
 	str 要轉換 的字符串；
 	*size 轉換後的字節數；
 	buf 數組內容
 	max_size 充許的最大字節數
 */
bool arithmetic :: string_to_char_array(bool is_debug, QString str,qint64 *size,char *buf,qint64 max_size)
{
    
    bool l_result = false;
    char *l_str_char;
    int i;
    
    if(is_debug)
	{
		qDebug() << __func__ << ": " << str;
	}

    /*因為是字符串形式，所以要用UTF8解析帶漢字的碼*/
    QByteArray ba = str.toUtf8();

   	/*總字節數*/
    *size = ba.size();

    if(*size <= max_size)
    {
        l_str_char = ba.data();
        for(i=0;i<*size;i++)
        {
            buf[i] = l_str_char[i];
        }
        l_result = true;
		if(is_debug)
		{
			qDebug() << __func__ << "operate success" ;
		}

    }
    /*字節數超出範圍*/
    else
    {
        l_result = false;
        if(is_debug)
		{
			qDebug() << __func__ << "operate failed" ;
		}
    }
    
    return l_result;
}



/** 接口函數，
 	将string類型（十六進制顯示） 轉換成 數組，且標示長度
  	str 要轉換 的字符串；
 	*size 轉換後的字節數；
 	buf 數組內容
 	max_size 充許的最大字節數
 */
bool arithmetic :: string_hex_to_char_array(bool is_debug, QString str,qint64 *size,char *buf,qint64 max_size)
{
    QString l_str_hex;
    bool l_result = false;
    char *l_str_char;
    int i;
    
    if(is_debug)
	{
		qDebug() << __func__ << ": " << str;
	}
	
	/*先進行一次，將十六進制顯示的字符串 ，轉成普通字符串*/
    l_result = hex_to_string(true,str,&l_str_hex);

    if(l_result == true)
    {
		/*要間接轉成數組*/
		QByteArray ba = l_str_hex.toLatin1();

		*size = ba.size();

		if(*size <= max_size)
		{
		    l_str_char = ba.data();
		    for(i=0;i<*size;i++)
		    {
		        buf[i] = l_str_char[i];
		    }
		    l_result = true;
		   	if(is_debug)
			{
				qDebug() << __func__ << "operate success" ;
			}
		}
		/*字節數超出範圍*/
		else
		{
		    l_result = false;
		    if(is_debug)
			{
				qDebug() << __func__ << "operate failed" ;
			}
		}

    }
    else
    {
		l_result = false;
		if(is_debug)
		{
			qDebug() << __func__ << "operate failed" ;
		}
    }
    
    return l_result;
}



/**接口函數 
	将 數組 類型 轉換成 string （且以字符串形式顯式），且標示長度
	str 輸出字符串
	size 輸入數組的大小
	buf 輸入數組的內容
*/
bool arithmetic :: char_array_to_string(QString *str,qint64 size,char *buf)
{
	bool l_result = false;

	l_result = true;

	/*因為要按字符串形式，所以得解析UTF8*/
	*str = QString::fromUtf8 ( buf,size);

	return l_result;
}

/** 接口函數	
	将 數組 類型 轉換成 string ，且標示長度；
	str 輸出字符串
	size 輸入數組的大小
	buf 輸入數組的內容
*/
bool arithmetic :: char_array_to_string_hex(QString *str,qint64 size,char *buf)
{
	bool l_result = false;
	QString l_str;

	l_result = true;
	/*因為要按十六進制格式，所以不用UTF8；結果輸出字符串*/
	l_str = QString::fromLatin1  ( buf,size );
	/*把上一條語句成功轉換的字符串，轉成十六進制顯示*/
	l_result = string_to_hex(true,l_str,str);
	
	return l_result;
}




