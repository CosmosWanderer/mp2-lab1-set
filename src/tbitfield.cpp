// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

/* 
Поташник Максим 3823Б1ФИ1 2 курс 1 лаба

Я тут в комментах сам с собой болтаю немного, но не обращайте внимания
*/

TBitField::TBitField(int len)
{
    BitLen = len;  
    if (len < 0) throw "Incorrect length!";
    MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    /* 
              BitLen
    MemLen = ---------
             Биты в одном TELEM
    
    а ещё штука, которая нужна для того, чтоб "остаток" битов тоже поместился
    */
    pMem = new TELEM[MemLen](); // Выделили память
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
    pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    /* 
    Вот у нас всё memory состоит из какого-то (BitLen) числа TELEM, которые мы на биты и делим как бы
    И вот этот индекс нам нужен, чтоб взять правильный TELEM, в котором именно и хранится бит номер n
    */
    int Index = n / (sizeof(TELEM) * 8);
    return Index;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита 
{
    // Маска - битовове поле, где только n-ый бит равен 1.
    TELEM Mask = 1;
    Mask <<= n % (sizeof(TELEM) * 8);
    return Mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    // Ну BitLen это indeed кол-во битов
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) throw "Incorrect index!";
    // Получаем индекс бита
    int Index = GetMemIndex(n);
    // Делаем маску
    TELEM Mask = GetMemMask(n);
    // Устанавливаем бит с помощью побитового или
    pMem[Index] |= Mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw "Incorrect index!";
    // Тут сложнее - если мы хотим удалить бит, нам нужна особая маска, в которой этого бита нет...
    // А получить её можно, если её инвертировать...
    // Или можно просто использовать xor, да. 
    int Index = GetMemIndex(n);
    TELEM Mask = GetMemMask(n);
    // xor
    pMem[Index] ^= Mask;

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw "Incorrect index!";
    // После "и" с маской, мы либо получим все нули, если бита нет на месте единицы в маске
    // Либо одну единицу на позиции n, a это не 0, т.е. isHere = 1
    int Index = GetMemIndex(n);
    TELEM Mask = GetMemMask(n);
    int isHere = 0 != (pMem[Index] & Mask);
    return isHere;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{  
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    // Если мы хотим, чтобы операция "или" работала корректно, нам нужно подогнать размер полей
    int RightLen = std::max(BitLen, bf.BitLen);
    int MinMemLen = std::min(MemLen, bf.MemLen);

    TBitField res(RightLen); 
    for (int i = 0; i < MinMemLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i]; 
    }
    if (BitLen > bf.BitLen) {
        for (int i = MinMemLen; i < res.MemLen; i++) {
            res.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = MinMemLen; i < res.MemLen; i++) {
            res.pMem[i] = bf.pMem[i];
        }
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int RightLen = std::max(BitLen, bf.BitLen);
    int MinMemLen = std::min(MemLen, bf.MemLen);

    TBitField res(RightLen);
    for (int i = 0; i < MinMemLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i]; 
    }
    for (int i = MinMemLen; i < res.MemLen; i++) {
        res.pMem[i] = 0;
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < res.MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }
    
    // В предыдущей версии некоректно учитывал лишние биты
    int amountOfRightBits = BitLen % (sizeof(TELEM) * 8);
    if (amountOfRightBits != 0) {
        // Как оказалось, 1 для инта и для лонг лонга - разные вещи, так что надо это учитывать 
        TELEM Mask = (static_cast<TELEM>(1) << amountOfRightBits) - 1;  
        res.pMem[MemLen - 1] &= Mask;       
    }

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++) {
        bool bit;
        // Получаем значение i-ого бита
        istr >> bit;
        // Выставляем полученное значение на i-ый бит
        if (bit) bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        // загружаем поочерёдно биты
        ostr << bf.GetBit(i);
    }
    return ostr;
}
