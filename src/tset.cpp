// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp) 
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField) {
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf) 
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    TBitField res = BitField;
    return res;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    bool isMem = BitField.GetBit(Elem);
    return isMem;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (BitField.GetBit(Elem) != 0) BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower != s.MaxPower) return 0;
    if (BitField != s.BitField) return 0;
    return 1;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField tmp = BitField | s.BitField;
    TSet res(tmp);
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res(BitField);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet res(BitField);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TBitField tmp = BitField & s.BitField;
    TSet res(tmp);
    return res;
    return TSet(0);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField tmp = ~BitField;
    TSet res(tmp);
    return res;
    return TSet(0);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
