#pragma once

// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <iostream>
#include <algorithm>

#define SUPPORT_DIVISION 0 // define as 1 when you have implemented the division
#define SUPPORT_IFSTREAM 0 // define as 1 when you have implemented the input >>


// if you do not plan to implement bonus, just delete those lines
class BigNum final
{
public:
    // constructors
    BigNum(){
        value.push_back(0);
    }
    BigNum(int64_t n){
        if (n < 0) {
            sign = false;
            n *= -1;
        }
        do {
            uint8_t digit = n % 10;
            value.push_back(static_cast<signed char>(digit));
            n /= 10;
        } while (n > 0);
    }
    explicit BigNum(const std::string& str){
        for (int32_t i = str.length()-1; i >= 0; i--){
            value.push_back(str[static_cast<unsigned long>(i)] - '0');
        }
    }

    // copy
    BigNum(const BigNum& other){
        this->value = other.value;
        this->sign = other.sign;
    }
    BigNum& operator=(const BigNum& rhs)= default;

    // unary operators
    const BigNum& operator+() const{
        return *this;
    }
    BigNum operator-() const{
        BigNum res = *this;
        if (res.sign){
            res.sign = false;
        }
        else {
            res.sign = true;
        }
        return res;
    }

    // binary arithmetics operators
    BigNum& operator+=(const BigNum& rhs) {
        if (sign == rhs.sign){
            uint32_t min = this->value.size() > rhs.value.size() ? rhs.value.size() : this->value.size();
            int remainder = 0;
            for (uint32_t i = 0; i < min; i++){
                int digit = this->value[i] + rhs.value[i] + remainder;
                remainder = digit / 10;
                this->value[i] = digit%10;
            }
            if(min == this->value.size()){
                for(uint32_t i = min; i < rhs.value.size(); i++){
                    int digit = rhs.value[i] + remainder;
                    remainder = digit / 10;
                    this->value.push_back(digit%10);
                }
            }else {
                for(uint32_t i = min; i < this->value.size(); i++){
                    int digit = this->value[i] + remainder;
                    remainder = digit / 10;
                    this->value[i] = digit%10;
                }
            }
            if (remainder){
                this->value.push_back(remainder);
            }
            return *this;
        }
        else {
            return *this -= (-rhs);
        }
    }

    BigNum& operator-=(const BigNum& rhs){
        if (sign == rhs.sign) {
            if (*this == rhs){
                sign = true;
                value.clear();
                value.push_back(0);
                return *this;
            }
            else {
                BigNum thisAbs = BigNum(*this);
                thisAbs.sign = true;
                BigNum rhsAbs = BigNum(rhs);
                rhsAbs.sign = true;
                int remainder = 0;
                std::vector<int> new_value;
                if (rhsAbs > thisAbs) {
                    std::swap(thisAbs, rhsAbs);
                    sign = !sign;
                }
                for (uint32_t i = 0; i < rhsAbs.value.size(); i++) {
                    int sub = thisAbs.value[i] - rhsAbs.value[i] - remainder;
                    if (sub < 0) {
                        sub = sub + 10;
                        remainder = 1;
                    }
                    else{
                        remainder = 0;
                    }
                    new_value.push_back(sub);
                }
                for (uint32_t i = rhsAbs.value.size(); i < thisAbs.value.size(); i++) {
                    int sub = thisAbs.value[i] - remainder;
                    if (sub < 0) {
                        sub = sub + 10;
                        remainder = 1;
                    }
                    else{
                        remainder = 0;
                    }
                    new_value.push_back(sub);
                }
                value = new_value;
                return *this;
            }
        }
        return *this += (-rhs);
    }

    BigNum& operator*=(const BigNum& rhs){
        if (sign != rhs.sign){
            sign = false;
        }
        else {
            sign = true;
        }
        std::vector<int> new_value(value.size() + rhs.value.size(),0);
        for(uint32_t i = 0; i < value.size(); i++)
        {
            for(uint32_t j = 0; j < rhs.value.size(); j++)
            {
                new_value[i+j] += value[i] * rhs.value[j];
                new_value[i+j+1] = new_value[i+j+1] + new_value[i+j]/10;
                new_value[i+j] %= 10;
            }
        }
        while(new_value[new_value.size()-1] == 0){
            new_value.pop_back();
        }
        value = new_value;
        return *this;
    }
    
#if SUPPORT_DIVISION == 1
    BigNum& operator/=(const BigNum& rhs); // bonus
    BigNum& operator%=(const BigNum& rhs); // bonus
#endif


private:
    std::vector<int> value;
    bool sign = true;
    friend std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);
    friend bool operator==(const BigNum& lhs, const BigNum& rhs);
    friend bool operator!=(const BigNum& lhs, const BigNum& rhs);
    friend bool operator<=(const BigNum& lhs, const BigNum& rhs);
    friend bool operator<(const BigNum& lhs, const BigNum& rhs);
    friend bool operator>(const BigNum& lhs, const BigNum& rhs);
};

#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

BigNum operator-(BigNum lhs, const BigNum& rhs){
    BigNum result;
    result = lhs.operator-=(rhs);
    return result;
}

BigNum operator+(BigNum lhs, const BigNum& rhs){
    BigNum result;
    result = lhs.operator+=(rhs);
    return result;
}

BigNum operator*(BigNum lhs, const BigNum& rhs){
    BigNum result;
    result = lhs.operator*=(rhs);
    return result;
}

bool operator==(const BigNum& lhs, const BigNum& rhs){
    return (rhs.value == lhs.value && lhs.sign == rhs.sign);
}
bool operator!=(const BigNum& lhs, const BigNum& rhs){
    return !operator==(lhs,rhs);
}
bool operator<=(const BigNum& lhs, const BigNum& rhs){
    if (lhs.sign != rhs.sign) {
        if (lhs.sign) {
            return false;
        } else {
            return true;
        }
    }

    if (lhs.sign == rhs.sign) {
        if (lhs.sign) {
            if (lhs.value.size() < rhs.value.size()) {
                return true;
            } else if (lhs.value.size() == rhs.value.size()) {
                for (int32_t i = rhs.value.size()-1; i >= 0; i--) {
                    if (rhs.value[static_cast<unsigned long>(i)] < lhs.value[static_cast<unsigned long>(i)]) {
                        return false;
                    }else if (rhs.value[static_cast<unsigned long>(i)] == lhs.value[static_cast<unsigned long>(i)]){
                        continue;
                    }
                    return true;
                }
            } else {
                return false;
            }
        } else {
            if (lhs.value.size() > rhs.value.size()) {
                return true;
            } else if (lhs.value.size() == rhs.value.size()) {
                for (int32_t i = rhs.value.size()-1; i >= 0; i--) {
                    if (rhs.value[static_cast<unsigned long>(i)] > lhs.value[static_cast<unsigned long>(i)]) {
                        return false;
                    } else if (rhs.value[static_cast<unsigned long>(i)] == lhs.value[static_cast<unsigned long>(i)]){
                        continue;
                    }
                    return true;
                }
            } else {
                return false;
            }
        }
    }
    return true;
}
bool operator>=(const BigNum& lhs, const BigNum& rhs){
    return operator<=(rhs,lhs);
}
bool operator<(const BigNum& lhs, const BigNum& rhs) {
    return operator<=(lhs,rhs) && operator!=(lhs,rhs);
}
bool operator>(const BigNum& lhs, const BigNum& rhs){
    return operator>=(lhs,rhs) && operator!=(lhs,rhs);
}

std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs){
    if (!rhs.sign){
        lhs << "-";
    }
    for (int32_t i = rhs.value.size()-1; i >= 0; i--){
        lhs << std::to_string(rhs.value[static_cast<unsigned long>(i)]);
    }
    return lhs;
}

#if SUPPORT_IFSTREAM == 1
std::istream& operator>>(std::istream& lhs, BigNum& rhs); // bonus
#endif
