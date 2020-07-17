#include "answerdeclaration.h"
#include <random>

AnswerDeclaration::AnswerDeclaration(int input_order_ID, bool answer) {
    ID = rand();
    verdict = answer;
    this->input_order_ID = input_order_ID;
}

AnswerDeclaration::operator bool() const noexcept { return verdict; }

bool AnswerDeclaration::cmp_ID(int ID) { return (ID == input_order_ID); }
