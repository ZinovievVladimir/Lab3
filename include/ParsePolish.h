#ifndef __PARSE__
#define __PARSE__

struct CNode {
  char* data;
  CNode *next;
};

struct Var {
  char ch;
  double val;
  Var* next;
};

CNode* Add(CNode*, char*);  // Добавление в список

class Polish {
  //char* PolRec;
  Var* vars;
  char* PolRec;

  bool isTrueN(char);
  bool isTrueSymbol(char);
  
  int prt(char); //Приоритет
  CNode* parse(char*);  // Разбиение выражения на части
public:
  Polish(char*);  // Преобразование в польскую запись
  char* GetPolish(); // Выводит польскую запись

  void AddVar(char, int); // Добавляет/изменяет переменную
  double Resultof();  // Результат выражения
};
#endif