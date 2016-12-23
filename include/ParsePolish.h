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

CNode* Add(CNode*, char*);  // ���������� � ������

class Polish {
  //char* PolRec;
  Var* vars;
  char* PolRec;

  bool isTrueN(char);
  bool isTrueSymbol(char);
  
  int prt(char); //���������
  CNode* parse(char*);  // ��������� ��������� �� �����
public:
  Polish(char*);  // �������������� � �������� ������
  char* GetPolish(); // ������� �������� ������

  void AddVar(char, int); // ���������/�������� ����������
  double Resultof();  // ��������� ���������
};
#endif