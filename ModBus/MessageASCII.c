/**********************************************************

�������������� ��������� ��� ������ � ��������

**********************************************************/

 #include "ModBus.Def"


char Bin ( char data )
{
    switch (data)
    {
        case '0' : return 0;
        case '1' : return 1;
        case '2' : return 2;
        case '3' : return 3;
        case '4' : return 4;
        case '5' : return 5;
        case '6' : return 6;
        case '7' : return 7;
        case '8' : return 8;
        case '9' : return 9;
        case 'A' : return 0x0a;
        case 'B' : return 0x0b;
        case 'C' : return 0x0c;
        case 'D' : return 0x0d;
        case 'E' : return 0x0e;
        case 'F' : return 0x0f;
        case 'a' : return 0x0a;
        case 'b' : return 0x0b;
        case 'c' : return 0x0c;
        case 'd' : return 0x0d;
        case 'e' : return 0x0e;
        case 'f' : return 0x0f;
    }
    return 0;
}

//#pragma romdata __CONST_DATA__
const char ascii_cod[]={"0123456789ABCDEF"};

char ASCII ( char data )
{
    return ascii_cod[data&0x0f];
}

/*************************************************
�������������� �������� � ASCII ������ � �������� 
in-��������� �� ������ �������� ������
out-��������� �� ������ ����������
������������ ������ ��������������� ������
0 ��� ������ ��������������
*************************************************/
char ConvertString ( char *in, char cntByte, char *out)
{
    char lenth = 0;
    char cnt = 0;
    char tmp;
    if ( *in != ':' )   //��� ������ ������-�����.
        return 0;
    in++;
    do {
        if ( *in == STOP_1 )
        {
            if ( *++in == STOP_2 )
                return lenth;
            else return 0;
        }
        tmp = Bin( *in ) << 4;
        in++; cnt++;
        if ( *in == STOP_1 || *in == STOP_2 )
            return 0;
        tmp += Bin ( *in );
        in++; cnt++;
        lenth++;
        *out++=tmp;
    } while ( ( cnt < cntByte ) || ( *in != 0x0d ) );
    return 0;
}

/*********************************************************
�������������� ��������� � ������ ��������
in, out - ������� ��������� � ��������� ��������������
lenth - ������ �������� ���������
**********************************************************/
char ConvertMessag ( char *in, char *out, char lenth )
{
    char cnt=0;
    *out = ':';     //����� ������
    cnt++;
    do {
    out++; cnt++; 
    *out = ASCII ( (*in) >> 4 );
    out++; cnt++; 
    *out = ASCII ( (*in) & 0x0f );in++;
    lenth--;
    } while ( lenth > 0 );
    out++; cnt++;
    *out = STOP_1;
    out++; cnt++;
    *out = STOP_2;
    return cnt;
}

/**********************************************************
���������� CRC ���������.
������ - str,
������ - lenth
���������� char LRC
**********************************************************/

char LRC ( char *str, char lenth )
{
    unsigned char lrc = 0;
    while ( lenth >0 )
    {
        lrc += *str;
        str++;
        lenth--;
    }
    lrc&=0x0ff;
    return (unsigned char)(-((char)lrc));
}
