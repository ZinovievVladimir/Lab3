#include "ParsePolish.h"
#include "Stack.h"
#include <string.h>
#include <stdexcept>
#include <math.h>
#include <stdio.h>

CNode* Add(CNode* CurNode, char* data) {
  CNode* Node = new CNode;
  Node->data = data;
  Node->next = 0;
  CurNode->next = Node;
  return Node;
}

bool Polish::isTrueN(char a) {
  return (((a >= '0') && (a <= '9')) || ((a >= 'a') && (a <= 'z')));
}

bool Polish::isTrueSymbol(char a) {
  return ((a == '+') || (a == '-') || (a == '/') || (a == '*') ||
    (a == '^') || (a == '(') || (a == ')') || (a == '|') || (a == ' '));
}

void Polish::AddVar(char a, int v) {
  Var* buf = vars;
  int ex = 0;
  while (buf) {
    if (a == buf->ch) {
      ex = 1;
      break;
    }
    buf = buf->next;
  }

  if (ex)
    buf->val = v;
  else {
    Var* newvar = new Var;
    newvar->ch = a;
    newvar->val = v;
    newvar->next = vars;
    vars = newvar;
  }
}

CNode* Polish::parse(char* expl) {
  int state;
  CNode* Parse = new CNode;
  CNode* cur = Parse;
  char * str = new char[10];
  int len = 0, hp = 0;
  char* c;

  if (!expl[0])
    throw std::logic_error("String is empty!");

  int sl = strlen(expl);
  char *exp = new char[sl + 2];
  exp = strcpy(exp, expl);
  exp[sl] = ' ';
  exp[sl + 1] = 0;

  if (isTrueN(exp[0]))
    state = 0;
  else
    state = 1;

  for (int i = 0; exp[i] != 0; i++) {
    c = new char(exp[i]);
    if (state == 0)
      if (isTrueN(*c) || ((*c == '.') && (!hp))) {
        if (*c == '.')
          hp = 1;
        str[len] = *c;
        len++;
        delete c;
      }
      else {
        if (!isTrueSymbol(*c))
          throw std::logic_error("Wrong expression!");
        str[len] = 0;
        cur = Add(cur, str);
        state = 1;
        str = new char[10];
        len = 0;
        hp = 0;
        if (*c != ' ')
          cur = Add(cur, c);
        continue;
      }
      if (state == 1)
        if (isTrueN(*c)) {
          str[len] = *c;
          len++;
          state = 0;
          delete c;
        }
        else {
          if (!isTrueSymbol(*c))
            throw std::logic_error("Wrong expression!");
          if (*c != ' ') {
            cur = Add(cur, c);
          }
        }
  }

  delete[]exp;
  Parse = Parse->next;
  return Parse;
}

int Polish::prt(char c) {
  int res;
  switch (c) {
  case '(':
  case '|': res = 0;
    break;
  case'_':
  case '+':
  case '-': res = 1;
    break;
  case '*':
  case '/': res = 2;
    break;
  case '^': res = 3;
  }
  return res;
}

Polish::Polish(char* exp) {
  vars = 0;

  char* result = new char[strlen(exp) * 2];
  int pos = 0;
  int mod = 0, br = 0, clmod = 0;
  char lastel = ' ';
  CNode* parsed = parse(exp);

  Stack<char> st;

  if (parsed == 0)
    throw std::logic_error("Wrong expression!");

  while (parsed) {
    char * el = parsed->data;
    if (isTrueN(*el)) {
      for (int i = 0; el[i] != 0; i++, pos++)
        result[pos] = el[i];
      result[pos] = ' ';
      pos++;
    }
    else {
      switch (*el) {
      case '(':
        st.push(*el);
        br = 1;
        break;
      case '|':
        if (mod || !pos || (isTrueSymbol(lastel) && (lastel != '|'))) {
          st.push(*el);
          if (parsed->next)
            if (*parsed->next->data == '|')
              mod = 1;
            else
              mod = 0;
          clmod = 1;
        }
        else {
          while (st.gettop() != '|') {
            result[pos++] = st.gettop();
            st.pop();
          }
          result[pos++] = '|';
          st.pop();
          clmod = 0;
        }
        break;
      case ')':
        if (st.gettop() == '(')
          throw std::logic_error("Wrong expression!");
        if (br)
        {
          while (st.gettop() != '(') {
            result[pos] = st.gettop();
            pos++;
            st.pop();
          }
        }
        else
          throw std::logic_error("Wrong expression!");
        st.pop();
        break;
      default:
        while (!st.empty()) {
          if ((prt(*el)) <= prt(st.gettop())) {
            result[pos] = st.gettop();
            pos++;
            st.pop();
          }
          else
            break;
        }

        if (*el == '-') {  // унарный минус
          if (!pos)
            st.push('_');
          else {
            if ((lastel == '|') && (clmod))
              st.push('_');
            else
              if (!st.empty()) {
                if (lastel == '('){
                  st.push('_');
                }
                else
                  st.push(*el);
              }
              else
                st.push(*el);
          }
        }
        else
          st.push(*el);
      }
    }
    parsed = parsed->next;
    lastel = *el;
  }

  while (!st.empty()) {
    if ((st.gettop() != '(') && (st.gettop() != '|'))
      result[pos] = st.gettop();
    else
      throw std::logic_error("Wrong expression!");
    pos++;
    st.pop();
  }

  result[pos] = 0;

  PolRec = result;
}

double Polish::Resultof() {

  if (!PolRec)
    throw std::logic_error("Wrong expression!");

  int len = strlen(PolRec), slen;
  double a, b;

  Var* buf;

  Stack<double> st;
  char* num;

  for (int i = 0; i < len; i++) {
    if ((PolRec[i] >= 'a') && (PolRec[i] <= 'z')) {
      buf = vars;
      while (buf) {
        if (buf->ch == PolRec[i])
          break;
        buf = buf->next;
      }
      if (!buf)
        throw std::logic_error("Variables are not declared!");
      st.push(buf->val);
      i++;
    }
    else
      if ((PolRec[i] >= '0') && (PolRec[i] <= '9')) {
        num = new char[10];
        slen = 0;
        while (PolRec[i] != ' ') {
          if ((PolRec[i] >= 'a') && (PolRec[i] <= 'z'))
            throw std::logic_error("Wrong expression");
          num[slen] = PolRec[i];
          slen++;
          i++;
        }
        num[slen] = 0;
        st.push(atof(num));
      }
      else
        if (!st.empty()) {
          b = st.gettop();
          st.pop();
          if ((PolRec[i] != '|') && (PolRec[i] != '_')) {
            if (!st.empty()) {
              a = st.gettop();
              st.pop();
            }
            else
              throw std::logic_error("Wrong expression!");
          }

          switch (PolRec[i]) {
          case '+':
            st.push(a + b);
            break;
          case '-':
            st.push(a - b);
            break;
          case '/':
            st.push(a / b);
            break;
          case '*':
            st.push(a * b);
            break;
          case '^':
            st.push(pow(a, b));
            break;
          case '|':
            st.push(abs(b));
            break;
          case '_':
            st.push(-b);
            break;
          default:
            throw std::logic_error("Wrong expression!");
          }
        }
        else
          throw std::logic_error("Wrong expression!");
  }

  double result = st.gettop();
  st.pop();
  if (st.empty())
    return result;
  else
     throw std::logic_error("Wrong expression!");
}

char* Polish::GetPolish() {
  return PolRec;
}