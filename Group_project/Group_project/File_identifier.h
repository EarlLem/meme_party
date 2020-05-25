#pragma once
#include "CSVReader.h"
#include "RKmethod.h"
#include <map>
#include <cstdlib>


class Handler {
public:
    virtual Handler* SetNext(Handler* handler) = 0;
    virtual algorithm* Handle(std::string method) = 0;
};

class AbstractHandler : public Handler {
private:
    Handler* next_handler_;

public:
    AbstractHandler() : next_handler_(nullptr) {
    }
    Handler* SetNext(Handler* handler) override {
        this->next_handler_ = handler;
        return handler;
    }
    algorithm* Handle(std::string method) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(method);
        }

        return {};
    }
};

class RKmethod_5_4_7fs_Handler :public AbstractHandler
{
public:
    algorithm* Handle(std::string method) override
    {
        std::stringstream ss(method);
        std::string rk_method, syst;
        ss >> rk_method >> syst;
        if (rk_method == "RKmethod_5_4_7fs")
        {
            if (syst == "LVOde")
            {
                return new RKintegrator<RKmethod_5_4_7fs, LVOde>;
            }
            else if (syst == "Osc_Van_der_Pol")
            {
                return new RKintegrator<RKmethod_5_4_7fs, Osc_Van_der_Pol>;
            }
            else if (syst == "Attractor_Lorence")
            {
                return new RKintegrator<RKmethod_5_4_7fs, Attractor_Lorence>;
            }
            else if (syst == "Chua_chain")
            {
                return new RKintegrator<RKmethod_5_4_7fs, Chua_chain>;
            }
        }
        else {
            return AbstractHandler::Handle(method);
        }
    }
};

class VernerRK_5_6_8_Handler :public AbstractHandler
{
public:
    algorithm* Handle(std::string method) override
    {
        std::stringstream ss(method);
        std::string rk_method, syst;
        ss >> rk_method >> syst;
        if (rk_method == "VernerRK_5_6_8")
        {
            if (syst == "LVOde")
            {
                return new RKintegrator<VernerRK_5_6_8, LVOde>;
            }
            else if (syst == "Osc_Van_der_Pol")
            {
                return new RKintegrator<VernerRK_5_6_8, Osc_Van_der_Pol>;
            }
            else if (syst == "Attractor_Lorence")
            {
                return new RKintegrator<VernerRK_5_6_8, Attractor_Lorence>;
            }
            else if (syst == "Chua_chain")
            {
                return new RKintegrator<VernerRK_5_6_8, Chua_chain>;
            }
        }
        else {
            return AbstractHandler::Handle(method);
        }
    }
};

/*
class RK57FEq1_Handler :public AbstractHandler
{
public:
    algorithm* Handle(std::string method) override
    {
        std::stringstream ss(method);
        std::string rk_method, syst;
        ss >> rk_method >> syst;
        if (rk_method == "RK57FEq1")
        {
            if (syst == "LVOde")
            {
                return new RKintegrator<RK57FEq1, LVOde>;
            }
            else if (syst == "Osc_Van_der_Pol")
            {
                return new RKintegrator<RK57FEq1, Osc_Van_der_Pol>;
            }
            else if (syst == "Attractor_Lorence")
            {
                return new RKintegrator<RK57FEq1, Attractor_Lorence>;
            }
            else if (syst == "Chua_chain")
            {
                return new RKintegrator<RK57FEq1, Chua_chain>;
            }
        }
        else {
            return AbstractHandler::Handle(method);
        }
    }
};


class RK68M_Handler :public AbstractHandler
{
public:
    algorithm* Handle(std::string method) override
    {
        std::stringstream ss(method);
        std::string rk_method, syst;
        ss >> rk_method >> syst;
        if (rk_method == "RK68M")
        {
            if (syst == "LVOde")
            {
                return new RKintegrator<RK68M, LVOde>;
            }
            else if (syst == "Osc_Van_der_Pol")
            {
                return new RKintegrator<RK68M, Osc_Van_der_Pol>;
            }
            else if (syst == "Attractor_Lorence")
            {
                return new RKintegrator<RK68M, Attractor_Lorence>;
            }
            else if (syst == "Chua_chain")
            {
                return new RKintegrator<RK68M, Chua_chain>;
            }
        }
        else {
            return AbstractHandler::Handle(method);
        }
    }
};
*/
void Identifier(const std::string& file_of_files, std::map<std::string, algorithm*>& mp)
{
    AbstractHandler* rk1 = new RKmethod_5_4_7fs_Handler;
    AbstractHandler* rk2 = new VernerRK_5_6_8_Handler;
    //AbstractHandler* rk3 = new RK57FEq1_5_6_8_Handler;
    //AbstractHandler* rk4 = new RK68M_Handler;
    rk1->SetNext(rk2);//->SetNext(rk3)->SetNext(rk4);

    std::ifstream file_of_file(file_of_files, std::ios::in);
    std::string file_name;
    if (file_of_file.is_open())
    {
        while (!file_of_file.eof())
        {
            std::getline(file_of_file, file_name);
            if (file_name.length() < 1)
            {
                break;
            }
            std::ifstream file(file_name, std::ios::in);
            std::string method;
            if (file.is_open())
            {
                std::getline(file, method);
                mp[file_name] = rk1->Handle(method);
            }
            else
            {
                cout << "Error! Couldn't open file with ALGORITHMS" << std::endl;
            }
            file.close();
            file_name.clear();
        }
    }
    else
    {
        std::cout << "Error! Coldn't open file with FILES" << std::endl;
    }
    file_of_file.close();
}

int Identify_file_test()
{
    std::map<std::string, algorithm*> lol;//создаем изначально мап куда забиваем данны
    Identifier("C:\\Users\\lkdes\\source\\repos\\trash\\kek.csv", lol);//сюда идет имя файла и ссылка на мап
    system("pause");
    return 0;
}