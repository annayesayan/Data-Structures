#ifndef SHUNTING_Y_H
#define SHUNTING_Y_H
#include <iostream>
#include "../../../Cs8/!includes/queue_n_stack/queue.h"
#include "../../../Cs8/!includes/queue_n_stack/stack.h"
using namespace std;

enum Presedence {OR, AND};

class shunting_yard
{
public:
    shunting_yard();
    shunting_yard(Queue<string> infix): _infix(infix){}
    Queue <string> postfix()       //returns postfix
    {
        Stack<Presedence> s;

            while(!_infix.empty()){
                   string t = _infix.pop();
                   if(t=="and"){
                       if(s.empty() || s.front() < AND)
                            s.push(AND);
                       else if(!s.empty() && s.front() >= AND){
                           _postfix.push(t);
                           s.push(AND);
                       }
                   }else if (t=="or") {
                       if(s.empty() || s.front() < OR)
                            s.push(OR);
                       else if(!s.empty() && s.front() >= OR){
                           if(s.pop()==AND)
                                _postfix.push("and");
                           else
                               _postfix.push("or");
                           s.push(OR);
                       }
                   }else {
                        _postfix.push(t);
                   }
            }
            while(!s.empty()){             //pop rest of stack if infix is already empty
                if(s.pop()==AND)
                     _postfix.push("and");
                else
                    _postfix.push("or");
            }
            //cout<<"_postfix: "<<_postfix<<endl;
            return _postfix;
    }
private:
    Queue <string> _infix;
    Queue <string> _postfix;
};
#endif // SHUNTING_Y_H
